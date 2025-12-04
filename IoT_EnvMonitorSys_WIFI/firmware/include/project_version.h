#ifndef PROJECT_VERSION_H
#define PROJECT_VERSION_H

#define PROJECT_NAME         "IoT环境监测系统"
#define PROJECT_VERSION      "1.0.0"
#define PROJECT_BUILD_DATE   __DATE__
#define PROJECT_BUILD_TIME   __TIME__

// 固件版本号组合
#define FIRMWARE_VERSION ((FIRMWARE_VERSION_MAJOR << 16) | \
                         (FIRMWARE_VERSION_MINOR << 8) | \
                          FIRMWARE_VERSION_PATCH)

#endif // PROJECT_VERSION_H