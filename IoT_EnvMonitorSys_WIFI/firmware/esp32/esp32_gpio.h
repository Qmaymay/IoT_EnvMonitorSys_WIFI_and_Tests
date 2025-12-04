#ifndef ESP32_GPIO_H
#define ESP32_GPIO_H

#include "../include/common.h"
#include "../hardware/hal/hal_gpio.h"

// ESP32 GPIO实现
result_code_t esp32_gpio_init(void);
result_code_t esp32_gpio_config(uint8_t pin, gpio_direction_t dir, gpio_pull_t pull);
result_code_t esp32_gpio_write(uint8_t pin, bool level);
bool esp32_gpio_read(uint8_t pin);

// ESP32特定功能
result_code_t esp32_gpio_set_interrupt(uint8_t pin, void (*isr)(void*), void* arg);

#endif // ESP32_GPIO_H