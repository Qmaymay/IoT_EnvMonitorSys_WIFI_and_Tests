#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "../include/common.h"

// MQTT配置
typedef struct {
    char broker[64];
    uint16_t port;
    char client_id[32];
    char username[32];
    char password[32];
} mqtt_config_t;

// MQTT主题
typedef struct {
    char data_topic[64];
    char status_topic[64];
    char control_topic[64];
    char ota_topic[64];
} mqtt_topics_t;

result_code_t mqtt_manager_init(const mqtt_config_t* config, const mqtt_topics_t* topics);
result_code_t mqtt_manager_connect(void);
result_code_t mqtt_manager_disconnect(void);
bool mqtt_manager_is_connected(void);
result_code_t mqtt_manager_publish_data(const sensor_data_t* data);
result_code_t mqtt_manager_publish_status(const device_status_t* status);
result_code_t mqtt_manager_subscribe_control(void);
result_code_t mqtt_manager_process(void);  // 处理接收到的消息

#endif // MQTT_MANAGER_H