#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../include/common.h"

// 设备状态枚举
typedef enum {
    DEVICE_STATE_INIT,        // 初始化
    DEVICE_STATE_NORMAL,      // 正常工作
    DEVICE_STATE_CONFIG,      // 配置模式
    DEVICE_STATE_ERROR,       // 错误处理
    DEVICE_STATE_OTA,         // OTA升级
    DEVICE_STATE_SLEEP        // 低功耗睡眠
} device_state_t;

// 状态机事件
typedef enum {
    EVENT_POWER_ON,
    EVENT_INIT_COMPLETE,
    EVENT_CONFIG_START,
    EVENT_CONFIG_COMPLETE,
    EVENT_ERROR_OCCURRED,
    EVENT_ERROR_RESOLVED,
    EVENT_OTA_START,
    EVENT_OTA_COMPLETE,
    EVENT_ENTER_SLEEP,
    EVENT_WAKE_UP
} state_event_t;

// 状态机接口
result_code_t state_machine_init(void);
result_code_t state_machine_run(void);
result_code_t state_machine_handle_event(state_event_t event);
device_state_t state_machine_get_current_state(void);
result_code_t state_machine_transition(device_state_t new_state);

#endif // STATE_MACHINE_H