#include "mqtt_client.h"
#include "sensor_emulator.h"
#include <stdio.h>
#include <unistd.h>

void test_mqtt_client(void) {
    printf("=== Testing MQTT Client ===\n");
    
    // 初始化
    if (mqtt_client_init() != RESULT_OK) {
        printf("FAIL: Client initialization\n");
        return;
    }
    printf("PASS: Client initialization\n");
    
    // 测试连接
    if (mqtt_connect() != RESULT_OK) {
        printf("FAIL: MQTT connection\n");
        return;
    }
    printf("PASS: MQTT connection\n");
    
    // 测试发布传感器数据
    sensor_data_t test_data = {
        .temperature = 23.5f,
        .humidity = 65.2f,
        .air_quality = 45.1f,
        .timestamp = 1234567890,
        .sequence = 1
    };
    
    if (mqtt_publish_sensor_data(&test_data) != RESULT_OK) {
        printf("FAIL: Sensor data publish\n");
    } else {
        printf("PASS: Sensor data publish\n");
    }
    
    // 测试状态发布
    device_status_t status = {
        .uptime_ms = 60000,
        .data_count = 10,
        .mqtt_connected = true,
        .wifi_strength = 85
    };
    strcpy(status.device_id, "test_device");
    strcpy(status.firmware_version, "1.0.0");
    
    if (mqtt_publish_status(&status) != RESULT_OK) {
        printf("FAIL: Status publish\n");
    } else {
        printf("PASS: Status publish\n");
    }
    
    // 处理一些MQTT任务
    for (int i = 0; i < 5; i++) {
        mqtt_process();
        sleep(1);
    }
    
    // 断开连接
    mqtt_disconnect();
    printf("PASS: MQTT disconnection\n");
    
    printf("=== MQTT Test Complete ===\n");
}

int main(void) {
    test_mqtt_client();
    return 0;
}
