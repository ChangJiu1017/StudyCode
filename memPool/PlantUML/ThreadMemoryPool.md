下面给出一份完整的 PlantUML 代码设计流程图，涵盖核心组件关系（类图）与关键操作流程（时序图）：线程注册、分配、跨线程释放、回捞、页耗尽与扩容。可直接保存为 .puml 使用。

----------------
类图（架构关系）
----------------
@startuml
title 线程私有池 + SPSC 邮箱 + 跨线程归还（架构关系）

class MemPoolAPI {
  +init_pool()
  +register_thread(): int
  +unregister_thread(): int
  +malloc_from_pool(size): void*
  +calloc_from_pool(nmemb,size): void*
  +realloc_from_pool(ptr,size): void*
  +free_to_pool(ptr): int
  +pool_total_bytes(): size_t
  +thread_remote_drained(): size_t
}

class ThreadPool {
  -tid: int
  -local_free: Block*
  -cur_page: Page*
  -all_pages: Page*
  -remote_drained: size_t
  -inbox_from[MAX_THREADS]: SPSCQueue
  -inbox_ready[MAX_THREADS]: int
  -deferred: Block*
  --
  +drain_inboxes()
  +bump_alloc_block(): void*
}

class Block {
  -next: Block*
  -owner_tid: int
}

class Page {
  -base: uint8_t*
  -curr: uint8_t*
  -end:  uint8_t*
  -next: Page*
}

class SPSCQueue {
  -head: uint32_t
  -tail: uint32_t
  -cap_mask: uint32_t
  -buf: void** 
  --
  +enqueue(p: void*): bool
  +dequeue(): void*
}

class ThreadTable {
  -g_threads[MAX_THREADS]: ThreadPool*
  -g_thread_count: int
  --
  +get(tid: int): ThreadPool*
  +add(tp: ThreadPool*)
}

class InitNode {
  -total_size: ulong
  -cur_base: uint8_t*
  -mem_base: uint8_t*
  -rb: RingBuffer
}

class RingBuffer {
  -head: uint64_t
  -tail: uint64_t
  -Length: uint64_t
  -array_addr: uint64_t*
  -max_Length: uint64_t
  --
  +RB_Init(...): uint8_t
  +RB_Write_Addr(...): uint8_t
  +RB_Read_Addr(...): uint8_t
}

note right of RingBuffer
  仅为兼容保留；
  不参与并发路径
  (跨线程归还改用 SPSC)
end note

MemPoolAPI --> ThreadTable : 通过线程注册表定位所有者
ThreadPool "1" o-- "many" Page : 页面链表(私有)
ThreadPool "1" o-- "many" SPSCQueue : inbox_from[tid]
ThreadPool "1" o-- "many" Block : local_free
Block --> ThreadPool : owner_tid 路由归还
MemPoolAPI ..> SPSCQueue : free_to_pool 使用入站邮箱
MemPoolAPI ..> ThreadPool : 线程本地池主处理
MemPoolAPI ..> InitNode : 兼容保留(不并发)

@enduml

----------------
时序图：线程注册
----------------
@startuml
title 线程注册 register_thread 流程

actor Thread as T
participant MemPoolAPI as API
participant ThreadTable as TT
participant ThreadPool as TP

T -> API : register_thread()
API -> TT : get TLS pool?
TT --> API : null (首次)
API -> TP : create ThreadPool()
API -> TP : alloc_page(PAGE_SIZE)\n初始化 local_free, cur_page, inbox
API -> TT : add(tp)
API --> T : return tid

@enduml

----------------
时序图：分配 malloc_from_pool
----------------
@startuml
title 分配流程 malloc_from_pool(size)

actor Thread as T
participant MemPoolAPI as API
participant ThreadPool as TP
participant SPSCQueue as Inbox

T -> API : malloc_from_pool(size)
API -> TP : ensure TLS & get tp
alt size == 0
  API --> T : return NULL
else size > BLOCK_SIZE
  API --> T : return system malloc
else pool path
  alt tp.local_free 非空
    API -> TP : pop from local_free
    API --> T : return block.payload
  else 需要回捞
    API -> TP : drain_inboxes()
    loop for each source inbox
      TP -> Inbox : dequeue*
      Inbox --> TP : batch blocks
      TP -> TP : push into local_free
    end
    alt 回捞后有块
      API -> TP : pop from local_free
      API --> T : return block.payload
    else bump 分配
      API -> TP : bump_alloc_block()
      API --> T : return block.payload (owner_tid=tp.tid)
    end
  end
end

@enduml

----------------
时序图：跨线程释放 free_to_pool
----------------

@startuml
title 跨线程释放 free_to_pool(ptr)

actor Releaser as R
participant MemPoolAPI as API
participant ThreadTable as TT
participant ThreadPool as Owner
participant SPSCQueue as Inbox

R -> API : free_to_pool(ptr)
API -> API : b = ptr - sizeof(Block)
API -> API : owner = b.owner_tid

' 第一层判断：是否为当前线程
alt owner == current_thread
    API -> Owner : push b into local_free
    API --> R : return
else owner != current_thread
    ' 使用 group 明确创建一个嵌套作用域，避免语法歧义
    group 跨线程处理
        API -> TT : get owner ThreadPool
        ' 第二层判断：owner 线程是否存在
        alt owner EXISTS
            API -> Inbox : enqueue(ptr)
            ' 第三层判断：队列状态
            alt 队列满或未就绪
                API -> Owner : push b into deferred ' <-- 修正: 使用已定义的参与者 "Owner"
            end ' <-- 闭合第三层 alt
            API --> R : return
        else owner 不存在(已退出)
            API --> R : system free(ptr)
        end ' <-- 闭合第二层 alt
    end ' <-- 闭合 group
end ' <-- 闭合第一层 alt

@enduml

----------------
时序图：回捞 drain_inboxes
----------------
@startuml
title 回捞 drain_inboxes()

participant ThreadPool as TP
participant SPSCQueue as Inbox

TP -> TP : for each source
loop 批量出队
  TP -> Inbox : dequeue()
  alt 得到 ptr
    Inbox --> TP : ptr
    TP -> TP : b = ptr - sizeof(Block)\n b->next = local_free
    TP -> TP : remote_drained++
  else 队列空
    Inbox --> TP : NULL
    break
  end
end
@enduml

----------------
时序图：页耗尽与扩容
----------------
@startuml
title 页耗尽 bump_alloc_block

participant ThreadPool as TP

TP -> TP : need = sizeof(Block)+BLOCK_SIZE
alt cur_page.remaining < need
  TP -> TP : alloc_page(PAGE_SIZE)
  TP -> TP : new_page.next = all_pages\ncur_page = new_page
end
TP -> TP : p = cur_page.curr ; cur_page.curr += need
TP -> TP : b = (block*)p ; b.owner_tid = tp.tid ; b.next = NULL
TP --> TP : return (p + sizeof(block)) as payload

@enduml

----------------
流程约束与要点（注释块）
----------------
@startuml
title 关键约束与要点（注释）

note as N1
- 主路径无锁无原子：分配与本线程释放只操作 ThreadPool 私有成员。[1]
- 跨线程释放：释放方不改所有者结构，只把指针入队到所有者的 SPSC 收件箱。[2]
- SPSC 队列：严格单生产者单消费者，生产者只写 tail，消费者只写 head。[1]
- 顺序保证：入队先写数据后推进 tail；出队观察 tail 后读数据再推进 head（编译器屏障）。[1]
- 所有权模型：block.header.owner_tid 固定归属；非所有者线程不可直接复用该块。[2]
- 收件箱满：入当前线程 deferred，稍后由所有者吸收或重试入箱。[3]
- ring_buffer：仅兼容保留，不参与跨线程（避免并发数据竞争）。[4]
end note
@enduml

[1](http://www.360doc.com/content/24/0417/13/81013658_1120645331.shtml)
[2](https://blog.csdn.net/luozenghui529480823/article/details/52953413)
[3](https://mbb.eet-china.com/tech/t9/163897.html)
[4](https://kmdreko.github.io/posts/20191003/a-simple-lock-free-ring-buffer/)