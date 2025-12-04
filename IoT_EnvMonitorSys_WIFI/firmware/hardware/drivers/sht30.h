#ifndef SHT30_H
#define SHT30_H

#include "../../include/common.h"

// SHT30温湿度传感器
#define SHT30_ADDRESS         0x44

typedef struct {
    float temperature;
    float humidity;
    uint8_t crc_ok;
} sht30_data_t;

result_code_t sht30_init(void);
result_code_t sht30_read(sht30_data_t* data);
result_code_t sht30_soft_reset(void);
uint8_t sht30_check_crc(uint16_t data, uint8_t crc);

#endif // SHT30_H