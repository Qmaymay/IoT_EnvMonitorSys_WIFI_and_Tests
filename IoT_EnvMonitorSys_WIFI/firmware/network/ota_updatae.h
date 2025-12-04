#ifndef OTA_UPDADER_H
#define OTA_UPDADER_H

#include "../include/common.h"

// OTA状态
typedef enum {
    OTA_STATE_IDLE,
    OTA_STATE_CHECKING,
    OTA_STATE_DOWNLOADING,
    OTA_STATE_VERIFYING,
    OTA_STATE_SUCCESS,
    OTA_STATE_FAILED
} ota_state_t;

// OTA配置
typedef struct {
    char server_url[128];
    char version_check_url[128];
    uint32_t check_interval_ms;
} ota_config_t;

result_code_t ota_updater_init(const ota_config_t* config);
result_code_t ota_updater_check(void);
result_code_t ota_updater_start(const char* firmware_url);
ota_state_t ota_updater_get_state(void);
result_code_t ota_updater_get_progress(uint8_t* percent);

#endif // OTA_UPDADER_H