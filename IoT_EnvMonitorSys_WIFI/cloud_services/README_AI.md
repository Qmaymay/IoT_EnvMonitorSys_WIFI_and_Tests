# 云端服务模块

## 模块概述

云端服务模块负责接收、存储和分析设备固件发送的环境监测数据，提供数据持久化和智能分析能力。

## 设计意图

- **数据汇聚中心**：统一接收和管理多个设备的数据
- **智能分析引擎**：对环境数据进行机器学习分析
- **服务解耦**：与设备固件松耦合，支持独立部署和扩展
- **数据持久化**：可靠存储历史数据供查询和分析

## 目录结构

```
cloud_services/
├── ai_analyzer/           # AI分析服务
│   ├── real_ai_analyzer.py    # 核心分析引擎
│   ├── ai_models/             # 预训练模型
│   └── main.py               # 服务入口
├── data_collector/         # 数据收集服务
│   ├── mqtt_receiver.py       # MQTT数据接收器
│   └── path_debug.py         # 路径调试工具
└── shared/                 # 共享组件
    ├── database.py            # 数据库管理
    └── sensor_data.db        # 数据存储文件
```

## 核心组件

### 1. 数据收集服务 (`data_collector/`)

**关键技术**：
- MQTT协议订阅机制
- JSON数据解析和容错处理
- 实时数据流处理

**主要文件**：
- `mqtt_receiver.py`：MQTT消息接收和数据处理主程序
- `path_debug.py`：模块导入路径调试工具

### 2. AI分析服务 (`ai_analyzer/`)

**关键技术**：
- scikit-learn机器学习库
- 环境数据聚类分析
- 预训练模型加载和推理

**主要文件**：
- `real_ai_analyzer.py`：AI分析核心逻辑
- `ai_models/`：存储训练好的机器学习模型
- `main.py`：AI服务启动入口

### 3. 共享组件 (`shared/`)

**关键技术**：
- SQLite数据库操作
- 数据模型抽象
- 跨模块数据访问接口

**主要文件**：
- `database.py`：统一的数据库管理类
- `sensor_data.db`：SQLite数据库文件

## 模块关系

### 内部依赖关系
```
data_collector/mqtt_receiver.py 
    → shared/database.py (数据存储)
    
ai_analyzer/real_ai_analyzer.py
    → shared/database.py (数据读取)
    → ai_models/ (模型加载)
```

### 外部依赖关系
```
设备固件模块
    ↓ (MQTT消息)
data_collector/mqtt_receiver.py
    ↓ (数据库存储)  
shared/sensor_data.db
    ↑ (数据读取)
ai_analyzer/real_ai_analyzer.py
```

## 核心功能

### 数据流处理
```python
# MQTT消息 → 数据解析 → 数据库存储 → AI分析
MQTT消息接收 → JSON解析 → 数据验证 → SQLite存储 → 机器学习分析 → 结果输出
```

### 数据库设计
- `sensor_data`表：存储原始传感器数据
- `ai_analysis`表：存储AI分析结果
- 支持设备ID区分和多维度查询

### AI分析能力
- 环境类型识别（舒适、炎热、潮湿等）
- 健康风险评估
- 个性化环境改善建议生成

## 运行命令

### 环境准备
```bash
# 安装Python依赖
pip install paho-mqtt scikit-learn joblib numpy

# 安装MQTT Broker (选择一种方式)
# Windows: 下载安装 Mosquitto
# Linux: sudo apt-get install mosquitto mosquitto-clients
```

### 启动数据收集服务
```bash
# 进入项目根目录
cd IoT_EnvMonitorSys_Basic_and_Tests

# 启动MQTT数据接收器
python IoT_EnvMonitorSys_Basic/cloud_services/data_collector/mqtt_receiver.py

# 或者直接进入目录启动
cd IoT_EnvMonitorSys_Basic/cloud_services/data_collector
python mqtt_receiver.py
```

### 运行AI分析服务
```bash
# 运行AI分析演示
cd IoT_EnvMonitorSys_Basic/cloud_services/ai_analyzer
python main.py

# 或者直接使用AI分析器
python -c "
from real_ai_analyzer import RealAIAnalyzer
ai = RealAIAnalyzer()
result = ai.analyze_with_ai('test_device', 25.5, 60.0, 85.0)
print('分析结果:', result)
"
```

### 数据库管理
```bash
# 查看数据库内容
sqlite3 IoT_EnvMonitorSys_Basic/cloud_services/shared/sensor_data.db

# SQLite命令示例
sqlite> .tables                    # 查看所有表
sqlite> SELECT * FROM sensor_data; # 查看传感器数据
sqlite> .exit                      # 退出
```

### 测试云端服务
```bash
# 运行云端服务测试
cd test_engine
pytest tests/test_cloud_services.py -v
pytest tests/test_complete_data_flow.py -v
```

### 调试和验证
```bash
# 检查模块导入路径
cd IoT_EnvMonitorSys_Basic/cloud_services/data_collector
python path_debug.py

# 测试数据库连接
python -c "
from shared.database import DatabaseManager
db = DatabaseManager()
print('数据库路径:', db.db_path)
print('数据库连接成功')
"
```

## 完整工作流程

1. **启动MQTT Broker**
2. **启动数据收集服务**
   ```bash
   python data_collector/mqtt_receiver.py
   ```
3. **运行设备固件**（在另一个终端）
   ```bash
   ./build/bin/Release/env_monitor_app
   ```
4. **验证数据接收**
   ```bash
   sqlite3 shared/sensor_data.db "SELECT * FROM sensor_data;"
   ```
5. **运行AI分析**
   ```bash
   python ai_analyzer/main.py
   ```

## 技术特点

1. **模块化设计**：各服务可独立运行和测试
2. **容错处理**：MQTT消息解析具备错误恢复能力
3. **模型持久化**：预训练模型避免每次重新训练
4. **数据完整性**：数据库事务保证数据一致性

## 扩展方向

- 支持更多数据源接入
- 增加实时数据看板
- 集成更多机器学习算法
- 添加数据导出和报表功能