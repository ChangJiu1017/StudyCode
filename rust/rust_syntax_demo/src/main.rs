// 宏定义：用于简化重复代码
#[macro_use]
extern crate thiserror;



// 自定义错误类型（使用 thiserror crate）
#[derive(Debug, Error)]
pub enum TaskError {
    #[error("任务 '{0}' 不存在")]
    TaskNotFound(String),
}

// 枚举：任务状态
#[derive(Debug, Clone, Copy, PartialEq)]
enum TaskStatus {
    Pending,
    Completed,
}

// 结构体：任务信息
#[derive(Debug, Clone)]
struct Task {
    title: String,
    status: TaskStatus,
}

// 模块系统：将功能模块化
mod task_manager {
    use std::collections::HashMap;
    use super::{Task, TaskStatus, TaskError};

    // 使用 HashMap 管理任务
    pub struct TaskManager {
        tasks: HashMap<String, Task>,
    }

    impl TaskManager {
        // 构造函数
        pub fn new() -> Self {
            TaskManager {
                tasks: HashMap::new(),
            }
        }

        // 添加任务
        pub fn add_task(&mut self, title: &str) {
            let task = Task {
                title: title.to_string(),
                status: TaskStatus::Pending,
            };
            self.tasks.insert(title.to_string(), task);
        }

        // 删除任务
        pub fn remove_task(&mut self, title: &str) -> Result<(), TaskError> {
            if self.tasks.remove(title).is_none() {
                return Err(TaskError::TaskNotFound(title.to_string()));
            }
            Ok(())
        }

        // 查找任务
        pub fn get_task(&self, title: &str) -> Option<&Task> {
            self.tasks.get(title)
        }

        // 获取所有任务
        pub fn list_tasks(&self) -> Vec<&Task> {
            self.tasks.values().collect()
        }

        // 使用迭代器和闭包筛选未完成的任务
        pub fn list_pending_tasks(&self) -> Vec<&Task> {
            self.tasks.values()
                .filter(|task| task.status == TaskStatus::Pending)
                .collect()
        }

        // 使用闭包作为函数参数
        pub fn apply_to_all<F>(&mut self, f: F)
        where
            F: Fn(&mut Task),
        {
            for task in self.tasks.values_mut() {
                f(task);
            }
        }
    }
}

// 主函数
fn main() {
    // 可变变量
    let mut manager = task_manager::TaskManager::new();

    // 添加任务
    manager.add_task("学习 Rust");
    manager.add_task("编写示例代码");
    manager.add_task("阅读文档");

    // 修改任务状态（使用闭包）
    manager.apply_to_all(|task| {
        task.status = TaskStatus::Completed;
    });

    // 使用迭代器和闭包列出已完成任务
    let completed_tasks: Vec<_> = manager.list_tasks()
        .into_iter()
        .filter(|task| task.status == TaskStatus::Completed)
        .collect();

    println!("已完成的任务:");
    for task in &completed_tasks {
        println!("- {}", task.title);
    }

    // 使用模式匹配处理删除任务的结果
    match manager.remove_task("阅读文档") {
        Ok(()) => println!("任务 '阅读文档' 已成功删除"),
        Err(e) => println!("删除任务失败: {}", e),
    }

    // 使用宏输出任务信息
    macro_rules! print_task_info {
        ($task:expr) => {
            println!("任务: {}, 状态: {:?}", $task.title, $task.status);
        };
    }

    // 使用宏输出任务
    if let Some(task) = manager.get_task("学习 Rust") {
        print_task_info!(task);
    }

    // 控制流：if-else
    let is_empty = manager.list_tasks().is_empty();
    if is_empty {
        println!("任务列表为空");
    } else {
        println!("还有未完成的任务");
    }

    // 控制流：loop
    let mut count = 0;
    loop {
        if count >= 3 {
            break;
        }
        println!("循环计数: {}", count);
        count += 1;
    }

    // 控制流：while
    let mut number = 3;
    while number != 0 {
        println!("{}!", number);
        number -= 1;
    }

    // 控制流：for
    let numbers = [1, 2, 3, 4, 5];
    for number in numbers.iter() {
        println!("数字: {}", number);
    }

    // 使用 Result 处理错误
    match manager.get_task("不存在的任务") {
        Some(task) => println!("找到任务: {}", task.title),
        None => println!("任务不存在"),
    }
}