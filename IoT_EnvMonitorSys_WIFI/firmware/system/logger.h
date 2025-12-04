#ifndef LOGGER_H
#define LOGGER_H

#include "../include/common.h"

// 日志级别
typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE
} log_level_t;

// 日志初始化
result_code_t logger_init(log_level_t level);
result_code_t logger_set_level(log_level_t level);

// 日志记录函数
void log_error(const char* tag, const char* format, ...);
void log_warn(const char* tag, const char* format, ...);
void log_info(const char* tag, const char* format, ...);
void log_debug(const char* tag, const char* format, ...);
void log_verbose(const char* tag, const char* format, ...);

// 辅助宏
#define LOG_E(tag, ...) log_error(tag, __VA_ARGS__)
#define LOG_W(tag, ...) log_warn(tag, __VA_ARGS__)
#define LOG_I(tag, ...) log_info(tag, __VA_ARGS__)
#define LOG_D(tag, ...) log_debug(tag, __VA_ARGS__)
#define LOG_V(tag, ...) log_verbose(tag, __VA_ARGS__)

#endif // LOGGER_H