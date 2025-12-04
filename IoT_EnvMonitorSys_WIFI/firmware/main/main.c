/**
 * firmware/main.c
 * 
 * IoT环境监测系统 - 主程序入口
 * Windows兼容版本
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// Windows特定头文件
#ifdef _WIN32
#include <windows.h>
#endif

#include "../include/common.h"
#include "../include/config.h"
#include "../system/logger.h"
#include "../application/state_machine.h"
#include "../system/watchdog.h"

// ============================================
// 全局变量（极简版，先跑起来！）
// ============================================

// 设备状态
static device_status_t g_device_status = {
    .device_id = DEFAULT_DEVICE_ID,
    .firmware_version = (FIRMWARE_VERSION_MAJOR << 16) | 
                       (FIRMWARE_VERSION_MINOR << 8) | 
                       FIRMWARE_VERSION_PATCH,
    .uptime_ms = 0,
    .data_count = 0,
    .wifi_rssi = 0,
    .mqtt_connected = false,
    .battery_level = 100
};

// 传感器数据缓存
static sensor_data_t g_sensor_data = {0};

// ============================================
// 时间工具函数（Windows兼容版本）
// ============================================

/**
 * @brief 获取当前时间（毫秒）
 * Windows兼容实现
 */
uint32_t get_current_time_ms(void)
{
#ifdef _WIN32
    // Windows高性能计时器实现
    static LARGE_INTEGER frequency = {0};
    static LARGE_INTEGER start_time = {0};
    static uint32_t is_initialized = 0;
    
    if (!is_initialized) {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start_time);
        is_initialized = 1;
        return 0;
    }
    
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    
    // 计算经过的时间（毫秒）
    uint64_t elapsed_ticks = current_time.QuadPart - start_time.QuadPart;
    uint64_t elapsed_ms = (elapsed_ticks * 1000) / frequency.QuadPart;
    
    return (uint32_t)elapsed_ms;
#else
    // Linux/POSIX实现
    static uint32_t start_time = 0;
    static struct timespec ts;
    
    if (start_time == 0) {
        clock_gettime(CLOCK_MONOTONIC, &ts);
        start_time = (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
        return 0;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint32_t current = (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    return current - start_time;
#endif
}

/**
 * @brief 延迟函数（毫秒）
 * Windows兼容实现
 */
void delay_ms(uint32_t ms)
{
#ifdef _WIN32
    // Windows实现
    Sleep(ms);
#else
    // Linux/POSIX实现
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}

/**
 * @brief 获取Unix时间戳
 */
uint32_t get_timestamp(void)
{
    return (uint32_t)time(NULL);
}

// ============================================
// 系统初始化（极简版）
// ============================================

/**
 * @brief 系统初始化
 * @return 初始化结果
 */
static result_code_t system_init_simple(void)
{
    printf("\n");
    printf("=========================================\n");
    printf("  IoT环境监测系统启动\n");
    printf("  版本: v%d.%d.%d\n", 
           FIRMWARE_VERSION_MAJOR,
           FIRMWARE_VERSION_MINOR,
           FIRMWARE_VERSION_PATCH);
    printf("  编译时间: %s %s\n", __DATE__, __TIME__);
    printf("  设备ID: %s\n", DEFAULT_DEVICE_ID);
    printf("=========================================\n\n");
    
    // 1. 初始化日志系统（最简，直接printf）
    printf("[LOGGER] 日志系统初始化\n");
    
    // 2. 初始化看门狗（如果启用）
#ifdef ENABLE_WATCHDOG
    printf("[WATCHDOG] 看门狗初始化\n");
    watchdog_init(WATCHDOG_TIMEOUT_MS);
#endif
    
    // 3. 初始化状态机
    printf("[STATE] 状态机初始化\n");
    if (state_machine_init() != RESULT_OK) {
        printf("[ERROR] 状态机初始化失败\n");
        return RESULT_ERROR;
    }
    
    printf("[SYSTEM] 系统初始化完成\n");
    return RESULT_OK;
}

// ============================================
// 模拟传感器数据（先让程序跑起来！）
// ============================================

/**
 * @brief 模拟读取传感器数据
 * @param data 传感器数据指针
 * @return 读取结果
 */
static result_code_t read_sensor_data_mock(sensor_data_t* data)
{
    if (!data) return RESULT_INVALID_PARAM;
    
    // 模拟数据（以后换成真实传感器）
    static uint16_t sequence = 0;
    
    data->temperature = 25.0f + (rand() % 100) / 100.0f;  // 25.0-26.0℃
    data->humidity = 60.0f + (rand() % 100) / 100.0f;    // 60.0-61.0%
    data->air_quality = 50.0f + (rand() % 200) / 10.0f;  // 50.0-70.0
    data->battery_voltage = 3.7f + (rand() % 50) / 100.0f; // 3.7-4.2V
    data->timestamp = get_timestamp();
    data->sequence = sequence++;
    
    // 更新统计数据
    g_device_status.data_count++;
    
    return RESULT_OK;
}

/**
 * @brief 显示传感器数据
 * @param data 传感器数据
 */
static void display_sensor_data(const sensor_data_t* data)
{
    if (!data) return;
    
    printf("[SENSOR] 数据 #%u:\n", data->sequence);
    printf("  温度: %.1f°C\n", data->temperature);
    printf("  湿度: %.1f%%\n", data->humidity);
    printf("  空气质量: %.1f\n", data->air_quality);
    printf("  电池电压: %.2fV\n", data->battery_voltage);
    
    // 将时间戳转换为可读格式
    time_t timestamp = (time_t)data->timestamp;
    struct tm* timeinfo = localtime(&timestamp);
    char time_buffer[64];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    printf("  时间: %s\n", time_buffer);
}

// ============================================
// 简单状态处理
// ============================================

/**
 * @brief 处理初始化状态
 */
static void handle_init_state(void)
{
    printf("[STATE] 初始化状态\n");
    
    // 模拟初始化过程
    printf("  - 检查硬件...\n");
    delay_ms(500);
    printf("  - 初始化传感器...\n");
    delay_ms(500);
    printf("  - 连接网络...\n");
    delay_ms(500);
    printf("  - 初始化完成，进入正常工作状态\n");
    
    // 切换到正常状态
    state_machine_handle_event(EVENT_INIT_COMPLETE);
}

/**
 * @brief 处理正常工作状态
 */
static void handle_normal_state(void)
{
    static uint32_t last_read_time = 0;
    static uint32_t last_report_time = 0;
    uint32_t current_time = get_current_time_ms();
    
    // 每5秒读取一次传感器
    if (current_time - last_read_time >= DEFAULT_SAMPLE_INTERVAL_MS) {
        printf("[NORMAL] 读取传感器数据\n");
        
        if (read_sensor_data_mock(&g_sensor_data) == RESULT_OK) {
            display_sensor_data(&g_sensor_data);
            last_read_time = current_time;
        }
    }
    
    // 每30秒"上报"一次（这里只是打印）
    if (current_time - last_report_time >= DEFAULT_REPORT_INTERVAL_MS) {
        printf("[NORMAL] 模拟数据上报\n");
        printf("  MQTT服务器: %s:%d\n", DEFAULT_MQTT_BROKER, DEFAULT_MQTT_PORT);
        printf("  数据序列: %u\n", g_sensor_data.sequence);
        last_report_time = current_time;
    }
    
    // 模拟一些状态变化
    static int counter = 0;
    counter++;
    
    if (counter % 20 == 0) {
        // 每20次循环模拟WiFi信号变化
        g_device_status.wifi_rssi = -50 - (rand() % 30);
        printf("[NETWORK] WiFi信号强度: %d dBm\n", g_device_status.wifi_rssi);
    }
    
    if (counter % 30 == 0) {
        // 每30次循环模拟MQTT连接状态变化
        g_device_status.mqtt_connected = !g_device_status.mqtt_connected;
        printf("[NETWORK] MQTT连接状态: %s\n", 
               g_device_status.mqtt_connected ? "已连接" : "断开");
    }
}

/**
 * @brief 处理配置状态
 */
static void handle_config_state(void)
{
    printf("[CONFIG] 配置模式\n");
    printf("  等待配置...\n");
    printf("  1. 连接到热点: ESP32-Config\n");
    printf("  2. 访问: http://192.168.4.1\n");
    printf("  3. 配置WiFi和MQTT参数\n");
    
    // 5秒后自动"完成"配置
    static uint32_t config_start_time = 0;
    if (config_start_time == 0) {
        config_start_time = get_current_time_ms();
    }
    
    if (get_current_time_ms() - config_start_time > 5000) {
        printf("[CONFIG] 配置完成，返回正常模式\n");
        state_machine_handle_event(EVENT_CONFIG_COMPLETE);
        config_start_time = 0;
    }
}

/**
 * @brief 主状态机循环
 */
static void main_state_loop(void)
{
    device_state_t current_state = state_machine_get_current_state();
    
    // 更新运行时间
    static uint32_t start_time = 0;
    if (start_time == 0) {
        start_time = get_current_time_ms();
    }
    g_device_status.uptime_ms = get_current_time_ms() - start_time;
    
    // 根据当前状态执行不同的处理
    switch (current_state) {
        case DEVICE_STATE_INIT:
            handle_init_state();
            break;
            
        case DEVICE_STATE_NORMAL:
            handle_normal_state();
            break;
            
        case DEVICE_STATE_CONFIG:
            handle_config_state();
            break;
            
        case DEVICE_STATE_ERROR:
            printf("[ERROR] 错误处理状态\n");
            delay_ms(1000);
            // 3秒后尝试恢复
            static uint32_t error_start_time = 0;
            if (error_start_time == 0) {
                error_start_time = get_current_time_ms();
            }
            if (get_current_time_ms() - error_start_time > 3000) {
                printf("[ERROR] 尝试恢复...\n");
                state_machine_handle_event(EVENT_ERROR_RESOLVED);
                error_start_time = 0;
            }
            break;
            
        case DEVICE_STATE_OTA:
            printf("[OTA] 固件升级状态\n");
            delay_ms(5000);  // 模拟升级过程
            printf("[OTA] 升级完成，重启系统\n");
            // 这里应该重启，我们模拟回到正常状态
            state_machine_handle_event(EVENT_OTA_COMPLETE);
            break;
            
        case DEVICE_STATE_SLEEP:
            printf("[SLEEP] 低功耗睡眠模式\n");
            delay_ms(2000);  // 模拟睡眠
            printf("[SLEEP] 唤醒\n");
            state_machine_handle_event(EVENT_WAKE_UP);
            break;
            
        default:
            printf("[ERROR] 未知状态: %d\n", current_state);
            break;
    }
}

// ============================================
// 模拟外部事件（用于测试状态转换）
// ============================================

/**
 * @brief 模拟外部事件生成
 */
static void simulate_external_events(void)
{
    static uint32_t last_event_time = 0;
    uint32_t current_time = get_current_time_ms();
    
    // 每15秒生成一个随机事件（用于测试）
    if (current_time - last_event_time > 15000) {
        int event_type = rand() % 6;
        
        switch (event_type) {
            case 0:
                printf("[EVENT] 模拟: 按钮按下，进入配置模式\n");
                state_machine_handle_event(EVENT_CONFIG_START);
                break;
                
            case 1:
                printf("[EVENT] 模拟: 传感器错误\n");
                state_machine_handle_event(EVENT_ERROR_OCCURRED);
                break;
                
            case 2:
                printf("[EVENT] 模拟: 收到OTA升级命令\n");
                state_machine_handle_event(EVENT_OTA_START);
                break;
                
            case 3:
                printf("[EVENT] 模拟: 进入低功耗模式\n");
                state_machine_handle_event(EVENT_ENTER_SLEEP);
                break;
                
            case 4:
                printf("[EVENT] 模拟: 电源重新上电\n");
                state_machine_handle_event(EVENT_POWER_ON);
                break;
        }
        
        last_event_time = current_time;
    }
}

// ============================================
// 主程序入口
// ============================================

/**
 * @brief 主函数
 * @return 程序退出码
 */
int main(void)
{
    // 初始化随机种子（用于模拟数据）
    srand((unsigned int)time(NULL));
    
    // 1. 系统初始化
    result_code_t ret = system_init_simple();
    if (ret != RESULT_OK) {
        printf("[FATAL] 系统初始化失败，程序退出\n");
        return -1;
    }
    
    printf("\n[SYSTEM] 系统启动成功，开始主循环\n");
    printf("按Ctrl+C退出程序\n\n");
    
    // 2. 主循环
    uint32_t loop_count = 0;
    while (1) {
        loop_count++;
        
        // 打印循环计数（每10次打印一次）
        if (loop_count % 10 == 0) {
            printf("[MAIN] 循环计数: %u, 运行时间: %.1f秒\n", 
                   loop_count, g_device_status.uptime_ms / 1000.0);
        }
        
        // 喂狗（如果启用）
#ifdef ENABLE_WATCHDOG
        watchdog_feed_all();
#endif
        
        // 执行状态机主循环
        main_state_loop();
        
        // 模拟外部事件
        simulate_external_events();
        
        // 短暂延迟，避免CPU占用100%
        delay_ms(100);
        
        // 模拟看门狗超时测试（每1000次循环）
        if (loop_count % 1000 == 999) {
            printf("[TEST] 模拟看门狗测试点\n");
        }
    }
    
    // 程序不会执行到这里
    return 0;
}