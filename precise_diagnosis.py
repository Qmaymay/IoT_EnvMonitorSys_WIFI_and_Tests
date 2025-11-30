# precise_diagnosis.py
import os
import sqlite3

def precise_diagnosis():
    # 精确检查分析代码使用的路径
    analyzed_path = r"E:\C_language_learning\IoT_EnvMonitorSys_Basic_and_Tests\IoT_EnvMonitorSys_Basic\cloud_services\ai_analyzer\..\shared\sensor_data.db"
    resolved_path = os.path.abspath(analyzed_path)
    
    print("=== 精确路径诊断 ===")
    print(f"🎯 分析代码使用的路径: {analyzed_path}")
    print(f"🎯 解析后的实际路径: {resolved_path}")
    print(f"🎯 文件是否存在: {os.path.exists(resolved_path)}")
    
    if os.path.exists(resolved_path):
        print(f"📁 文件大小: {os.path.getsize(resolved_path)} bytes")
        
        # 检查数据库内容
        try:
            conn = sqlite3.connect(resolved_path)
            cursor = conn.cursor()
            cursor.execute("SELECT COUNT(*) FROM sensor_data")
            count = cursor.fetchone()[0]
            print(f"📊 实际数据条数: {count}")
            conn.close()
        except Exception as e:
            print(f"❌ 数据库错误: {e}")
    else:
        print("❌ 数据库文件不存在！")
        print("🔍 在附近目录查找数据库文件...")
        
        # 在父目录中查找
        parent_dir = os.path.dirname(resolved_path)
        for file in os.listdir(parent_dir):
            if file.endswith('.db'):
                full_path = os.path.join(parent_dir, file)
                print(f"📁 找到数据库: {full_path}")

if __name__ == "__main__":
    precise_diagnosis()