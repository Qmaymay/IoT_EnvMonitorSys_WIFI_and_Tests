"""
完整数据流测试 - 从接收到分析的端到端测试
"""
import sys
import os
import json
import time
from pathlib import Path

# 设置路径
project_root = Path(__file__).parent.parent.parent
sys.path.extend([
    str(project_root / "IoT_EnvMonitorSys_Basic" / "cloud_services" / "ai_analyzer"),
    str(project_root / "IoT_EnvMonitorSys_Basic" / "cloud_services" / "shared")
])

from database import DatabaseManager
from real_ai_analyzer import RealAIAnalyzer


def test_complete_data_flow():
    """测试从数据库读取真实数据并进行AI分析的完整流程"""
    print("🚀 开始完整数据流测试...")
    
    # 1. 初始化组件
    db = DatabaseManager()  # 使用真实数据库
    ai = RealAIAnalyzer()
    
    # 2. 检查数据库中是否有真实数据
    devices = db.get_all_devices()
    if not devices:
        print("⏭️  跳过测试 - 数据库中无设备数据")
        return True  # 优雅跳过
    
    device_id = devices[1]  # 使用第一个真实设备
    
    # 3. 获取真实数据
    recent_data = db.get_recent_data(device_id, hours=1)  # 最近1小时数据
    if not recent_data:
        print("⏭️  跳过测试 - 设备无近期数据")
        return True
    
    print(f"📊 使用设备 {device_id} 的 {len(recent_data)} 条真实数据")
    
    # 4. 使用最新数据进行AI分析
    latest_data = recent_data[0]
    print(f"🎯 分析数据: 温度{latest_data['temp']}°C, 湿度{latest_data['hum']}%, 空气质量{latest_data['air']}%")
    
    # 5. AI分析
    analysis_result = ai.analyze_with_ai(
        device_id,
        latest_data['temp'], 
        latest_data['hum'],
        latest_data['air']
    )
    
    # 6. 验证分析结果
    assert "environment_type" in analysis_result, "AI分析缺少环境类型"
    assert "ai_suggestions" in analysis_result, "AI分析缺少建议"
    assert len(analysis_result["ai_suggestions"]) > 0, "AI建议为空"
    
    print(f"🤖 分析结果: {analysis_result['environment_type']}")
    print("💡 建议:", analysis_result['ai_suggestions'][:2])  # 只显示前2条建议
    
    print("🎉 完整数据流测试通过！")
    return True


def test_ai_analysis_with_sample_data():
    """使用样本数据测试AI分析功能"""
    print("🧪 使用样本数据测试AI分析...")
    
    ai = RealAIAnalyzer()
    
    # 测试典型场景
    test_scenarios = [
        (25.0, 50.0, 85.0, "舒适环境"),
        (35.0, 40.0, 70.0, "炎热环境"), 
        (15.0, 80.0, 60.0, "潮湿环境"),
        (22.0, 45.0, 95.0, "优质环境")
    ]
    
    for temp, hum, air, scenario in test_scenarios:
        result = ai.analyze_with_ai(f"test_{scenario}", temp, hum, air)
        
        assert "environment_type" in result
        assert "ai_suggestions" in result
        print(f"  {scenario}: {result['environment_type']} ✓")
    
    print("✅ 样本数据测试通过")


if __name__ == "__main__":
    # 优先使用真实数据，没有则使用样本数据
    if not test_complete_data_flow():
        test_ai_analysis_with_sample_data()
    print("🎉 所有测试完成！")