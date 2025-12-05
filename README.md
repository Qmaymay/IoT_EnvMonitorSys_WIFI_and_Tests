# IoT Environment Monitor System with WiFi

# IoT环境监测系统 - WiFi版本 (v1.0)

## 🚀 项目概述
一个完整的物联网环境监测系统，包含嵌入式设备固件、云端服务和自动化测试框架。设备模拟温湿度传感器数据，通过WiFi连接到MQTT服务器，云端进行数据收集和AI分析。

## 📊 系统架构

```
┌─────────────────┐    WiFi/MQTT    ┌─────────────────┐
│   设备固件层     │────────────────▶│   云端服务层   │
│   (C语言)       │                 │   (Python)      │
│ • SHT30温湿度    │                 │ • 数据收集存储   │
│ • MQ135空气质量  │                 │ • AI分析处理     │
│ • WiFi连接       │◀────────────────│ • 控制指令下发   │
│ • MQTT通信       │                 │                 │
└─────────────────┘                 └─────────────────┘
         │                                  │ 
         └──────────────┬───────────────────┘
                        │
                        ▼                                         
┌─────────────────────────────────────────────────┐
│                 自动化测试验证体系               │
│           (Pytest + 单元/集成/系统测试)         │
└─────────────────────────────────────────────────┘
```

## 📁 项目结构

```
IoT_EnvMonitorSys_WIFI_and_Tests/
├── IoT_EnvMonitorSys_WIFI/          # 主项目
│   ├── firmware/                    # 设备固件
│   │   ├── application/             # 应用层
│   │   │   ├── data_processor.c/h   # 数据处理
│   │   │   ├── state_machine.c/h    # 状态机
│   │   │   └── states/              # 各状态实现
│   │   ├── esp32/                   # ESP32平台抽象
│   │   │   ├── esp32_gpio.c/h       # GPIO驱动
│   │   │   ├── esp32_i2c.c/h        # I2C驱动
│   │   │   └── esp32_wifi.c/h       # WiFi连接
│   │   ├── hardware/                # 硬件层
│   │   │   ├── drivers/             # 传感器驱动
│   │   │   │   ├── sht30.c/h        # 温湿度传感器
│   │   │   │   ├── mq135.c/h        # 空气质量传感器
│   │   │   │   └── sensor_driver.c/h# 传感器管理器
│   │   │   └── hal/                 # 硬件抽象层
│   │   ├── include/                 # 公共头文件
│   │   │   ├── common.h             # 通用数据类型
│   │   │   ├── config.h             # 编译配置
│   │   │   └── app_config.h         # 应用配置
│   │   ├── src/                     # 主程序
│   │   │   └── main.c               # 主入口点
│   │   ├── network/                 # 网络层
│   │   │   ├── mqtt_manager.c/h     # MQTT管理
│   │   │   └── ota_updater.c/h      # OTA升级
│   │   ├── system/                  # 系统层
│   │   │   ├── logger.c/h           # 日志系统
│   │   │   ├── watchdog.c/h         # 看门狗
│   │   │   └── config_manager.c/h   # 配置管理
│   │   └── unit_tests/              # 单元测试
│   └── cloud_services/              # 云端服务
│       ├── ai_analyzer/             # AI分析
│       ├── data_collector/          # 数据收集
│       └── shared/                  # 共享资源
└── test_engine/                     # 测试框架
    ├── tests/                       # 测试用例
    │   ├── smoke/                   # 冒烟测试
    │   ├── system/                  # 系统测试
    │   ├── reliability/             # 可靠性测试
    │   └── performance/             # 性能测试
    └── utils/                       # 测试工具
```

## 🔧 核心功能模块

### 1. 设备固件 (`firmware/`)
- **多状态机架构**：初始化 → WiFi连接 → MQTT连接 → 正常工作 → 错误处理
- **传感器模拟**：
  - SHT30: 温度(-20°C ~ 50°C)、湿度(0%~100%)
  - MQ135: 空气质量检测(0-1000 PPM)
- **网络通信**：
  - WiFi连接管理 (支持重连机制)
  - MQTT发布/订阅 (支持主题: env_monitor/data)
  - 数据上报频率可配置

### 2. 云端服务 (`cloud_services/`)
- **MQTT数据接收**：实时接收设备数据
- **SQLite数据库**：数据持久化存储
- **AI分析引擎**：环境数据聚类和预测分析
- **RESTful API**：数据查询和监控接口

### 3. 自动化测试 (`test_engine/`)
- **单元测试**：各模块功能验证
- **集成测试**：模块间接口测试
- **系统测试**：端到端业务流程验证
- **性能测试**：压力测试和性能评估

## 📦 硬件接口定义

### 传感器数据结构 (`include/common.h`)
```c
typedef struct {
    float temperature;      // 摄氏度
    float humidity;         // 百分比
    float air_quality;      // 空气质量指数
    float battery_voltage;  // 电池电压
    uint32_t timestamp;     // Unix时间戳
    uint16_t sequence;      // 数据序列号
} sensor_data_t;
```

### 设备状态枚举
```c
typedef enum {
    DEVICE_STATE_INIT,           // 初始化
    DEVICE_STATE_CONNECTING_WIFI,// 连接WiFi
    DEVICE_STATE_CONNECTING_MQTT,// 连接MQTT
    DEVICE_STATE_NORMAL,         // 正常工作
    DEVICE_STATE_ERROR           // 错误处理
} device_state_t;
```

## 🚀 快速开始

### 环境要求
- **编译器**: CMake 3.10+, Visual Studio 2019+ 或 GCC
- **Python**: 3.9+ (用于云端服务和测试)
- **依赖库**: paho-mqtt, scikit-learn, pytest

### 构建设备固件
```bash
# 1. 配置项目
mkdir build && cd build
cmake ..

# 2. 编译
cmake --build .

# 3. 运行模拟器版本
.\bin\env_monitor_app.exe
```

### 配置参数 (`include/config.h`)
```c
#define WIFI_SSID "Your_WiFi_SSID"
#define WIFI_PASSWORD "Your_WiFi_Password"
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_PORT 1883
#define SAMPLE_INTERVAL_MS 5000  // 采样间隔
```

### 运行云端服务
```bash
# 安装Python依赖
pip install -r cloud_services/requirements.txt

# 启动MQTT数据接收器
python cloud_services/data_collector/mqtt_receiver.py

# 启动AI分析服务
python cloud_services/ai_analyzer/main.py
```

### 运行测试套件
```bash
cd test_engine
# 运行所有测试
pytest tests/

# 运行特定测试类别
pytest tests/smoke/ -v        # 冒烟测试
pytest tests/system/ -v       # 系统测试
pytest tests/performance/ -v  # 性能测试
```

## 🧪 测试策略

### 1. 设备固件测试
- **硬件模拟测试**: GPIO/I2C接口验证
- **状态机测试**: 状态转换逻辑验证
- **网络测试**: WiFi/MQTT连接稳定性
- **数据验证**: 传感器数据范围和精度

### 2. 云端服务测试
- **数据接收测试**: MQTT消息处理
- **数据库测试**: 数据持久化验证
- **AI算法测试**: 分析准确性验证
- **API测试**: REST接口功能验证

### 3. 系统集成测试
- **端到端测试**: 设备→云端完整流程
- **压力测试**: 高并发数据接收
- **恢复测试**: 网络中断恢复能力
- **安全测试**: 认证和加密验证

## ⚙️ 构建配置

### CMake配置选项
```bash
# 指定目标平台
cmake -DTARGET_PLATFORM=simulator ..     # 模拟器模式
cmake -DTARGET_PLATFORM=esp32 ..         # ESP32模式

# 启用Python绑定
cmake -DENABLE_PYTHON_BINDINGS=ON ..

# 指定构建类型
cmake -DCMAKE_BUILD_TYPE=Debug ..        # 调试版本
cmake -DCMAKE_BUILD_TYPE=Release ..      # 发布版本
```

### 输出文件
- **可执行程序**: `env_monitor_app.exe` (Windows)
- **动态库**: `env_monitor.dll` (供Python调用)
- **调试文件**: `env_monitor_app.pdb` (调试符号)

## 🔄 开发工作流

1. **功能开发** → 在feature分支开发新功能
2. **单元测试** → 编写和运行单元测试
3. **代码审查** → 提交Pull Request进行审查
4. **集成测试** → 合并到develop分支运行集成测试
5. **系统测试** → 发布前运行完整系统测试
6. **版本发布** → 打tag发布稳定版本

## 📈 监控和日志

### 日志级别
```c
typedef enum {
    LOG_LEVEL_ERROR = 0,    // 错误
    LOG_LEVEL_WARN,         // 警告
    LOG_LEVEL_INFO,         // 信息
    LOG_LEVEL_DEBUG,        // 调试
    LOG_LEVEL_VERBOSE       // 详细
} log_level_t;
```

### 日志输出示例
```
[2025-12-15 14:30:25] [MAIN] === IoT环境监测系统启动 ===
[2025-12-15 14:30:26] [WIFI] 连接到WiFi: MyWiFi
[2025-12-15 14:30:27] [MQTT] 连接到服务器: test.mosquitto.org:1883
[2025-12-15 14:30:32] [SENSOR] 温度: 25.3°C, 湿度: 60.2%, 空气质量: 45
```

## 🐛 故障排除

### 常见问题

1. **编译错误**: 确保CMake版本≥3.10，安装必要的编译工具链
2. **WiFi连接失败**: 检查SSID/密码，确认网络可达
3. **MQTT连接失败**: 确认broker地址和端口正确，防火墙设置
4. **传感器数据异常**: 检查模拟器参数范围设置





