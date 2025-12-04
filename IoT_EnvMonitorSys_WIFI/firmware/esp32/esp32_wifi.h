#ifndef ESP32_WIFI_H
#define ESP32_WIFI_H

#include "../include/common.h"

// WiFi连接状态
typedef enum {
    WIFI_DISCONNECTED,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_CONFIGURING
} wifi_state_t;

// WiFi配置
typedef struct {
    char ssid[32];
    char password[64];
} wifi_config_t;

result_code_t esp32_wifi_init(void);
result_code_t esp32_wifi_connect(const wifi_config_t* config);
result_code_t esp32_wifi_disconnect(void);
wifi_state_t esp32_wifi_get_state(void);
result_code_t esp32_wifi_get_rssi(int8_t* rssi);
result_code_t esp32_wifi_start_config(void);  // 启动配网模式

#endif // ESP32_WIFI_H