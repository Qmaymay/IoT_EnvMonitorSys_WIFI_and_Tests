#ifndef ERROR_STATE_H
#define ERROR_STATE_H

#include "../state_machine.h"

// 错误处理状态接口
result_code_t error_state_enter(void);
result_code_t error_state_run(void);
result_code_t error_state_exit(void);
result_code_t error_state_handle_event(state_event_t event);

// 错误处理函数
result_code_t error_state_diagnose(void);
result_code_t error_state_recover(void);
result_code_t error_state_report_error(void);

#endif // ERROR_STATE_H