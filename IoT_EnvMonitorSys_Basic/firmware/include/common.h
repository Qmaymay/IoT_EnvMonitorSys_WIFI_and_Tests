#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include "env_monitor_export.h"

// common.h 定义数据结构

// 传感器数据结构 - 所有模块共享的通用数据格式
// 设计目的：确保传感器数据在采集、传输、处理过程中格式统一。
typedef struct {
    float temperature;      // 温度 (°C)
    float humidity;         // 湿度 (%)
    float air_quality;      // 空气质量指数
    uint32_t timestamp;     // 时间戳
    uint16_t sequence;      // 序列号，用于检测数据丢失
} sensor_data_t;

\
// 设备状态结构 - 面向对象思维的结构化设计
/*
技术特点：
1, 固定大小数组：避免动态内存分配，适合嵌入式系统
2, 布尔状态标志：明确的状态表示
3, 时间戳管理：统一的时间基准
*/
typedef struct {
    char device_id[32];
    char firmware_version[16];
    uint32_t uptime_ms;
    uint32_t data_count;
    uint8_t wifi_strength;  // WiFi信号强度 (0-100)
    bool mqtt_connected;
} device_status_t;


// MQTT客户端状态机
/*
技术优势：
1, 清晰的连接生命周期管理
2, 便于状态监控和故障诊断
3, 支持异步连接过程
*/
typedef enum {
    MQTT_STATE_DISCONNECTED,  // 断开状态
    MQTT_STATE_CONNECTING,    // 连接中
    MQTT_STATE_CONNECTED,     // 已连接
    MQTT_STATE_ERROR          // 错误状态
} mqtt_state_t;


// 所有模块使用相同的返回码，确保错误处理一致性
// 设计目的：消除各模块间返回值的歧义，提供标准化的错误处理机制。
typedef enum {
    RESULT_OK = 0,
    RESULT_ERROR = -1,
    RESULT_INVALID_PARAM = -2,
    RESULT_NETWORK_ERROR = -3,
    RESULT_MQTT_ERROR = -4
} result_code_t;


// 使用宏控制函数可见性
/*
技术实现：
Windows: __declspec(dllexport/dllimport)
Linux: 空定义，使用默认可见性
*/

ENV_MONITOR_API uint32_t get_current_time_ms(void);
ENV_MONITOR_API void network_delay_ms(uint32_t ms);
ENV_MONITOR_API uint32_t get_timestamp(void);


#endif // COMMON_H