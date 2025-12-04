#include "sht30.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simulate SHT30 sensor
result_code_t sht30_init(void) {
    printf("[SHT30] Initialize sensor\n");
    return RESULT_OK;
}

result_code_t sht30_read(sht30_data_t* data) {
    // Simulate reading temperature and humidity
    data->temperature = 25.0f + ((rand() % 100) / 10.0f - 5.0f);
    data->humidity = 60.0f + ((rand() % 100) / 10.0f - 5.0f);
    data->crc_ok = 1;
    
    printf("[SHT30] Temperature: %.1fC, Humidity: %.1f%%\n", 
           data->temperature, data->humidity);
    
    return RESULT_OK;
}

result_code_t sht30_soft_reset(void) {
    printf("[SHT30] Software reset\n");
    return RESULT_OK;
}

uint8_t sht30_check_crc(uint16_t data, uint8_t crc) {
    // Simple CRC check simulation
    return 1;
}