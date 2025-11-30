# IoT Environment Monitor System - Basic
# IoT环境监测系统 - 基础PC版 (v1.0)

## 版本特性
- 单循环架构
- 模拟传感器数据
- 云端AI数据分析
- 完整自动化测试框架

## 项目结构
- `Iot_EnvMonitorSys_Basic/` - 被测系统
- `test-engine/` - 测试工程

# IoT环境监测系统 - 技术框架

# IoT环境监测系统

## 项目概述
一个嵌入式设备与云端服务协同工作的典型架构，包含设备数据采集、云端数据处理，AI分析服务。
一个自动化测试整个"物联网环境监测系统"的工程架构。

## 系统架构

```
┌─────────────────┐    MQTT     ┌─────────────────┐
│   设备固件层     │─────────────▶│   云端服务层   │
│   (C语言)       │             │   (Python)      │
│ • 传感器数据模拟 │             │ • 数据收集存储   │
│ • 设备通信       │             │ • AI分析处理     │
└─────────────────┘             └─────────────────┘
         │                              │ 
         └──────────────┬───────────────┘
                        │
                        ▼                                         
┌─────────────────────────────────────────────────┐
│                 测试验证体系                     │
│           (集成测试 & 系统测试)                   │
└─────────────────────────────────────────────────┘

```

## 核心模块
 
### 设备固件 (`firmware/`) 详细介绍见README_FW.md
- 多区域气候数据模拟
- MQTT协议通信
- 跨平台动态库支持

### 云端服务 (`cloud_services/`) 详细介绍见README_AI.md
- 固件传感器数据收集与存储 (SQLite)
- 环境数据AI分析 (scikit-learn)
- 模块化服务设计

### 测试引擎 (`test_engine/`) 详细介绍见README_TEST.md
- **设备固件模块测试**：传感器数据验证、功能测试
- **云端服务模块测试**：数据处理、AI分析验证
- **端到端系统测试**：完整业务流程验证


## 快速开始

### 环境准备
```bash
# 安装依赖
pip install paho-mqtt scikit-learn pytest

# 构建设备程序
mkdir build && cd build
cmake .. && cmake --build .
```

### 运行演示
```bash
# 启动数据接收服务
python cloud_services/data_collector/mqtt_receiver.py

# 运行设备模拟 (新终端)
cd build
cmake --build . --config Debug
cd ..
& ".\build\bin\Debug\env_monitor_app.exe"

# 执行测试验证
cd test_engine && pytest tests/
```


## 总结

该项目提供了一个可参考的IoT系统实现方案，涵盖了设备端到云端的完整技术栈。代码结构清晰，模块划分合理，适合作为相关领域的技术参考和学习材料。
