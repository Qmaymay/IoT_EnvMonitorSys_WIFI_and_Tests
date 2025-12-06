#include "wifi_manager.h"
#include "../esp32/esp32_wifi.h"
#include "../system/logger.h"
#include <string.h>

// 内部状态
typedef struct {
    wifi_manager_config_t config;
    bool initialized;
    bool connected;
    char current_ssid[32];
    int8_t current_rssi;
    wifi_status_callback_t status_callback;
    uint32_t connect_start_time;
    uint8_t retry_count;
} wifi_manager_state_t;

static wifi_manager_state_t g_state = {0};

// 默认配置
static const wifi_manager_config_t DEFAULT_CONFIG = {
    .ssid = "",
    .password = "",
    .connect_timeout_ms = 30000,
    .max_retries = 3,
    .auto_reconnect = true,
    .reconnect_interval_ms = 5000,
    .enable_scan = true
};

// 内部函数声明
static void wifi_manager_notify_status(bool connected, const char* ssid, int8_t rssi);
static result_code_t wifi_manager_internal_connect(const char* ssid, const char* password);

result_code_t wifi_manager_init(const wifi_manager_config_t* config) {
    if (g_state.initialized) {
        LOG_W("WIFI", "WiFi manager already initialized");
        return RESULT_OK;
    }
    
    // 初始化ESP32 WiFi底层
    result_code_t ret = esp32_wifi_init();
    if (ret != RESULT_OK) {
        LOG_E("WIFI", "Failed to initialize ESP32 WiFi");
        return ret;
    }
    
    // 保存配置
    if (config != NULL) {
        memcpy(&g_state.config, config, sizeof(wifi_manager_config_t));
    } else {
        memcpy(&g_state.config, &DEFAULT_CONFIG, sizeof(wifi_manager_config_t));
    }
    
    g_state.initialized = true;
    g_state.connected = false;
    g_state.status_callback = NULL;
    g_state.retry_count = 0;
    
    LOG_I("WIFI", "WiFi manager initialized successfully");
    LOG_D("WIFI", "Config: timeout=%dms, max_retries=%d, auto_reconnect=%s",
          g_state.config.connect_timeout_ms,
          g_state.config.max_retries,
          g_state.config.auto_reconnect ? "true" : "false");
    
    return RESULT_OK;
}

result_code_t wifi_manager_connect(const char* ssid, const char* password) {
    if (!g_state.initialized) {
        LOG_E("WIFI", "WiFi manager not initialized");
        return RESULT_ERROR;
    }
    
    if (ssid == NULL || strlen(ssid) == 0) {
        LOG_E("WIFI", "Invalid SSID");
        return RESULT_INVALID_PARAM;
    }
    
    LOG_I("WIFI", "Connecting to WiFi: %s", ssid);
    
    result_code_t ret = wifi_manager_internal_connect(ssid, password);
    
    if (ret == RESULT_OK) {
        strncpy(g_state.current_ssid, ssid, sizeof(g_state.current_ssid) - 1);
        g_state.current_ssid[sizeof(g_state.current_ssid) - 1] = '\0';
        g_state.connected = true;
        g_state.retry_count = 0;
        wifi_manager_notify_status(true, ssid, g_state.current_rssi);
    } else {
        g_state.connected = false;
        wifi_manager_notify_status(false, ssid, 0);
        
        // 如果启用了自动重连且未达到最大重试次数
        if (g_state.config.auto_reconnect && 
            g_state.retry_count < g_state.config.max_retries) {
            g_state.retry_count++;
            LOG_W("WIFI", "Connection failed, will retry %d/%d", 
                  g_state.retry_count, g_state.config.max_retries);
        }
    }
    
    return ret;
}

static result_code_t wifi_manager_internal_connect(const char* ssid, const char* password) {
    wifi_config_t config;
    memset(&config, 0, sizeof(config));
    
    strncpy(config.ssid, ssid, sizeof(config.ssid) - 1);
    config.ssid[sizeof(config.ssid) - 1] = '\0';
    
    if (password != NULL) {
        strncpy(config.password, password, sizeof(config.password) - 1);
        config.password[sizeof(config.password) - 1] = '\0';
    }
    
    // 调用底层ESP32 WiFi连接
    result_code_t ret = esp32_wifi_connect(&config);
    
    if (ret == RESULT_OK) {
        // 获取信号强度
        esp32_wifi_get_rssi(&g_state.current_rssi);
        LOG_I("WIFI", "Connected to %s, RSSI: %d dBm", ssid, g_state.current_rssi);
    }
    
    return ret;
}

result_code_t wifi_manager_disconnect(void) {
    if (!g_state.initialized) {
        return RESULT_ERROR;
    }
    
    LOG_I("WIFI", "Disconnecting from WiFi");
    
    result_code_t ret = esp32_wifi_disconnect();
    if (ret == RESULT_OK) {
        g_state.connected = false;
        g_state.current_ssid[0] = '\0';
        g_state.current_rssi = 0;
        wifi_manager_notify_status(false, NULL, 0);
    }
    
    return ret;
}

bool wifi_manager_is_connected(void) {
    return g_state.connected;
}

result_code_t wifi_manager_get_rssi(int8_t* rssi) {
    if (!g_state.connected || rssi == NULL) {
        return RESULT_INVALID_PARAM;
    }
    
    *rssi = g_state.current_rssi;
    return RESULT_OK;
}

result_code_t wifi_manager_scan(wifi_network_info_t* networks, uint8_t* count, uint8_t max_count) {
    if (!g_state.initialized || networks == NULL || count == NULL) {
        return RESULT_INVALID_PARAM;
    }
    
    if (!g_state.config.enable_scan) {
        LOG_W("WIFI", "WiFi scanning is disabled in config");
        return RESULT_NOT_SUPPORTED;
    }
    
    LOG_I("WIFI", "Starting WiFi scan...");
    
    // 这里应该调用底层扫描功能，当前模拟实现
    *count = 0;
    
    // 模拟扫描到几个网络
    if (max_count >= 3) {
        // 网络1
        strncpy(networks[0].ssid, "Home_WiFi_5G", 32);
        networks[0].rssi = -45;
        networks[0].channel = 36;
        networks[0].authmode = 3;  // WPA2
        (*count)++;
        
        // 网络2
        strncpy(networks[1].ssid, "TP-Link_2.4G", 32);
        networks[1].rssi = -62;
        networks[1].channel = 6;
        networks[1].authmode = 3;  // WPA2
        (*count)++;
        
        // 网络3
        strncpy(networks[2].ssid, "Guest_WiFi", 32);
        networks[2].rssi = -75;
        networks[2].channel = 11;
        networks[2].authmode = 0;  // OPEN
        (*count)++;
    }
    
    LOG_I("WIFI", "WiFi scan completed, found %d networks", *count);
    return RESULT_OK;
}

result_code_t wifi_manager_connect_with_retry(const char* ssid, const char* password,
                                             uint8_t max_retries, uint32_t retry_delay_ms) {
    if (!g_state.initialized) {
        return RESULT_ERROR;
    }
    
    LOG_I("WIFI", "Connecting with retry: SSID=%s, max_retries=%d", 
          ssid, max_retries);
    
    for (uint8_t i = 0; i < max_retries; i++) {
        LOG_D("WIFI", "Connection attempt %d/%d", i + 1, max_retries);
        
        result_code_t ret = wifi_manager_connect(ssid, password);
        if (ret == RESULT_OK) {
            LOG_I("WIFI", "Connected successfully on attempt %d", i + 1);
            return RESULT_OK;
        }
        
        if (i < max_retries - 1) {
            LOG_W("WIFI", "Connection failed, retrying in %d ms...", retry_delay_ms);
            // 实际项目中这里应该用非阻塞延时
            // delay_ms(retry_delay_ms);
        }
    }
    
    LOG_E("WIFI", "All connection attempts failed");
    return RESULT_ERROR;
}

result_code_t wifi_manager_set_status_callback(wifi_status_callback_t callback) {
    if (!g_state.initialized) {
        return RESULT_ERROR;
    }
    
    g_state.status_callback = callback;
    LOG_D("WIFI", "WiFi status callback set");
    return RESULT_OK;
}

result_code_t wifi_manager_get_default_config(wifi_manager_config_t* config) {
    if (config == NULL) {
        return RESULT_INVALID_PARAM;
    }
    
    memcpy(config, &DEFAULT_CONFIG, sizeof(wifi_manager_config_t));
    return RESULT_OK;
}

static void wifi_manager_notify_status(bool connected, const char* ssid, int8_t rssi) {
    if (g_state.status_callback != NULL) {
        g_state.status_callback(connected, ssid, rssi);
    }
}

// 定时调用的自动重连函数（在main.c的循环中调用）
result_code_t wifi_manager_auto_reconnect(void) {
    if (!g_state.initialized || !g_state.config.auto_reconnect) {
        return RESULT_OK;
    }
    
    if (!g_state.connected) {
        // 如果当前没有连接且有保存的SSID
        if (g_state.current_ssid[0] != '\0') {
            if (g_state.retry_count < g_state.config.max_retries) {
                LOG_D("WIFI", "Auto reconnecting to %s...", g_state.current_ssid);
                return wifi_manager_connect(g_state.current_ssid, "");
            }
        }
    }
    
    return RESULT_OK;
}