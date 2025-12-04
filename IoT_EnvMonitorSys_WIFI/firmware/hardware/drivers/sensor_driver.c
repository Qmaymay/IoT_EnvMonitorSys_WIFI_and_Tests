#include "sensor_driver.h"
#include <stdio.h>
#include <stdlib.h>

result_code_t sensor_driver_init(void) {
    printf("[Sensor] Initialize all sensors\n");
    
    // Initialize each sensor
    if (sht30_init() != RESULT_OK) {
        printf("[Sensor] SHT30 initialization failed\n");
        return RESULT_ERROR;
    }
    
    if (mq135_init() != RESULT_OK) {
        printf("[Sensor] MQ135 initialization failed\n");
        return RESULT_ERROR;
    }
    
    return RESULT_OK;
}

result_code_t sensor_driver_read_all(sensor_package_t* data) {
    sht30_data_t sht30_data;
    uint16_t mq135_raw;
    
    // Read temperature and humidity
    if (sht30_read(&sht30_data) != RESULT_OK) {
        return RESULT_ERROR;
    }
    
    // Read air quality
    if (mq135_read_raw(&mq135_raw) != RESULT_OK) {
        return RESULT_ERROR;
    }
    
    // Fill data
    data->sht30 = sht30_data;
    data->mq135_raw = mq135_raw;
    data->mq135_ppm = mq135_calculate_ppm(mq135_raw);
    data->battery_voltage = 3.7f + ((rand() % 100) / 1000.0f - 0.05f);
    
    return RESULT_OK;
}

result_code_t sensor_driver_calibrate(void) {
    printf("[Sensor] Start calibrating all sensors\n");
    mq135_calibrate();
    return RESULT_OK;
}

bool sensor_driver_check_health(void) {
    // Simple health check
    return true;
}