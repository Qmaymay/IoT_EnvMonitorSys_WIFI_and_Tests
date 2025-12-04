import os
import sqlite3
from datetime import datetime


class DatabaseManager:
    def __init__(self, db_path=None):
        if db_path is None:
            # 获取当前脚本所在目录
            current_dir = os.path.dirname(os.path.abspath(__file__))
            self.db_path = os.path.join(current_dir, "sensor_data.db")
            print(f"🔄 使用默认路径: {self.db_path}")
        else:
            self.db_path = db_path
            self.init_database()
            print(f"🔄 使用传入路径: {self.db_path}")
        
        print(f"📍 最终数据库路径: {os.path.abspath(self.db_path)}")
        print(f"📍 文件存在: {os.path.exists(self.db_path)}")
    
    def init_database(self):
        """初始化两个表"""
        conn = sqlite3.connect(self.db_path)
        cursor = conn.cursor()
        
        # 保持现有的传感器表结构
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS sensor_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                device_id TEXT NOT NULL,
                temperature REAL,
                humidity REAL,
                air_quality REAL,
                timestamp INTEGER,
                received_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        ''')
        
        # 从sensor_database.py添加AI分析表
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS ai_analysis (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                device_id TEXT NOT NULL,
                environment_type TEXT NOT NULL,
                comfort_score REAL NOT NULL,
                health_risk TEXT,
                suggestions TEXT,
                analyzed_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        ''')
        
        conn.commit()
        conn.close()
    
    
    def save_sensor_data(self, data):
        """保存传感器数据到数据库"""
        conn = sqlite3.connect(self.db_path)
        cursor = conn.cursor()
        
        cursor.execute('''
            INSERT INTO sensor_data 
            (device_id, temperature, humidity, air_quality, timestamp)
            VALUES (?, ?, ?, ?, ?)
        ''', (
            data.get('device_id'),
            data.get('temp'),
            data.get('hum'), 
            data.get('air'),
            data.get('ts')
        ))
        
        conn.commit()
        conn.close()
        print(f"Data saved: {data['device_id']} at {datetime.now()}")
    
    def get_all_devices(self):
        """获取数据库中所有的设备ID"""
        conn = sqlite3.connect(self.db_path)
        cursor = conn.cursor()
        
        cursor.execute("SELECT DISTINCT device_id FROM sensor_data")
        devices = [row[0] for row in cursor.fetchall()]
        
        conn.close()
        return devices

    def get_recent_data(self, device_id: str, hours: int = 24):
        """获取最近的数据用于AI分析"""
        conn = sqlite3.connect(self.db_path)
        cursor = conn.cursor()
        
        # 🎯 修改：使用 id 排序而不是时间
        cursor.execute('''
            SELECT temperature, humidity, air_quality, timestamp 
            FROM sensor_data 
            WHERE device_id = ?
            ORDER BY id DESC
            LIMIT 50
        ''', (device_id,))
        
        data = cursor.fetchall()
        conn.close()

        return [{'temp': row[0], 'hum': row[1], 'air': row[2], 'ts': row[3]} for row in data]


# 测试数据库创建
if __name__ == "__main__":
    db = DatabaseManager()
    
    # 测试插入数据
    test_data = {
        'device_id': 'test_device_001',
        'temp': 25.5,
        'hum': 60.0,
        'air': 45.0,
        'ts': int(datetime.now().timestamp())
    }
    
    db.save_sensor_data(test_data)
    print("Test data inserted successfully!")
    
    # 测试查询数据
    recent_data = db.get_recent_data('test_device_001')
    print(f"Recent data: {recent_data}")

     # 🎯 新增：直接查看数据库所有数据
    print("\n=== 数据库所有数据 ===")
    conn = sqlite3.connect(db.db_path)
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM sensor_data")
    all_data = cursor.fetchall()
    
    for row in all_data:
        print(f"ID: {row[0]}, 设备: {row[1]}, 温度: {row[2]}, 湿度: {row[3]}, 空气质量: {row[4]}, 时间: {row[6]}")
    
    conn.close()
    
