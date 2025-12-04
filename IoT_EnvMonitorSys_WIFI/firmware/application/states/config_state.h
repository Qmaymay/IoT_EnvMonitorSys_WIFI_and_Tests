#ifndef CONFIG_STATE_H
#define CONFIG_STATE_H

#include "../state_machine.h"

// 配置状态接口
result_code_t config_state_enter(void);
result_code_t config_state_run(void);
result_code_t config_state_exit(void);
result_code_t config_state_handle_event(state_event_t event);

// 配置状态特定函数
result_code_t config_state_start_wifi_config(void);
result_code_t config_state_process_command(const char* command);

#endif // CONFIG_STATE_H