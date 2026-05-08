stm32H7裸机程序在主循环2ms循环添加任务超时和CPU占用判断程序
在STM32H7的裸机程序中，要在一个2ms的主循环里实现任务超时管理和CPU占用率判断，可以结合定时器中断和在主循环中维护状态标志来完成。
下面是一个完整的实现思路和代码框架。
📝 核心思路
时间基准：使用一个定时器（如SysTick）产生一个固定的时间基准（例如1ms），并在中断服务程序中维护一个全局的 system_ticks 变量。
任务管理：定义一个任务结构体，包含任务函数指针、执行间隔、上次执行时间、超时时间、开始执行时间等信息。
CPU占用判断：在主循环中，通过记录一个“空闲”标志位，并在一个高频定时器中断中采样这个标志位，来统计CPU的空闲时间和忙碌时间，从而计算出占用率。
💻 代码实现
以下代码展示了如何将任务调度和CPU占用率计算整合在一个2ms的主循环中。
1. 定义任务结构体和全局变量
c

编辑



#include <stdint.h>
#include <stdio.h>

// 假设 system_ticks 在别处（如SysTick中断）定义并递增
extern volatile uint32_t system_ticks; 

// 最大任务数量
#define MAX_TASKS 10

// 任务结构体
typedef struct {
    void (*TaskFunction)(void); // 任务函数指针
    uint32_t Interval;          // 任务执行间隔 (ms)
    uint32_t LastRun;           // 上次执行时间戳 (ms)
    uint32_t Timeout;           // 任务超时时间 (ms)
    uint32_t StartTime;         // 任务本次开始执行的时间戳 (ms)
    uint8_t State;              // 任务状态 (0:停止, 1:运行)
    uint8_t Flag;               // 任务标志 (0:未就绪, 1:就绪)
    uint8_t TimeoutFlag;        // 超时标志 (0:未超时, 1:已超时)
} Task_t;<websource>source_group_web_2</websource>

// 任务数组和任务计数
Task_t Tasks[MAX_TASKS];
uint8_t TaskCount = 0;

// CPU占用率统计变量
volatile uint32_t cpu_total_time = 0; // 总采样时间
volatile uint32_t cpu_idle_time = 0;  // 空闲采样时间
volatile uint8_t cpu_usage_percent = 0; // CPU占用率百分比
volatile uint8_t cpu_is_idle = 1;     // CPU空闲标志，1为空闲，0为忙碌
```<websource>source_group_web_3</websource>

#### 2. 实现任务管理函数

```c
// 添加任务函数
void AddTask(void (*func)(void), uint32_t interval, uint32_t timeout) {
    if (TaskCount < MAX_TASKS) {
        Tasks[TaskCount].TaskFunction = func;
        Tasks[TaskCount].Interval = interval;
        Tasks[TaskCount].Timeout = timeout;
        Tasks[TaskCount].LastRun = 0;
        Tasks[TaskCount].StartTime = 0;
        Tasks[TaskCount].State = 1; // 初始为运行状态
        Tasks[TaskCount].Flag = 1;  // 初始为就绪状态
        Tasks[TaskCount].TimeoutFlag = 0;
        TaskCount++;
    }
}

// 处理任务超时的函数
void HandleTimeout(uint8_t task_id) {
    printf("Task %d timeout!\n", task_id);
    // 这里可以添加超时后的处理逻辑，例如：
    // 1. 停止任务: Tasks[task_id].State = 0;
    // 2. 重启任务: Tasks[task_id].StartTime = system_ticks;
    // 3. 记录错误日志
    Tasks[task_id].TimeoutFlag = 0; // 清除超时标志
}
3. 主程序与2ms循环
c

编辑



// 假设这是SysTick的中断服务程序，提供1ms的时间基准
// void SysTick_Handler(void) {
//     system_ticks++;
// }

// 假设这是另一个定时器中断，例如1ms中断，用于CPU占用率采样
// void Timer_CPU_Usage_ISR(void) {
//     cpu_total_time++;
//     if (cpu_is_idle == 1) {
//         cpu_idle_time++;
//     }
//     // 每秒计算一次CPU占用率
//     if (cpu_total_time >= 1000) {
//         cpu_usage_percent = (cpu_total_time - cpu_idle_time) * 100 / cpu_total_time;
//         cpu_total_time = 0;
//         cpu_idle_time = 0;
//         // 可以通过串口打印，或在主循环中处理
//     }
// }<websource>source_group_web_4</websource>

// 示例任务函数
void Task1(void) {
    // 模拟一个耗时操作
    for(volatile int i=0; i<10000; i++);
}

void Task2(void) {
    // 另一个任务
}

int main(void) {
    // 系统初始化 (时钟、定时器、串口等)
    // HAL_Init();
    // SystemClock_Config();
    // TIMx_Init(); // 初始化用于CPU采样的定时器

    // 添加任务
    AddTask(Task1, 100, 500); // Task1每100ms执行一次，超时时间为500ms
    AddTask(Task2, 200, 1000); // Task2每200ms执行一次，超时时间为1000ms

    uint32_t last_main_loop_tick = 0;

    while (1) {
        // --- 2ms 主循环 ---
        // 确保主循环大约每2ms执行一次，可以通过HAL_Delay(2)或检查system_ticks实现
        // 这里简化为一直执行，实际项目中需要控制循环周期
        // HAL_Delay(2); 

        // 标记CPU开始忙碌
        cpu_is_idle = 0; 

        // 遍历并调度所有任务
        for (uint8_t i = 0; i < TaskCount; i++) {
            // 检查任务是否就绪且正在运行
            if (Tasks[i].Flag == 1 && Tasks[i].State == 1) {
                
                // --- 任务超时检测 ---
                // 如果任务已启动且执行时间超过超时设定
                if (Tasks[i].StartTime > 0 && (system_ticks - Tasks[i].StartTime > Tasks[i].Timeout)) {
                    Tasks[i].TimeoutFlag = 1;
                    Tasks[i].State = 0; // 停止任务
                    HandleTimeout(i);
                }

                // --- 任务周期性调度 ---
                // 检查是否到达执行间隔
                if (system_ticks - Tasks[i].LastRun >= Tasks[i].Interval) {
                    Tasks[i].StartTime = system_ticks; // 记录任务开始时间
                    Tasks[i].LastRun = system_ticks;   // 更新上次运行时间
                    Tasks[i].TaskFunction();           // 执行任务函数
                }
            }
        }

        // 所有任务调度完毕，标记CPU进入空闲状态
        cpu_is_idle = 1; 

        // --- 此处可以处理CPU占用率 ---
        // 例如，每秒通过串口打印一次
        // if (system_ticks % 1000 == 0) {
        //     printf("CPU Usage: %d%%\n", cpu_usage_percent);
        // }
    }
}
💡 关键点说明
超时机制：在每个任务开始执行时记录 StartTime。在主循环中，通过比较当前时间 system_ticks 和 StartTime 来判断任务是否执行过久。一旦超时，就设置 TimeoutFlag 并执行相应的处理函数。
CPU占用率：
在主循环开始时，将 cpu_is_idle 标志设为0（忙碌）。
在主循环所有任务处理完毕后，将 cpu_is_idle 标志设为1（空闲）。
一个高频（如1ms）的定时器中断会不断采样 cpu_is_idle 标志。如果为1，则累加 cpu_idle_time。
每隔一段时间（如1秒），根据公式 (总时间 - 空闲时间) / 总时间 * 100% 计算出CPU占用率。
2ms循环：代码框架中的 while(1) 循环即为主循环。你需要通过硬件定时器或 HAL_Delay(2) 等方式，确保这个循环的执行周期稳定在2ms左右。
中断安全：system_ticks、cpu_total_time 等在中断中修改的全局变量，应使用 volatile 关键字修饰，并在主循环中访问时注意临界区保护（如暂时关闭中断），以防数据不一致。
需要我把SysTick和CPU采样定时器的初始化代码也补全吗？