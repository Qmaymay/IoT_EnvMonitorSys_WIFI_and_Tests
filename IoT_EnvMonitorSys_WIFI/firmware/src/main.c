#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/common.h"
#include "../network/wifi_manager.h"
#include "../network/mqtt_manager.h"
#include "../system/logger.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Function prototypes
result_code_t sensor_read_all(sensor_data_t* data);
result_code_t mqtt_connect(void);
result_code_t mqtt_publish_data(const sensor_data_t* data);
void delay_ms(uint32_t ms);
void system_init(void);
void wifi_status_callback(bool connected, const char* ssid, int8_t rssi);

// 全局配置
#define WIFI_SSID "MyWiFi"
#define WIFI_PASSWORD "password123"
#define MQTT_TOPIC "env_monitor/data"
#define SAMPLE_INTERVAL_MS 2000

// 设备状态枚举（根据你的common.h可能需要调整）
typedef enum {
    STATE_INIT,
    STATE_CONNECTING_WIFI,
    STATE_CONNECTING_MQTT,
    STATE_NORMAL,
    STATE_ERROR
} device_state_t;

// WiFi状态回调函数
void wifi_status_callback(bool connected, const char* ssid, int8_t rssi) {
    if (connected) {
        LOG_I("WIFI_CB", "Connected to %s (RSSI: %d dBm)", ssid, rssi);
    } else {
        LOG_W("WIFI_CB", "WiFi disconnected");
    }
}

// 系统初始化
void system_init(void) {
    // 初始化随机种子
    srand((unsigned int)time(NULL));
    
    // 初始化日志系统
    logger_init(LOG_LEVEL_INFO);
    LOG_I("SYSTEM", "=== IoT Environmental Monitor System ===");
    LOG_I("SYSTEM", "Build: %s %s", __DATE__, __TIME__);
}

// WiFi连接函数（使用新的wifi_manager）
static result_code_t wifi_connect_impl(void) {
    wifi_manager_config_t wifi_config;
    
    // 获取默认配置
    wifi_manager_get_default_config(&wifi_config);
    
    // 设置自定义参数
    strncpy(wifi_config.ssid, WIFI_SSID, sizeof(wifi_config.ssid) - 1);
    strncpy(wifi_config.password, WIFI_PASSWORD, sizeof(wifi_config.password) - 1);
    wifi_config.connect_timeout_ms = 20000;  // 20秒超时
    wifi_config.max_retries = 5;            // 最大重试5次
    wifi_config.auto_reconnect = true;      // 启用自动重连
    
    // 初始化WiFi管理器
    result_code_t ret = wifi_manager_init(&wifi_config);
    if (ret != RESULT_OK) {
        LOG_E("WIFI", "Failed to initialize WiFi manager");
        return ret;
    }
    
    // 设置状态回调
    wifi_manager_set_status_callback(wifi_status_callback);
    
    // 尝试连接WiFi（带重试机制）
    LOG_I("WIFI", "Connecting to %s...", WIFI_SSID);
    return wifi_manager_connect_with_retry(WIFI_SSID, WIFI_PASSWORD, 3, 2000);
}

// Main function
int main(void) {
    sensor_data_t sensor_data;
    device_state_t state = STATE_INIT;
    int wifi_retry_count = 0;
    int mqtt_retry_count = 0;
    const int max_retries = 3;
    
    // 系统初始化
    system_init();
    
    LOG_I("MAIN", "System initialized, starting state machine...");
    
    while (1) {
        // 自动重连检查（如果启用了自动重连）
        wifi_manager_auto_reconnect();
        
        switch (state) {
            case STATE_INIT:
                LOG_I("MAIN", "Entering INIT state");
                state = STATE_CONNECTING_WIFI;
                wifi_retry_count = 0;
                mqtt_retry_count = 0;
                break;
                
            case STATE_CONNECTING_WIFI:
                LOG_I("MAIN", "Entering CONNECTING_WIFI state");
                
                // 检查是否已经连接
                if (wifi_manager_is_connected()) {
                    LOG_I("MAIN", "WiFi already connected");
                    state = STATE_CONNECTING_MQTT;
                    break;
                }
                
                // 尝试连接WiFi
                LOG_I("MAIN", "Attempting WiFi connection...");
                result_code_t wifi_result = wifi_connect_impl();
                
                if (wifi_result == RESULT_OK) {
                    LOG_I("MAIN", "WiFi connected successfully");
                    state = STATE_CONNECTING_MQTT;
                    wifi_retry_count = 0;
                } else {
                    LOG_W("MAIN", "WiFi connection failed (attempt %d/%d)", 
                          wifi_retry_count + 1, max_retries);
                    wifi_retry_count++;
                    
                    if (wifi_retry_count >= max_retries) {
                        LOG_E("MAIN", "Max WiFi retries reached, entering error state");
                        state = STATE_ERROR;
                    } else {
                        delay_ms(2000);  // 等待2秒后重试
                    }
                }
                break;
                
            case STATE_CONNECTING_MQTT:
                LOG_I("MAIN", "Entering CONNECTING_MQTT state");
                
                // 先检查WiFi是否仍然连接
                if (!wifi_manager_is_connected()) {
                    LOG_W("MAIN", "WiFi lost while connecting MQTT");
                    state = STATE_CONNECTING_WIFI;
                    break;
                }
                
                // 尝试连接MQTT（假设mqtt_manager有相应接口）
                LOG_I("MAIN", "Connecting to MQTT broker...");
                if (mqtt_connect() == RESULT_OK) {
                    LOG_I("MAIN", "MQTT connected successfully");
                    state = STATE_NORMAL;
                    mqtt_retry_count = 0;
                } else {
                    LOG_W("MAIN", "MQTT connection failed (attempt %d/%d)", 
                          mqtt_retry_count + 1, max_retries);
                    mqtt_retry_count++;
                    
                    if (mqtt_retry_count >= max_retries) {
                        LOG_E("MAIN", "Max MQTT retries reached");
                        // 不进入错误状态，而是回到WiFi连接
                        state = STATE_CONNECTING_WIFI;
                    } else {
                        delay_ms(1000);  // 等待1秒后重试
                    }
                }
                break;
                
            case STATE_NORMAL:
                // 检查网络连接
                if (!wifi_manager_is_connected()) {
                    LOG_W("MAIN", "WiFi lost in NORMAL state");
                    state = STATE_CONNECTING_WIFI;
                    break;
                }
                
                // 读取传感器数据
                LOG_D("MAIN", "Reading sensor data...");
                if (sensor_read_all(&sensor_data) == RESULT_OK) {
                    // 添加时间戳
                    sensor_data.timestamp = (uint32_t)time(NULL);
                    
                    // 显示数据
                    LOG_I("SENSOR", "Temp: %.1f°C, Humidity: %.1f%%, Air Quality: %.0f",
                         sensor_data.temperature,
                         sensor_data.humidity,
                         sensor_data.air_quality);
                    
                    // 发布到MQTT
                    if (mqtt_publish_data(&sensor_data) != RESULT_OK) {
                        LOG_W("MAIN", "MQTT publish failed");
                        // 发布失败不影响状态，继续下一次采集
                    }
                } else {
                    LOG_E("MAIN", "Sensor read failed");
                    // 传感器读取失败，可以进入错误状态或重试
                }
                
                // 等待下一个采样周期
                delay_ms(SAMPLE_INTERVAL_MS);
                break;
                
            case STATE_ERROR:
                LOG_E("MAIN", "Entering ERROR state");
                LOG_E("MAIN", "WiFi retries: %d, MQTT retries: %d", 
                      wifi_retry_count, mqtt_retry_count);
                
                // 显示当前WiFi状态
                int8_t rssi = 0;
                if (wifi_manager_get_rssi(&rssi) == RESULT_OK) {
                    LOG_I("MAIN", "Current RSSI: %d dBm", rssi);
                }
                
                // 错误恢复：等待5秒后重试
                LOG_I("MAIN", "Will retry in 5 seconds...");
                delay_ms(5000);
                
                // 重置状态，重新开始
                state = STATE_INIT;
                break;
        }
        
        // 系统心跳（可选）
        static uint32_t heartbeat_counter = 0;
        heartbeat_counter++;
        if (heartbeat_counter % 100 == 0) {
            LOG_D("MAIN", "System heartbeat: %lu", heartbeat_counter);
        }
    }
    
    return 0;
}

// Simulated sensor reading
result_code_t sensor_read_all(sensor_data_t* data) {
    if (data == NULL) {
        return RESULT_INVALID_PARAM;
    }
    
    // Simulated sensor data
    static float base_temp = 25.0f;
    static float base_humidity = 60.0f;
    static float base_air = 50.0f;
    
    // 添加随机变化
    data->temperature = base_temp + ((rand() % 100) / 10.0f - 5.0f);
    data->humidity = base_humidity + ((rand() % 100) / 10.0f - 5.0f);
    data->air_quality = base_air + (rand() % 50);
    data->battery_voltage = 3.7f + ((rand() % 20) / 100.0f - 0.1f);  // 模拟电池电压
    data->sequence = rand() % 65535;
    
    // 限定合理范围
    if (data->temperature < -20.0f) data->temperature = -20.0f;
    if (data->temperature > 50.0f) data->temperature = 50.0f;
    if (data->humidity < 0.0f) data->humidity = 0.0f;
    if (data->humidity > 100.0f) data->humidity = 100.0f;
    if (data->air_quality < 0.0f) data->air_quality = 0.0f;
    if (data->battery_voltage < 3.0f) data->battery_voltage = 3.0f;
    if (data->battery_voltage > 4.2f) data->battery_voltage = 4.2f;
    
    return RESULT_OK;
}

// MQTT连接函数（需要根据mqtt_manager.h实现）
result_code_t mqtt_connect(void) {
    // TODO: 根据mqtt_manager.h的接口实现
    // 例如：
    // mqtt_config_t config;
    // strcpy(config.broker, "test.mosquitto.org");
    // config.port = 1883;
    // return mqtt_manager_connect(&config);
    
    LOG_I("MQTT", "Simulating MQTT connection...");
    
    // 模拟90%成功率
    if ((rand() % 100) < 90) {
        return RESULT_OK;
    }
    return RESULT_ERROR;
}

// MQTT发布函数（需要根据mqtt_manager.h实现）
result_code_t mqtt_publish_data(const sensor_data_t* data) {
    if (data == NULL) {
        return RESULT_INVALID_PARAM;
    }
    
    // TODO: 根据mqtt_manager.h的接口实现
    // 例如：
    // return mqtt_manager_publish_data(data);
    
    LOG_D("MQTT", "Simulating MQTT publish...");
    printf("Publishing: Temp=%.1fC, Hum=%.1f%%, AQ=%.0f\n",
           data->temperature, data->humidity, data->air_quality);
    
    // 模拟95%成功率
    if ((rand() % 100) < 95) {
        return RESULT_OK;
    }
    return RESULT_ERROR;
}

// Utility: delay function
void delay_ms(uint32_t ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}