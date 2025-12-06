#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "../include/common.h"

// WiFi管理器配置
typedef struct {
    char ssid[32];           // 可考虑修改为明确用途的命名target_ssid[32]
    char password[64];
    uint32_t connect_timeout_ms;    // 连接超时时间
    uint8_t max_retries;            // 最大重试次数
    bool auto_reconnect;            // 是否自动重连
    uint32_t reconnect_interval_ms; // 重连间隔
    bool enable_scan;               // 是否启用网络扫描
} wifi_manager_config_t;

// WiFi网络信息
typedef struct {
    char ssid[32];    // 可考虑修改为明确用途的命名scanned_ssid[32]
    int8_t rssi;          // 信号强度
    uint8_t channel;      // 信道
    uint8_t authmode;     // 认证模式
} wifi_network_info_t;

// WiFi连接状态回调
typedef void (*wifi_status_callback_t)(bool connected, const char* ssid, int8_t rssi);

// WiFi管理器接口
result_code_t wifi_manager_init(const wifi_manager_config_t* config);
result_code_t wifi_manager_connect(const char* ssid, const char* password);
result_code_t wifi_manager_disconnect(void);
bool wifi_manager_is_connected(void);
result_code_t wifi_manager_get_rssi(int8_t* rssi);
result_code_t wifi_manager_start_ap_mode(const char* ap_ssid, const char* ap_password);
result_code_t wifi_manager_scan(wifi_network_info_t* networks, uint8_t* count, uint8_t max_count);

// 高级功能
result_code_t wifi_manager_connect_with_retry(const char* ssid, const char* password,
                                             uint8_t max_retries, uint32_t retry_delay_ms);
result_code_t wifi_manager_auto_reconnect(void);
result_code_t wifi_manager_set_status_callback(wifi_status_callback_t callback);

// 获取默认配置
result_code_t wifi_manager_get_default_config(wifi_manager_config_t* config);

#endif // WIFI_MANAGER_H