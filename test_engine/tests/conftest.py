# test_engine/tests/system/cloud/conftest.py
import pytest
import sys
import os
import tempfile
import json
from unittest.mock import Mock, MagicMock, patch

# 添加cloud_services到Python路径
# sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..', '..', 'IoT_EnvMonitorSys_WIFI', 'cloud_services'))

# 获取项目根目录
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DATA_COLLECTOR_PATH = os.path.join(PROJECT_ROOT, 'IoT_EnvMonitorSys_WIFI', 'cloud_services', 'data_collector')
SHARED_PATH = os.path.join(PROJECT_ROOT, 'IoT_EnvMonitorSys_WIFI', 'cloud_services', 'shared')

# 添加路径
for path in [DATA_COLLECTOR_PATH, SHARED_PATH]:
    if path not in sys.path:
        sys.path.insert(0, path)
        print(f"✅ 添加路径: {path}")
    
@pytest.fixture
def mock_database():
    """模拟数据库管理器"""
    with patch('mqtt_receiver.DatabaseManager') as mock_db:
        mock_instance = MagicMock()
        mock_instance.save_sensor_data = Mock()
        mock_db.return_value = mock_instance
        yield mock_instance

@pytest.fixture
def mock_mqtt_client():
    """模拟MQTT客户端"""
    with patch('mqtt_receiver.mqtt.Client') as mock_mqtt:
        mock_instance = MagicMock()
        mock_instance.on_connect = None
        mock_instance.on_message = None
        mock_instance.connect = Mock()
        mock_instance.subscribe = Mock()
        mock_instance.loop_forever = Mock()
        mock_instance.disconnect = Mock()
        mock_mqtt.return_value = mock_instance
        yield mock_instance

@pytest.fixture
def sample_sensor_data():
    """示例传感器数据"""
    return {
        "device_id": "esp32_001",
        "temperature": 25.5,
        "humidity": 60.0,
        "air_quality": 120.0,
        "timestamp": 1640995200,
        "battery": 3.7
    }

@pytest.fixture
def broken_json_data():
    """损坏的JSON数据（模拟固件发送的格式）"""
    return '{device_id:"esp32_001",temperature:25.5,humidity:60.0}'

@pytest.fixture
def temp_db_file():
    """临时数据库文件"""
    with tempfile.NamedTemporaryFile(suffix='.db', delete=False) as f:
        db_path = f.name
    yield db_path
    # 清理
    if os.path.exists(db_path):
        os.unlink(db_path)