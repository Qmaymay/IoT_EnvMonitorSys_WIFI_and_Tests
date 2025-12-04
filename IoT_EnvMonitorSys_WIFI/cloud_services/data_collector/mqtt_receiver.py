import os, sys
import json
import paho.mqtt.client as mqtt

current_dir = os.path.dirname(os.path.abspath(__file__))
shared_dir = os.path.join(current_dir, '..', 'shared')
sys.path.insert(0, shared_dir)
from database import DatabaseManager

# 配置信息
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "devices/+/sensor_data"

class DataCollector:
    def __init__(self):
        # 添加调试信息
        current_dir = os.path.dirname(os.path.abspath(__file__))
        shared_dir = os.path.join(current_dir, '..', 'shared')
        db_path = os.path.join(shared_dir, "sensor_data.db")
        
        print(f"📁 接收器使用数据库: {db_path}")
        print(f"📁 数据库存在: {os.path.exists(db_path)}")
        
        self.db = DatabaseManager(db_path=db_path)  # 明确指定路径
        # self.db = DatabaseManager()
        self.mqtt_client = mqtt.Client()
        
        # 设置MQTT回调函数
        self.mqtt_client.on_connect = self.on_connect
        self.mqtt_client.on_message = self.on_message
        
    def on_connect(self, client, userdata, flags, rc):
        """MQTT连接成功回调"""
        if rc == 0:
            print("✅ Connected to MQTT broker successfully!")
            # 订阅设备数据主题
            client.subscribe(MQTT_TOPIC)
            print(f"Subscribed to topic: {MQTT_TOPIC}")
        else:
            print(f"Failed to connect, return code {rc}")
    
    def on_message(self, client, userdata, msg):
        try:
            raw = msg.payload.decode('utf-8')
            print(f"📨 收到: {raw}")
            
            try:
                # 第一步：先尝试直接解析JSON
                data = json.loads(raw)  # 如果是正常JSON，这里就成功了
                print("✅ 正常JSON解析成功")
            except json.JSONDecodeError:
                # 第二步：如果JSON解析失败，说明需要修复
                print("🛠️ 检测到非标准JSON，开始修复...")
                fixed = raw.replace('{', '{"').replace(':', '":"').replace(',', '","').replace('}', '"}')
                fixed = fixed.replace('"","', '","').replace('":"{', ':{')
                data = json.loads(fixed)  # 修复后再解析
                print("✅ 修复后JSON解析成功")
            
            self.db.save_sensor_data(data)
            print("💾 保存成功!")
        
        except Exception as e:
            print(f"❌ 失败: {e}")
        
        
    def start(self):
        """启动数据收集服务"""
        print(" Starting IoT Data Collector...")
        print(f" MQTT Broker: {MQTT_BROKER}:{MQTT_PORT}")
        
        try:
            # 连接MQTT代理
            self.mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
            
            # 启动网络循环（阻塞调用）
            print(" Starting network loop...")
            self.mqtt_client.loop_forever()
            
        except KeyboardInterrupt:
            print("\n Shutting down data collector...")
            self.mqtt_client.disconnect()
        except Exception as e:
            print(f"❌ Unexpected error: {e}")


if __name__ == "__main__":
    collector = DataCollector()
    collector.start()

    