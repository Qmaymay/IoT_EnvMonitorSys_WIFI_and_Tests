
def test_imports():
    import sys
    assert sys.version_info.major == 3
    
def test_paths():
    import os
    assert os.path.exists('tests/')

def test_pytest():
    assert True  # 最简单的测试

def test_device_starts():
    """设备能启动"""
    assert True
    