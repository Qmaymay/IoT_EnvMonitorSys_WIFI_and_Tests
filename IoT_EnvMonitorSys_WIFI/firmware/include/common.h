#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// 通用错误码
typedef enum {
    RESULT_OK = 0,
    RESULT_ERROR = -1,
    RESULT_INVALID_PARAM = -2,
    RESULT_TIMEOUT = -3,
    RESULT_NO_MEMORY = -4,
    RESULT_NOT_SUPPORTED = -5
} result_code_t;

// 传感器数据结构
typedef struct {
    float temperature;      // 摄氏度
    float humidity;         // 百分比
    float air_quality;      // 空气质量指数
    float battery_voltage;  // 电池电压
    uint32_t timestamp;     // Unix时间戳
    uint16_t sequence;      // 数据序列号
} sensor_data_t;

// 设备状态机状态（工作模式）
// typedef enum {
//     DEVICE_STATE_INIT,
//     DEVICE_STATE_CONNECTING_WIFI,
//     DEVICE_STATE_CONNECTING_MQTT,
//     DEVICE_STATE_NORMAL,
//     DEVICE_STATE_CONFIG,
//     DEVICE_STATE_ERROR,
//     DEVICE_STATE_OTA,
//     DEVICE_STATE_SLEEP
// } device_state_t;
typedef enum {
    STATE_INIT,
    STATE_CONNECTING_WIFI,
    STATE_CONNECTING_MQTT,
    STATE_NORMAL,
    STATE_ERROR
} device_state_t;

// 设备状态（设备信息）
typedef struct {
    char device_id[32];
    uint32_t firmware_version;
    uint32_t uptime_ms;
    uint32_t data_count;
    int8_t wifi_rssi;       // WiFi信号强度
    bool mqtt_connected;
    uint8_t battery_level;  // 电池电量百分比
} device_status_t;

// 设备配置
typedef struct {
    char device_id[32];
    uint32_t sample_interval_ms;   // 采样间隔
    uint32_t report_interval_ms;   // 上报间隔
    float temperature_threshold;   // 温度阈值
    float humidity_threshold;      // 湿度阈值
    char mqtt_broker[64];
    uint16_t mqtt_port;
} device_config_t;

#endif // COMMON_H