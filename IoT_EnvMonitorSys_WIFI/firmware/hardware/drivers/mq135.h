#ifndef MQ135_H
#define MQ135_H

#include "../../include/common.h"

// MQ135空气质量传感器
result_code_t mq135_init(void);
result_code_t mq135_read_raw(uint16_t* raw_value);
result_code_t mq135_calibrate(void);
float mq135_calculate_ppm(uint16_t raw_value);

#endif // MQ135_H