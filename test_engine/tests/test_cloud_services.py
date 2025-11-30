# -*- coding: utf-8 -*-
"""
cloud-services核心功能测试
"""
import sys
import os
from pathlib import Path

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))
from IoT_EnvMonitorSys_Basic.cloud_services.ai_analyzer.real_ai_analyzer import RealAIAnalyzer


def test_ai_analyzer_integration():
    """测试AI分析器集成"""
    # from ai_analyzer.real_ai_analyzer import RealAIAnalyzer
    
    ai = RealAIAnalyzer()
    result = ai.analyze_with_ai("test_device", 25.5, 60.0, 85.0)
    
    assert "environment_type" in result
    assert "ai_suggestions" in result
    print("✅ AI分析器测试通过")


if __name__ == "__main__":
    print("开始运行cloud-services测试...")
    test_ai_analyzer_integration()
    print("所有测试完成！")
