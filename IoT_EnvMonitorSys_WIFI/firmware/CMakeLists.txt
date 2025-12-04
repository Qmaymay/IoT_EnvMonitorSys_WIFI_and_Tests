# ============================================
# firmware/CMakeLists.txt - 与总项目配套
# ============================================

cmake_minimum_required(VERSION 3.10)

# 项目定义
project(env_monitor_firmware
    VERSION 1.0.0
    DESCRIPTION "IoT环境监测系统固件"
    LANGUAGES C
)

# ============================================
# 1. 基本配置
# ============================================

# 使用父项目设置的C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 使用父项目的输出目录设置（不需要再设置）

# ============================================
# 2. 从父项目获取配置
# ============================================

# 使用父项目传递的变量
message(STATUS "固件配置:")
message(STATUS "  平台: ${TARGET_PLATFORM}")
message(STATUS "  Python绑定: ${ENABLE_PYTHON_BINDINGS}")

# 平台定义
if(TARGET_PLATFORM STREQUAL "simulator")
    message(STATUS "  模式: 模拟器")
    add_definitions(-DPLATFORM_SIMULATOR=1)
elseif(TARGET_PLATFORM STREQUAL "esp32")
    message(STATUS "  模式: ESP32")
    add_definitions(-DPLATFORM_ESP32=1)
elseif(TARGET_PLATFORM STREQUAL "stm32")
    message(STATUS "  模式: STM32")
    add_definitions(-DPLATFORM_STM32=1)
endif()

# ============================================
# 3. 源文件设置
# ============================================

# 确保必要的目录存在
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 主程序源文件
set(MAIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

# 如果main.c不存在，创建基础版本
if(NOT EXISTS ${MAIN_SOURCE})
    message(STATUS "创建基础main.c文件")
    file(WRITE ${MAIN_SOURCE}
        "/**\n * IoT环境监测系统 - 主程序\n * 平台: ${TARGET_PLATFORM}\n * 构建时间: __DATE__ __TIME__\n */\n\n#include <stdio.h>\n\nint main(void)\n{\n    printf(\"========================================\\n\");\n    printf(\"IoT环境监测系统\\n\");\n    printf(\"平台: ${TARGET_PLATFORM}\\n\");\n    printf(\"========================================\\n\\n\");\n    \n    printf(\"系统启动成功！\\n\");\n    printf(\"等待传感器数据...\\n\");\n    \n    // 简单循环\n    int count = 0;\n    while (count < 5) {\n        printf(\"运行中... %d\\n\", ++count);\n        #ifdef PLATFORM_SIMULATOR\n        for (int i = 0; i < 100000000; i++); // 简单延时\n        #endif\n    }\n    \n    printf(\"\\n系统停止\\n\");\n    return 0;\n}\n"
    )
endif()

# 配置头文件
set(CONFIG_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)
if(NOT EXISTS ${CONFIG_HEADER})
    file(WRITE ${CONFIG_HEADER}
        "/**\n * 配置头文件\n */\n\n#ifndef CONFIG_H\n#define CONFIG_H\n\n// 版本\n#define FIRMWARE_VERSION_MAJOR 1\n#define FIRMWARE_VERSION_MINOR 0\n#define FIRMWARE_VERSION_PATCH 0\n#define FIRMWARE_VERSION_STRING \"v1.0.0\"\n\n// 平台\n#ifdef PLATFORM_SIMULATOR\n    #define CURRENT_PLATFORM \"simulator\"\n#elif defined(PLATFORM_ESP32)\n    #define CURRENT_PLATFORM \"esp32\"\n#elif defined(PLATFORM_STM32)\n    #define CURRENT_PLATFORM \"stm32\"\n#else\n    #define CURRENT_PLATFORM \"unknown\"\n#endif\n\n#endif /* CONFIG_H */\n"
    )
endif()

# ============================================
# 4. 创建主可执行程序
# ============================================

# 收集所有.c文件
file(GLOB_RECURSE ALL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# 如果没有找到源文件，使用main.c
if(NOT ALL_SOURCES)
    set(ALL_SOURCES ${MAIN_SOURCE})
endif()

message(STATUS "源文件数量: ${ALL_SOURCES}")

# 创建可执行程序
add_executable(env_monitor ${ALL_SOURCES})

# 设置输出名称（父项目已控制输出目录）
set_target_properties(env_monitor PROPERTIES
    OUTPUT_NAME "env_monitor"
)

# ============================================
# 5. Python绑定库（可选）
# ============================================

if(ENABLE_PYTHON_BINDINGS)
    message(STATUS "构建Python绑定库")
    
    # 创建Python接口目录
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python)
    
    # Python接口源文件
    set(PYTHON_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/python/py_interface.c)
    
    if(NOT EXISTS ${PYTHON_SOURCE})
        file(WRITE ${PYTHON_SOURCE}
            "/**\n * Python接口\n */\n\n#include <stdio.h>\n\n// 导出宏\n#ifdef _WIN32\n    #define EXPORT __declspec(dllexport)\n#else\n    #define EXPORT\n#endif\n\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n\nEXPORT void env_monitor_init(void)\n{\n    printf(\"[Python] 环境监测系统初始化\\n\");\n}\n\nEXPORT float get_temperature(void)\n{\n    return 25.5f;\n}\n\nEXPORT float get_humidity(void)\n{\n    return 60.0f;\n}\n\nEXPORT const char* get_version(void)\n{\n    return \"v1.0.0\";\n}\n\n#ifdef __cplusplus\n}\n#endif\n"
        )
    endif()
    
    # 创建共享库
    add_library(env_monitor_shared SHARED ${PYTHON_SOURCE})
    
    # 设置库属性
    set_target_properties(env_monitor_shared PROPERTIES
        OUTPUT_NAME "env_monitor_shared"
        PREFIX ""  # 在Windows上去掉lib前缀
    )
    
    # 添加导出定义
    target_compile_definitions(env_monitor_shared PRIVATE
        -DPYTHON_EXPORT
    )
    
endif()

# ============================================
# 6. 构建完成信息
# ============================================

message(STATUS "")
message(STATUS "固件构建配置完成")
message(STATUS "==========================================")