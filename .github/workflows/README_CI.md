# 需要
被测工程有改动，提交时触发rrimware.yml，重要功能完成手动触发ci.yml
测试工程有改动，提交时也触发ci.yml


# CI/CD 流程

## 三个独立CI

### 1. 固件CI (`firmware.yml`)
- **触发**：固件代码修改时
- **任务**：编译固件、运行固件单元测试
- **开发者**：固件工程师

### 2. 云端CI (`cloud.yml`)  
- **触发**：云端代码修改时
- **任务**：运行云端测试、构建打包
- **开发者**：后端工程师

### 3. 集成测试CI (`integration.yml`)
- **触发**：
  - 主分支重要提交
  - 每天凌晨2点（定时）
  - 手动触发
- **任务**：使用测试框架进行端到端测试
- **测试范围**：
  - `full`：完整测试套件
  - `quick`：快速冒烟测试
  - `firmware-e2e`：固件端到端测试
  - `cloud-e2e`：云端端到端测试

## 工作流程

### 开发阶段
- 改固件 → 触发固件CI（快速反馈）
- 改云端 → 触发云端CI（快速反馈）

### 集成阶段  
- 重要功能完成 → 手动触发集成测试
- 每日定时 → 自动运行完整测试
- 发布前 → 运行完整测试套件

## 目录结构
```
test_engine/tests/
├── performance      # 性能测试
├── reliability      # 稳定性测试
└── cecurity         # 安全测试
└── smoke/           # 端到端测试
├── system/firmware/ # 固件集成测试
├── system/cloud/    # 云端集成测试s
└── system/e2e/      # 端到端测试
```

### 本地运行
```bash
# 安装测试框架
cd test_engine
pip install -e .

# 运行特定测试
python -m pytest tests/system/firmware/ -v
python -m pytest tests/system/e2e/ -v
```

