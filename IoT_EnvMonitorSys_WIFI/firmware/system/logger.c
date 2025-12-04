#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

static log_level_t current_level = LOG_LEVEL_INFO;

result_code_t logger_init(log_level_t level) {
    current_level = level;
    printf("[Logger] Initialization completed, log level: %d\n", level);
    return RESULT_OK;
}

result_code_t logger_set_level(log_level_t level) {
    current_level = level;
    return RESULT_OK;
}

void log_error(const char* tag, const char* format, ...) {
    if (current_level < LOG_LEVEL_ERROR) return;
    
    time_t now = time(NULL);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("[%s] [ERROR] [%s] ", time_str, tag);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}

void log_warn(const char* tag, const char* format, ...) {
    if (current_level < LOG_LEVEL_WARN) return;
    
    time_t now = time(NULL);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("[%s] [WARN]  [%s] ", time_str, tag);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}

void log_info(const char* tag, const char* format, ...) {
    if (current_level < LOG_LEVEL_INFO) return;
    
    time_t now = time(NULL);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("[%s] [INFO]  [%s] ", time_str, tag);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}

void log_debug(const char* tag, const char* format, ...) {
    if (current_level < LOG_LEVEL_DEBUG) return;
    
    time_t now = time(NULL);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("[%s] [DEBUG] [%s] ", time_str, tag);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}

void log_verbose(const char* tag, const char* format, ...) {
    if (current_level < LOG_LEVEL_VERBOSE) return;
    
    time_t now = time(NULL);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("[%s] [VERBOSE] [%s] ", time_str, tag);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}