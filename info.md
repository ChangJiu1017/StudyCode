使用 `adb` 查看 Android 设备的资源占用情况，可以通过以下几类命令查看 **CPU、内存、磁盘、网络** 等资源的使用情况。以下是常用命令及说明：

---

## 一、查看当前运行的进程和资源占用

### 1. 查看进程及资源占用（类似于 Linux 的 top）

```bash
adb shell top

```

默认刷新进程信息（CPU、内存等），可以加参数控制：

```bash
adb shell top -n 1 -m 10

```

说明：

- `n 1`：只刷新一次
- `m 10`：显示前10个进程
- 默认显示内容包括 PID、CPU%、MEM%、NAME 等

---

## 二、查看内存使用情况

### 2. 查看总体内存占用：

```bash
adb shell cat /proc/meminfo

```

可以看到 `MemTotal`、`MemFree`、`Buffers`、`Cached` 等信息。

### 3. 查看某个包的内存占用（以包名为例）：

```bash
adb shell dumpsys meminfo com.example.app

```

这会显示该应用的详细内存信息，比如：

- Java Heap
- Native Heap
- Code、Stack、Graphics 等

---

## 三、查看 CPU 使用情况

### 4. 查看总体 CPU 信息：

```bash
adb shell cat /proc/stat

```

也可以用 `top` 或 `dumpsys cpuinfo`：

```bash
adb shell dumpsys cpuinfo

```

这会列出当前所有进程的 CPU 使用率占比。

---

## 四、查看磁盘使用情况

### 5. 查看磁盘空间占用：

```bash
adb shell df

```

或：

```bash
adb shell df -h

```

显示系统各分区的使用情况，例如 `/data`、`/system` 等。

---

## 五、查看网络流量

### 6. 查看某个 UID 的网络使用情况（流量）：

先查包名对应的 UID：

```bash
adb shell dumpsys package com.example.app | grep userId

```

再查看该 UID 使用的流量：

```bash
adb shell cat /proc/net/xt_qtaguid/stats

```

过滤该 UID：

```bash
adb shell cat /proc/net/xt_qtaguid/stats | grep <uid>

```

---

## 六、图形化方式（辅助工具）

也可以使用 **Android Studio** 的 **Device Monitor** 或 **Profiler**，更直观查看 CPU、内存、网络等使用情况。

---

需要我帮你编写一个自动统计脚本来简化这些命令的使用吗？

在 Android 设备上查看 **GPU 使用情况** 相比 CPU、内存更复杂一些，因为 Android 系统本身并不直接提供完整的 `GPU usage` 命令。但你可以通过以下几种方式间接或直接查看 GPU 的使用情况：

---

## 一、使用 dumpsys SurfaceFlinger（系统渲染帧率和GPU压力）

```bash
adb shell dumpsys SurfaceFlinger --latency

```

或者查看每帧渲染耗时情况（有助于评估 GPU 压力）：

```bash
adb shell dumpsys gfxinfo com.example.app

```

输出内容包括：

- **Draw、Process、Execute** 的耗时（单位 ms）
- 帧率（FPS）
- 是否有 **帧丢失（Jank）**

---

## 二、使用 dumpsys gpu（某些厂商设备支持）

部分设备支持：

```bash
adb shell dumpsys gpu

```

但很多设备并不支持该命令，执行会提示找不到 service。

---

## 三、使用 systrace 分析 GPU、CPU 等系统资源使用

```bash
adb shell atrace gfx view sched freq

```

或者更完整：

```bash
adb shell atrace -t 10 gfx view wm am res sched freq

```

输出的是跟踪数据，可用于分析 GPU 工作时间，需配合工具（如 `Systrace` 或 `Perfetto UI`）查看。

---

## 四、使用开发者选项 GPU 调试功能（图形方式）

打开设备的开发者选项：

1. **设置 -> 关于手机 -> 连续点击“版本号”启用开发者选项**
2. 开启以下选项：
    - **GPU呈现模式分析：**
        
        ```
        设置 -> 开发者选项 -> GPU呈现模式分析 -> 显示屏幕更新条形图
        
        ```
        
    - **启用 GPU 调试层（Profile GPU Rendering）**
        - 显示每帧 GPU 渲染时间柱状图

这可以在设备屏幕上可视化地看到 GPU 每帧的工作时间。

---

## 五、使用 `adb shell cat /sys/class/kgsl/kgsl-3d0/gpu_busy_percentage`

部分高通平台支持这个接口：

```bash
adb shell cat /sys/class/kgsl/kgsl-3d0/gpu_busy_percentage

```

这个文件表示当前 GPU 的使用率百分比（通常 0~100），是 **最直接的 GPU 占用率指标**。

如果设备支持该路径，建议用脚本持续查看：

```bash
watch -n 1 adb shell cat /sys/class/kgsl/kgsl-3d0/gpu_busy_percentage

```

---

## 六、其它方式（厂商工具）

- 高通平台：使用 **Adreno Profiler**
- Mali GPU：使用 **Mali Graphics Debugger**
- Imagination GPU：使用 **PVRPerfServer（PowerVR Profiler）**

这些都需要设备支持并配合特定驱动使用。

---

### 小结：常见方法概览

| 方法 | 可行性 | 说明 |
| --- | --- | --- |
| `adb shell cat /sys/class/kgsl/...` | 高通设备支持 | 直接查看 GPU 占用率 |
| `dumpsys gfxinfo` | 所有设备支持 | 间接评估 GPU 渲染压力 |
| `Systrace / atrace` | 通用但复杂 | 用于性能分析 |
| 开发者选项图形分析 | 直观有效 | 适合手动分析渲染性能 |

---

你使用的是哪种芯片（如高通、Mali、PowerVR），我可以帮你具体查一下是否支持直接获取 GPU 使用率。

查看 GPU 占用率是有简便方法的，不一定需要通过 `perfetto` 这样的工具。对于大部分 Android 设备，**MTK** 和 **其他平台**，你可以使用以下方法直接获取 GPU 使用率。

### 1. **通过 `adb shell` 直接读取 GPU 使用率**

### 方法一：通过 `devfreq` 目录（MTK 设备支持）

在 MTK 设备上，GPU 频率和负载可以通过 `devfreq` 文件系统来查看。可以通过下面的命令查看 GPU 的使用情况：

```bash
adb shell cat /sys/class/devfreq/10040000.gpu/load

```

- `10040000.gpu` 是典型的 GPU 设备文件，你需要根据实际设备路径来替换。
- 这个命令会返回一个数字，通常是 GPU 的 **负载百分比**，例如 `75` 表示 GPU 使用率为 75%。

### 方法二：通过 `/proc/gpufreq`

对于某些 MTK 设备，你可以使用以下命令来获取 GPU 占用率：

```bash
adb shell cat /proc/gpufreq/gpu_loading

```

它会返回当前 GPU 的使用率百分比。

### 方法三：通过 `atrcace` 获取 GPU 性能信息

如果你不想配置复杂的追踪工具，可以通过 `atrace` 来简单地采集 GPU 的信息：

```bash
adb shell atrace -t 10 -c gfx

```

- `t 10`: 跟踪 10 秒
- `c gfx`: 只跟踪 GPU 相关的事件

---

### 2. **使用 `adb shell dumpsys` 命令**

另外，`dumpsys` 命令也可以提供有关 GPU 使用的信息，尤其是在 Android 上，像 `gfx` 和 `power` 相关信息：

```bash
adb shell dumpsys gfxinfo

```

这将显示关于 **渲染时间、帧率** 和 **GPU 使用情况** 的信息。

例如：

```bash
adb shell dumpsys gfxinfo com.example.app

```

这会显示某个应用程序的渲染性能和 GPU 使用情况。

---

### 3. **简化的实时监控方法**

你可以使用 `watch` 命令实时监控 GPU 使用率：

```bash
watch -n 1 adb shell cat /sys/class/devfreq/10040000.gpu/load

```

每秒（或你设置的间隔）更新一次 GPU 使用率。

---

## 结论

如果你只想查看 GPU 占用率，最简单的方法就是使用 **`cat`** 命令直接访问设备上的 `devfreq` 或 `gpufreq` 文件系统，而 **`adb shell atrace`** 和 **`dumpsys`** 命令则适合更复杂的性能分析。你可以选择其中一种方法来实现查看 GPU 使用情况。

需要帮助查找设备路径或其他相关命令吗？