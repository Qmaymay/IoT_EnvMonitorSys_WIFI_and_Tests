# path_debug.py - 放在data-collector目录下测试
import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
shared_dir = os.path.join(current_dir, '..', 'shared')

print("调试信息:")
print(f"当前文件: {__file__}")
print(f"当前目录: {current_dir}")
print(f"Shared目录: {shared_dir}")
print(f"Shared存在: {os.path.exists(shared_dir)}")

if os.path.exists(shared_dir):
    print("Shared目录内容:", os.listdir(shared_dir))

sys.path.insert(0, shared_dir)

try:
    from database import DatabaseManager
    print("✅ 导入成功!")
except ImportError as e:
    print(f"❌ 导入失败: {e}")