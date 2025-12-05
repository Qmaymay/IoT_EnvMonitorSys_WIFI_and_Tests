cmake_minimum_required(VERSION 3.10)

# 项目名称和语言
project(env_monitor_wifi C)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 编译选项
if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -g)
endif()

# 禁止安全警告
add_definitions(-D_CRT_SECURE_NO_WARNINGS)


# 平台定义
set(TARGET_PLATFORM "simulator" CACHE STRING "目标平台 (simulator/esp32/stm32)")

if(TARGET_PLATFORM STREQUAL "simulator")
    message(STATUS "  模式: 模拟器")
    add_definitions(-DPLATFORM_SIMULATOR=1)
    
elseif(TARGET_PLATFORM STREQUAL "esp32")
    message(STATUS "  模式: ESP32")
    add_definitions(-DPLATFORM_ESP32=1)
    
    # ESP32特定设置
    # 可以添加IDF组件等
    
elseif(TARGET_PLATFORM STREQUAL "stm32")
    message(STATUS "  模式: STM32")
    add_definitions(-DPLATFORM_STM32=1)
    
    # STM32特定设置
    # 可以添加HAL库等
endif()


# 包含头文件目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/drivers
)

# 源文件列表
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/drivers/sht30.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/drivers/mq135.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware/drivers/sensor_driver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system/logger.c
)


# 可执行文件 - 主设备程序
add_executable(env_monitor_wifi ${SOURCES})

# Windows库链接（放在这里）
if(WIN32)
    target_link_libraries(env_monitor_wifi)
    # 平台特定的链接也可以放这里
    if(TARGET_PLATFORM STREQUAL "simulator")
        target_link_libraries(env_monitor_wifi ws2_32)  # ✅ 现在target已存在
    endif()
endif()



# 动态库 - 供Python测试调用
add_library(env_monitor_shared SHARED ${SOURCES})

# 导出定义（如果需要）
target_compile_definitions(env_monitor_shared PRIVATE ENV_MONITOR_DLL_EXPORTS)

# 设置库的属性
set_target_properties(env_monitor_shared PROPERTIES
    OUTPUT_NAME "env_monitor"
    VERSION 1.0.0
    SOVERSION 1
)

# 设置可执行文件输出名称，避免与库文件冲突
set_target_properties(env_monitor_wifi PROPERTIES
    OUTPUT_NAME "env_monitor_app"
)

message(STATUS "=== IoT WiFi Monitor CMake配置 ===")
message(STATUS "可执行文件输出: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "动态库输出: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "生成目标:")
message(STATUS "  env_monitor_app     - 设备可执行程序")
message(STATUS "  env_monitor         - 设备动态库")