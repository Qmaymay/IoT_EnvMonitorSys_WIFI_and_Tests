#ifndef HAL_I2C_H
#define HAL_I2C_H

#include "../../include/common.h"

// I2C抽象接口
result_code_t hal_i2c_init(uint32_t frequency);
result_code_t hal_i2c_write(uint8_t addr, const uint8_t* data, size_t len);
result_code_t hal_i2c_read(uint8_t addr, uint8_t* buffer, size_t len);
result_code_t hal_i2c_write_read(uint8_t addr, const uint8_t* tx_data, size_t tx_len,
                                 uint8_t* rx_data, size_t rx_len);

#endif // HAL_I2C_H