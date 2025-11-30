#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "common.h"
#include "config.h"
#include "env_monitor_export.h"

#ifdef __cplusplus
extern "C" {
#endif

// MQTT客户端初始化
ENV_MONITOR_API result_code_t mqtt_client_init(void);

// MQTT连接管理
ENV_MONITOR_API result_code_t mqtt_connect(void);
ENV_MONITOR_API void mqtt_disconnect(void);
ENV_MONITOR_API bool mqtt_is_connected(void);
ENV_MONITOR_API mqtt_state_t mqtt_get_state(void);

// 数据发布
// 直接接收传感器数据结构，内部处理JSON转换
ENV_MONITOR_API result_code_t mqtt_publish_sensor_data(const sensor_data_t* data);
ENV_MONITOR_API result_code_t mqtt_publish_status(const device_status_t* status);

// 周期处理（需要在主循环中调用）
// 作用：需要在主循环中定期调用，处理后台通信任务
ENV_MONITOR_API void mqtt_process(void);

// 获取最后错误信息
ENV_MONITOR_API const char* mqtt_get_last_error(void);

#ifdef __cplusplus
}
#endif

#endif // MQTT_CLIENT_H