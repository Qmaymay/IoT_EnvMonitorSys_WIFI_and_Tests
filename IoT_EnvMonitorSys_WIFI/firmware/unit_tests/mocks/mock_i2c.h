#ifndef MOCK_I2C_H
#define MOCK_I2C_H

#include <stdint.h>
#include <stddef.h>

// I2C模拟接口
void mock_i2c_init(void);
void mock_i2c_set_device_response(uint8_t addr, const uint8_t* response, size_t len);
void mock_i2c_reset(void);

// 期望值验证
void mock_i2c_expect_write(uint8_t addr, const uint8_t* expected_data, size_t expected_len);
void mock_i2c_expect_read(uint8_t addr, size_t expected_len);

#endif // MOCK_I2C_H