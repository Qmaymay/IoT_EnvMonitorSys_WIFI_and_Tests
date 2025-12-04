#ifndef ENV_MONITOR_EXPORT_H
#define ENV_MONITOR_EXPORT_H

#ifdef __cplusplus
extern "C" {
#endif

// 跨平台导出宏
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
}
#endif

#endif // ENV_MONITOR_EXPORT_H