cmake_minimum_required(VERSION 3.10)

# 总项目名称
project(IoT_EnvMonitorSys_Complete)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 关键：设置全局输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

# 创建构建目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 定义变量（从命令行接收或使用默认值）
set(TARGET_PLATFORM "simulator" CACHE STRING "目标平台 (simulator/esp32/stm32)")
set(ENABLE_PYTHON_BINDINGS OFF CACHE BOOL "启用Python绑定")

# 直接添加 firmware 子目录
add_subdirectory(IoT_EnvMonitorSys_WIFI/firmware)

# 只设置一个target
# if(TARGET env_monitor_wifi)
#     set_target_properties(env_monitor_wifi PROPERTIES
#         RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
#         RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
#         LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
#         LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
#     )
# endif()


message(STATUS "=== 完整项目构建 ===")
message(STATUS "项目名称: ${PROJECT_NAME}")
message(STATUS "目标平台: ${TARGET_PLATFORM}")
message(STATUS "输出目录: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "==========================================")
message(STATUS "可用目标:")
message(STATUS "  env_monitor      - 设备可执行程序")
message(STATUS "  env_monitor_shared - 设备动态库") 
message(STATUS "==========================================")