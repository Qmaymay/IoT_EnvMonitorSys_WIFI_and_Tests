#include "esp32_wifi.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static wifi_state_t wifi_state = WIFI_DISCONNECTED;
static wifi_config_t wifi_config;

result_code_t esp32_wifi_init(void) {
    printf("[WiFi] 初始化ESP32 WiFi\n");
    wifi_state = WIFI_DISCONNECTED;
    return RESULT_OK;
}

result_code_t esp32_wifi_connect(const wifi_config_t* config) {
    printf("[WiFi] 尝试连接到: %s\n", config->ssid);
    
    memcpy(&wifi_config, config, sizeof(wifi_config_t));
    wifi_state = WIFI_CONNECTING;
    
    // 模拟连接过程
    printf("[WiFi] 正在连接...\n");
    
    // 模拟80%的成功率
    if ((rand() % 100) < 80) {
        wifi_state = WIFI_CONNECTED;
        printf("[WiFi] 连接成功\n");
        return RESULT_OK;
    } else {
        wifi_state = WIFI_DISCONNECTED;
        printf("[WiFi] 连接失败\n");
        return RESULT_ERROR;
    }
}

result_code_t esp32_wifi_disconnect(void) {
    printf("[WiFi] 断开连接\n");
    wifi_state = WIFI_DISCONNECTED;
    return RESULT_OK;
}

wifi_state_t esp32_wifi_get_state(void) {
    return wifi_state;
}

result_code_t esp32_wifi_get_rssi(int8_t* rssi) {
    if (wifi_state != WIFI_CONNECTED) {
        return RESULT_ERROR;
    }
    
    // 模拟信号强度
    *rssi = -40 - (rand() % 40);
    return RESULT_OK;
}

result_code_t esp32_wifi_start_config(void) {
    printf("[WiFi] 启动配网模式\n");
    wifi_state = WIFI_CONFIGURING;
    return RESULT_OK;
}