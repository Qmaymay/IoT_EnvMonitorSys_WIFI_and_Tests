#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "../include/common.h"

// 看门狗初始化
result_code_t watchdog_init(uint32_t timeout_ms);

// 任务看门狗注册（监控各个任务）
typedef struct {
    const char* task_name;
    uint32_t timeout_ms;
    void (*timeout_callback)(void);
} watchdog_task_t;

result_code_t watchdog_register_task(const watchdog_task_t* task);
result_code_t watchdog_feed_task(const char* task_name);
result_code_t watchdog_feed_all(void);

// 硬件看门狗
result_code_t watchdog_hw_feed(void);

#endif // WATCHDOG_H