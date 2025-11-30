#define ENV_MONITOR_DLL_EXPORTS
#include "common.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// 添加缺失的函数实现
uint32_t get_current_time_ms(void) {
    return (uint32_t)(clock() * 1000 / CLOCKS_PER_SEC);
}

void network_delay_ms(uint32_t ms) {
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
}

// 简化的MQTT客户端状态
static bool connected = true;

result_code_t mqtt_client_init(void) {
    printf("[MQTT] Initializing (simplified version)\n");
    return RESULT_OK;
}

result_code_t mqtt_connect(void) {
    printf("[MQTT] Connected (simplified)\n");
    connected = true;
    return RESULT_OK;
}

void mqtt_disconnect(void) {
    printf("[MQTT] Disconnected\n");
    connected = false;
}

bool mqtt_is_connected(void) {
    return connected;
}

result_code_t mqtt_publish_sensor_data(const sensor_data_t* data) {
    if (!connected) return RESULT_MQTT_ERROR;

    // 生成JSON
    char payload[512];
    snprintf(payload, sizeof(payload),
        "{\"device_id\":\"%s\",\"temp\":%.2f,\"hum\":%.2f,\"air\":%.2f,\"ts\":%lu}",
        DEVICE_ID, data->temperature, data->humidity, data->air_quality, data->timestamp);
    
    printf("[MQTT] 生成的JSON: %s\n", payload);
    

    char command[1024];
    snprintf(command, sizeof(command), 
             "cd /d \"C:\\Program Files\\mosquitto\" && mosquitto_pub.exe -t \"%s\" -m \"%s\"", 
             MQTT_TOPIC_DATA, payload);
    
    printf("[MQTT] 执行命令: %s\n", command);
    int result = system(command);
    
    
    printf("[MQTT] 结果: %d\n", result);
    return (result == 0) ? RESULT_OK : RESULT_ERROR;
}

void mqtt_process(void) {
    // 空实现
}

const char* mqtt_get_last_error(void) {
    return "No error";
}

result_code_t mqtt_publish_status(const device_status_t* status) {
    printf("[MQTT] Status publishing not implemented\n");
    return RESULT_OK;
}
