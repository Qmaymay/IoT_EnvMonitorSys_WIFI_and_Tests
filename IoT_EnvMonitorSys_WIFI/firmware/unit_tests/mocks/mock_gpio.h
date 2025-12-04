#ifndef MOCK_GPIO_H
#define MOCK_GPIO_H

#include <stdint.h>
#include <stdbool.h>

// GPIO模拟接口
void mock_gpio_init(void);
void mock_gpio_set_pin_state(uint8_t pin, bool state);
bool mock_gpio_get_pin_state(uint8_t pin);
void mock_gpio_reset_all(void);

// 期望值设置
void mock_gpio_expect_write(uint8_t pin, bool expected_value);
void mock_gpio_expect_read(uint8_t pin, bool return_value);

#endif // MOCK_GPIO_H