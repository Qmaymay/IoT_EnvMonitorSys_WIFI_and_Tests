#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "../include/config.h"
#include "../include/app_config.h"

// 系统默认配置
#define SYSTEM_TICK_MS          10
#define SYSTEM_HEAP_WARNING     10240  // 10KB剩余内存警告
#define SYSTEM_STACK_CHECK      true

// 系统工作模式
typedef enum {
    SYSTEM_MODE_NORMAL,
    SYSTEM_MODE_CONFIG,
    SYSTEM_MODE_OTA,
    SYSTEM_MODE_FACTORY_TEST
} system_mode_t;

// 系统状态
typedef struct {
    system_mode_t mode;
    uint32_t free_heap;
    uint32_t uptime_seconds;
    uint8_t restart_count;
    uint32_t last_error;
} system_status_t;

#endif // SYSTEM_CONFIG_H