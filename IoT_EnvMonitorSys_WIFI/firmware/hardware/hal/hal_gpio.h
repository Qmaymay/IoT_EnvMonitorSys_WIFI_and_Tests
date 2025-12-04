#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "../../include/common.h"

typedef enum {
    GPIO_DIR_INPUT,
    GPIO_DIR_OUTPUT
} gpio_direction_t;

typedef enum {
    GPIO_PULL_NONE,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} gpio_pull_t;

// GPIO抽象接口
result_code_t hal_gpio_init(void);
result_code_t hal_gpio_config(uint8_t pin, gpio_direction_t dir, gpio_pull_t pull);
result_code_t hal_gpio_write(uint8_t pin, bool level);
bool hal_gpio_read(uint8_t pin);

#endif // HAL_GPIO_H