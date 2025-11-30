# 总项目根目录的CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# 总项目名称
project(IoT_EnvMonitorSys_Complete)

# 设置C标准
set(CMAKE_C_STANDARD 11)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

# 直接添加 firmware 子目录
add_subdirectory(IoT_EnvMonitorSys_Basic/firmware)

message(STATUS "=== 完整项目构建 ===")
message(STATUS "输出目录: ${CMAKE_SOURCE_DIR}/build/bin")
message(STATUS "==========================================")
message(STATUS "可用目标:")
message(STATUS "  env_monitor      - 设备可执行程序")
message(STATUS "  env_monitor_shared - 设备动态库") 
message(STATUS "  env_monitor_static - 设备静态库")
message(STATUS "==========================================")