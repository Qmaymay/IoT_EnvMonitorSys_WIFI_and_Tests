#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include "../include/common.h"

// 数据处理配置
typedef struct {
    bool enable_filter;
    uint8_t filter_window;
    bool enable_calibration;
    uint8_t calibration_count;
} data_processor_config_t;

// 数据处理器
result_code_t data_processor_init(const data_processor_config_t* config);
result_code_t data_processor_filter(sensor_data_t* data);
result_code_t data_processor_calibrate(const sensor_data_t* raw_data);
result_code_t data_processor_check_alarm(const sensor_data_t* data, bool* alarm_flags);
result_code_t data_processor_aggregate(sensor_data_t* hourly_data, const sensor_data_t* raw_data);

// 数据验证
bool data_processor_validate(const sensor_data_t* data);

#endif // DATA_PROCESSOR_H