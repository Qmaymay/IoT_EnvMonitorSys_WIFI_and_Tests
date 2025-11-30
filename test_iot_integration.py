import ctypes
import os
import sys



class IoTDeviceTester:
    def __init__(self):
        # 动态定位库文件
        project_root = os.path.dirname(os.path.abspath(__file__))
        lib_path = os.path.join(
            project_root, 
            "IoT_EnvMonitorSys_Basic", 
            "firmware", 
            "build", 
            "lib", 
            "env_monitor.dll"  # Windows
            # "libenv_monitor.so"  # Linux
            # "libenv_monitor.dylib"  # macOS
        )
        
        print(f"加载动态库: {lib_path}")
        self.lib = ctypes.CDLL(lib_path)
        
        # 定义函数原型
        self.lib.sensor_emulator_init.restype = ctypes.c_int
        self.lib.sensor_emulator_read.argtypes = [ctypes.c_void_p]
        self.lib.sensor_emulator_read.restype = ctypes.c_int
        
    def test_sensor_functions(self):
        """测试传感器函数"""
        print("=== 测试传感器模块 ===")
        
        # 初始化传感器
        result = self.lib.sensor_emulator_init()
        print(f"传感器初始化结果: {result}")
        
        # 测试读取传感器数据
        # 需要定义对应的数据结构...
        
    def test_mqtt_functions(self):
        """测试MQTT函数"""
        print("=== 测试MQTT模块 ===")
        # 测试MQTT相关函数...

if __name__ == "__main__":
    tester = IoTDeviceTester()
    tester.test_sensor_functions()