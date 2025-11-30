#include "common.h"
#include "config.h"
#include "sensor_emulator.h"
#include "mqtt_client.h"
#include <stdio.h>
#include <string.h>
#include <time.h>



static void print_device_info(void) {
    printf("=== IoT Environment Monitor ===\n");
    printf("Device ID: %s\n", DEVICE_ID);
    printf("Firmware: %s\n", FIRMWARE_VERSION);
    printf("Sample Interval: %d ms\n", SENSOR_READ_INTERVAL_MS);
    printf("MQTT Broker: %s:%d\n", MQTT_BROKER, MQTT_PORT);
    printf("===============================\n\n");
}

int main(void) {
    print_device_info();
    
    // 初始化组件
    if (sensor_emulator_init() != RESULT_OK) {
        printf("Failed to initialize sensor emulator\n");
        return -1;
    }
    
    if (mqtt_client_init() != RESULT_OK) {
        printf("Failed to initialize MQTT client\n");
        return -1;
    }
    
    // 连接MQTT
    if (mqtt_connect() != RESULT_OK) {
        printf("Warning: MQTT connection failed, continuing in offline mode\n");
    }
    
    printf("Starting main loop...\n");
    
    uint32_t last_sample_time = 0;
    uint16_t sequence_number = 0;
    
    // 主循环
    while (1) {
        uint32_t current_time = get_current_time_ms();
        
        // 处理MQTT后台任务
        mqtt_process();
        
        // 定期采样和发送数据
        if ((current_time - last_sample_time) >= SENSOR_READ_INTERVAL_MS) {
            sensor_data_t sensor_data;
            
            // 读取传感器数据
            if (sensor_emulator_read(&sensor_data) == RESULT_OK) {
                sensor_data.timestamp = get_timestamp();
                sensor_data.sequence = sequence_number++;

                printf("=== BUILD VERSION: NEW VERSION ===\n");
                
                // printf("[SENSOR] T: %.2f°C, H: %.2f%%, AQ: %.2f\n",
                //        sensor_data.temperature, sensor_data.humidity, 
                //        sensor_data.air_quality);
                
                // 发布到MQTT
                if (mqtt_is_connected()) {
                    if (mqtt_publish_sensor_data(&sensor_data) != RESULT_OK) {
                        printf("Failed to publish sensor data: %s\n", 
                               mqtt_get_last_error());
                    }
                } else {
                    printf("[MQTT] Offline - data not sent\n");
                    
                    // 尝试重新连接
                    if (mqtt_connect() == RESULT_OK) {
                        printf("[MQTT] Reconnected successfully\n");
                    }
                }
            }
            
            last_sample_time = current_time;
        }
        
        // 在实际硬件上可能需要延迟以节省功耗
        network_delay_ms(100);
    }
    
    // 清理资源
    mqtt_disconnect();
    
    return 0;
}