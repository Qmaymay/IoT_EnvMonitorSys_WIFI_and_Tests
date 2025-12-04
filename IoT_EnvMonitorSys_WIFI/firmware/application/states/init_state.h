
#ifndef INIT_STATE_H
#define INIT_STATE_H

#include "../state_machine.h"

// 初始化状态接口
result_code_t init_state_enter(void);
result_code_t init_state_run(void);
result_code_t init_state_exit(void);
result_code_t init_state_handle_event(state_event_t event);

#endif // INIT_STATE_H