# tests/conftest.py内容建议：
import pytest
import sys
import os

# 添加项目根目录到路径
sys.path.insert(0, os.path.dirname(os.path.dirname(__file__)))

# 共享的fixture
@pytest.fixture
def firmware_lib_path():
    """返回固件动态库路径"""
    return os.path.join("..", "..", "build", "bin", "libenv_monitor_shared.so")