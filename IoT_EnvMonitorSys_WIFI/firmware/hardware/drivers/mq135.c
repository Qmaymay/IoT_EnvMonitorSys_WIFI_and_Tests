#include "mq135.h"
#include <stdio.h>
#include <stdlib.h>

result_code_t mq135_init(void) {
    printf("[MQ135] Initialize air quality sensor\n");
    return RESULT_OK;
}

result_code_t mq135_read_raw(uint16_t* raw_value) {
    *raw_value = 500 + (rand() % 500);
    printf("[MQ135] Raw value: %d\n", *raw_value);
    return RESULT_OK;
}

result_code_t mq135_calibrate(void) {
    printf("[MQ135] Calibrate sensor\n");
    return RESULT_OK;
}

float mq135_calculate_ppm(uint16_t raw_value) {
    // Simple conversion formula (simulation)
    float ppm = (float)raw_value * 0.1f;
    printf("[MQ135] Calculate PPM: %.1f\n", ppm);
    return ppm;
}