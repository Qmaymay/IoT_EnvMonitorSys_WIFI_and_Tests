import sys
import os

# 添加shared目录到Python路径
current_dir = os.path.dirname(os.path.abspath(__file__))
shared_dir = os.path.join(current_dir, '..', 'shared')
sys.path.insert(0, shared_dir)

from real_ai_analyzer import RealAIAnalyzer
from database import DatabaseManager


def main():
    db = DatabaseManager()
    ai_analyzer = RealAIAnalyzer()
    
    # 获取设备数据
    devices = db.get_all_devices()
    if not devices:
        print("❌ 无设备数据")
        return
    
    device_id = devices[1]
    recent_data = db.get_recent_data(device_id)
    
    if not recent_data:
        print("❌ 该设备无数据")
        return
    
    # AI分析
    latest = recent_data[0]
    result = ai_analyzer.analyze_with_ai(device_id, latest['temp'], latest['hum'], latest['air'])
    
    # 输出结果
    print(f"🎯 设备: {device_id}")
    print(f"📊 数据: {latest['temp']}°C, {latest['hum']}%, {latest['air']}%")
    print(f"🤖 分析: {result['environment_type']} (置信度: {result['prediction_confidence']}%)")
    print("💡 建议:")
    for suggestion in result['ai_suggestions']:
        print(f"  • {suggestion}")


if __name__ == "__main__":
    main()