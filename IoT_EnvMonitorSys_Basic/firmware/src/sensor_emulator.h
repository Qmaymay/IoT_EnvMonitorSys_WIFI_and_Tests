// sensor_emulator.h - 正确
#ifndef SENSOR_EMULATOR_H
#define SENSOR_EMULATOR_H

#include "common.h"
#include "env_monitor_export.h"

#ifdef __cplusplus
extern "C" {
#endif

// 只在头文件中使用 ENV_MONITOR_API
ENV_MONITOR_API result_code_t sensor_emulator_init(void);
ENV_MONITOR_API result_code_t sensor_emulator_read(sensor_data_t* data);
ENV_MONITOR_API float sensor_get_temperature(void);
ENV_MONITOR_API float sensor_get_humidity(void);
ENV_MONITOR_API float sensor_get_air_quality(void);

#ifdef __cplusplus
}
#endif

#endif