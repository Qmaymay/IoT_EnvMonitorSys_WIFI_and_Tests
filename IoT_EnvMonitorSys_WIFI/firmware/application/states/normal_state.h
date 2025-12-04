#ifndef NORMAL_STATE_H
#define NORMAL_STATE_H

#include "../state_machine.h"

// 正常工作状态接口
result_code_t normal_state_enter(void);
result_code_t normal_state_run(void);
result_code_t normal_state_exit(void);
result_code_t normal_state_handle_event(state_event_t event);

// 正常工作状态特定函数
result_code_t normal_state_collect_data(void);
result_code_t normal_state_report_data(void);
result_code_t normal_state_check_system(void);

#endif // NORMAL_STATE_H