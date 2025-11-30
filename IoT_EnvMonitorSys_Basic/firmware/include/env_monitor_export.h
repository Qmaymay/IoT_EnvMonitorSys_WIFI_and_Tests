// IoT_EnvMonitorSys_Basic/firmware/include/env_monitor_export.h
#ifndef ENV_MONITOR_EXPORT_H
#define ENV_MONITOR_EXPORT_H

#ifdef _WIN32
    #ifdef ENV_MONITOR_DLL_EXPORTS
        #define ENV_MONITOR_API __declspec(dllexport)
    #else
        #define ENV_MONITOR_API __declspec(dllimport)
    #endif
#else
    #define ENV_MONITOR_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 这里可以放通用的导出函数声明

#ifdef __cplusplus
}
#endif

#endif // ENV_MONITOR_EXPORT_H