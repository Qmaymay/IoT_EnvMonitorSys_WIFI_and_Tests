# check_dll_functions.py
import ctypes
import os

def check_dll_functions():
    """检查动态库中实际可用的函数"""
    dll_path = r"E:\C_language_learning\IoT_EnvMonitorSys_Basic_and_Tests\IoT_EnvMonitorSys_Basic\firmware\build\bin\Release\env_monitor.dll"
    
    if not os.path.exists(dll_path):
        print(f"❌ 动态库文件不存在: {dll_path}")
        return
    
    print(f"✅ 动态库文件存在: {dll_path}")
    
    try:
        # 加载动态库
        lib = ctypes.CDLL(dll_path)
        print("✅ 动态库加载成功")
        
        # 获取所有导出函数
        print("\n📋 动态库导出函数:")
        function_count = 0
        
        for attr_name in dir(lib):
            if not attr_name.startswith('_') and attr_name not in ['_FuncPtr', '_handle', '_name']:
                print(f"  {attr_name}")
                function_count += 1
        
        print(f"\n总共找到 {function_count} 个函数")
        
        # 如果没有找到预期的函数，可能是C++名称修饰
        if function_count == 0:
            print("⚠️  没有找到明显的导出函数，可能是C++名称修饰")
            print("尝试查看原始导出表...")
            
    except Exception as e:
        print(f"❌ 加载动态库失败: {e}")

if __name__ == "__main__":
    check_dll_functions()