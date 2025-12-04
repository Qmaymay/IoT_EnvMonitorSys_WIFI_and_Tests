#ifndef CONFIG_H
#define CONFIG_H

// 编译时配置
#define FIRMWARE_VERSION_MAJOR  1
#define FIRMWARE_VERSION_MINOR  0
#define FIRMWARE_VERSION_PATCH  0

#define DEFAULT_DEVICE_ID       "WiFi_001"
#define DEFAULT_SAMPLE_INTERVAL_MS  5000
#define DEFAULT_REPORT_INTERVAL_MS  30000

// MQTT配置
#define DEFAULT_MQTT_BROKER     "mqtt.broker.com"
#define DEFAULT_MQTT_PORT       1883
#define MQTT_KEEPALIVE          60

// WiFi配置
#define WIFI_CONNECT_TIMEOUT_MS  30000
#define WIFI_RETRY_COUNT         3

// 看门狗配置
#define WATCHDOG_TIMEOUT_MS     30000

#endif // CONFIG_H