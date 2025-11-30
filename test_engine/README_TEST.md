# 测试引擎模块

## 模块概述

测试引擎模块为IoT环境监测系统提供完整的自动化测试验证体系，涵盖设备固件和云端服务的集成测试、系统测试和端到端验证。

## 设计意图

- **质量保障**：确保设备固件与云端服务的协同工作可靠性
- **自动化验证**：建立持续集成中的自动化测试流水线
- **跨模块测试**：验证C固件与Python服务的数据交互正确性
- **回归保护**：防止新功能引入对现有系统的破坏

## 目录结构

```
test_engine/
├── tests/                 # 测试用例目录
│   ├── test_integration.py    # 系统集成测试
│   ├── test_sensor.py         # 传感器模块测试
│   ├── test_cloud_services.py # 云端服务测试
│   ├── test_complete_data_flow.py # 完整数据流测试
│   ├── test_performance.py    # 性能测试
│   ├── test_reliability.py    # 可靠性测试
│   └── conftest.py           # pytest配置
├── utils/                 # 测试工具
│   └── path_resolver.py      # 构建产物路径解析
└── requirements.txt       # 测试依赖
```

## 核心组件

### 1. 集成测试 (`tests/test_integration.py`)

**测试重点**：
- 设备固件与云端服务的完整集成
- MQTT通信链路验证
- 跨语言接口兼容性

**关键技术**：
- 动态库加载测试 (ctypes)
- 可执行文件功能验证
- 构建产物完整性检查

### 2. 传感器测试 (`tests/test_sensor.py`)

**测试重点**：
- 传感器数据合理性和连续性
- 多区域气候数据验证
- 数据突变和时间断裂检测

**关键技术**：
- 数据范围验证 (-15°C ~ 30°C, 15% ~ 100%)
- 时间序列连续性分析
- 统计异常检测

### 3. 云端服务测试 (`tests/test_cloud_services.py`)

**测试重点**：
- AI分析功能正确性
- 数据流处理完整性
- 服务模块集成验证

### 4. 数据流测试 (`tests/test_complete_data_flow.py`)

**测试重点**：
- 端到端业务流程验证
- 真实数据场景测试
- 数据库操作完整性

## 模块关系

### 测试覆盖范围
```
测试引擎
    ├── 设备固件测试 (test_sensor.py)
    │   └── 验证: 数据生成 → 动态库接口 → 功能正确性
    │
    ├── 云端服务测试 (test_cloud_services.py)  
    │   └── 验证: AI分析 → 数据处理 → 数据库操作
    │
    └── 系统集成测试 (test_integration.py)
        └── 验证: 固件+服务完整链路 → MQTT通信 → 端到端流程
```

### 依赖关系
```
tests/
    → utils/path_resolver.py (定位构建产物)
    → IoT_EnvMonitorSys_Basic/cloud_services/ (测试云端服务)
    → build/bin/ (测试设备固件动态库)
```

## 运行命令

### 环境准备
```bash
# 安装测试依赖
cd test_engine
pip install -r requirements.txt

# 或手动安装主要依赖
pip install pytest paho-mqtt numpy scikit-learn joblib
```

### 运行完整测试套件
```bash
# 进入测试引擎目录
cd test_engine

# 运行所有测试
pytest tests/ -v

# 运行测试并生成报告
pytest tests/ -v --html=report.html

# 运行特定标签的测试
pytest tests/ -m "integration" -v
```

### 运行特定测试模块
```bash
# 运行集成测试
pytest tests/test_integration.py -v

# 运行传感器测试
pytest tests/test_sensor.py -v

# 运行云端服务测试
pytest tests/test_cloud_services.py -v

# 运行数据流测试
pytest tests/test_complete_data_flow.py -v
```

### 运行特定测试用例
```bash
# 运行单个测试类
pytest tests/test_integration.py::TestIoTIntegration -v

# 运行单个测试方法
pytest tests/test_sensor.py::TestSensorModule::test_sensor_data_validity -v

# 运行包含特定字符串的测试
pytest tests/ -k "sensor" -v
```

### 调试和开发
```bash
# 检查路径解析
python -c "
from utils.path_resolver import get_library_path, get_executable_path
print('动态库路径:', get_library_path())
print('可执行文件路径:', get_executable_path())
"

# 直接运行测试脚本（不通过pytest）
python tests/test_sensor.py
python tests/test_complete_data_flow.py
```

### CI/CD集成
```bash
# 在CI环境中跳过需要构建产物的测试
pytest tests/ -v --skip-ci

# 运行快速测试（跳过耗时测试）
pytest tests/ -v --quick

# 生成覆盖率报告
pytest tests/ --cov=IoT_EnvMonitorSys_Basic --cov-report=html
```

## 测试策略

### 1. 设备固件测试重点
- **数据合理性**：验证传感器数据在物理合理范围内
- **功能正确性**：测试动态库导出的C函数接口
- **连续性验证**：检测数据突变和时间序列断裂

### 2. 云端服务测试重点
- **AI分析准确性**：验证环境类型识别和建议生成
- **数据完整性**：测试数据库操作的原子性和一致性
- **服务可靠性**：验证异常处理和恢复机制

### 3. 系统集成测试重点
- **端到端数据流**：验证设备到云端的完整业务流程
- **跨语言兼容性**：测试C与Python的数据交换正确性
- **构建产物验证**：确保发布的二进制文件功能完整

## 技术特点

1. **分层测试策略**：单元 → 集成 → 系统测试的完整体系
2. **跨平台支持**：Windows/Linux测试环境兼容
3. **真实场景验证**：基于生产数据模式的测试用例
4. **自动化就绪**：完善的CI/CD集成支持

## 扩展方向

- 增加压力测试和负载测试
- 添加安全、性能测试用例
- 集成代码覆盖率分析
- 支持多设备并发测试场景