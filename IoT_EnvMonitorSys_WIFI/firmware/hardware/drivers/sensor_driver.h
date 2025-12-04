#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include "../include/common.h"
#include "sht30.h"
#include "mq135.h"


// 传感器管理器
typedef struct {
    sht30_data_t sht30;
    uint16_t mq135_raw;
    float mq135_ppm;
    float battery_voltage;
} sensor_package_t;

result_code_t sensor_driver_init(void);
result_code_t sensor_driver_read_all(sensor_package_t* data);
result_code_t sensor_driver_calibrate(void);
bool sensor_driver_check_health(void);

#endif // SENSOR_DRIVER_H