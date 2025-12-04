#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "../include/common.h"

// 配置管理器
result_code_t config_manager_init(void);
result_code_t config_manager_load(device_config_t* config);
result_code_t config_manager_save(const device_config_t* config);
result_code_t config_manager_reset(void);
result_code_t config_manager_set_default(device_config_t* config);
result_code_t config_manager_update_field(const char* key, const char* value);

#endif // CONFIG_MANAGER_H