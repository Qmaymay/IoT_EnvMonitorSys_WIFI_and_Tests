#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
传感器模块完整测试 - 验证数据合理性和连续性
"""
import pytest
import ctypes
import os, sys
import time
import statistics
from pathlib import Path

# 添加项目根目录
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))
from test_engine.utils.path_resolver import get_library_path


class TestSensorModule:
    """传感器模块完整测试"""
    
    def setup_class(self):
        """初始化测试环境"""
        self.dll_path = get_library_path()
        
        if not self.dll_path.exists():
            pytest.skip("动态库文件不存在，跳过传感器测试")
            
        # 加载传感器库
        try:
            self.lib = ctypes.CDLL(str(self.dll_path))
                              
            # 定义函数原型（根据你的C代码实际函数）
            self.lib.sensor_get_temperature.restype = ctypes.c_float
            self.lib.sensor_get_humidity.restype = ctypes.c_float
            self.lib.sensor_get_air_quality.restype = ctypes.c_float
            
        
        except Exception as e:
            pytest.skip(f"传感器库加载失败: {e}")
        
        # 测试配置
        self.sample_count = 50  # 采样次数
        self.sample_interval = 0.1  # 采样间隔(秒)
        
        # 合理值范围（根据实际传感器规格）
        self.valid_ranges = {
            'temperature': (-10.0, 50.0),      # 温度合理范围
            'humidity': (0.0, 100.0),          # 湿度合理范围
            'air_quality': (0.0, 500.0)        # 空气质量合理范围
        }


    def test_sensor_data_validity(self):
        """测试传感器数据合理性"""
        print("🧪 测试传感器数据合理性...")
        
        invalid_readings = 0
        total_readings = self.sample_count * 3
        
        for i in range(self.sample_count):
            # 直接调用C动态库函数（正确）
            temp = self.lib.sensor_get_temperature()
            hum = self.lib.sensor_get_humidity()
            air = self.lib.sensor_get_air_quality()

            # 验证数据在合理范围内
            if not (self.valid_ranges['temperature'][0] <= temp <= self.valid_ranges['temperature'][1]):
                invalid_readings += 1
                print(f"⚠️ 异常温度读数: {temp}°C")
                
            if not (self.valid_ranges['humidity'][0] <= hum <= self.valid_ranges['humidity'][1]):
                invalid_readings += 1
                print(f"⚠️ 异常湿度读数: {hum}%")
                
            if not (self.valid_ranges['air_quality'][0] <= air <= self.valid_ranges['air_quality'][1]):
                invalid_readings += 1
                print(f"⚠️ 异常空气质量读数: {air}")
            
            time.sleep(self.sample_interval)
        
        # 允许少量异常读数（传感器噪声）
        anomaly_ratio = invalid_readings / total_readings
        assert anomaly_ratio < 0.05, f"异常读数比例过高: {anomaly_ratio:.1%}"
        print(f"✅ 数据合理性测试通过 - 异常率: {anomaly_ratio:.1%}")


    def test_sensor_data_continuity(self):
        """测试传感器数据连续性 - 检测时间断裂和突变"""
        print("⏱️ 测试传感器数据连续性...")
        
        timestamps = []
        temperatures = []
        humidities = []
        air_qualities = []
        
        # 收集数据和时间戳
        for i in range(self.sample_count):
            current_time = time.time()
            
            # 读取真实传感器数据
            temp = self.lib.sensor_get_temperature()
            hum = self.lib.sensor_get_humidity()
            air = self.lib.sensor_get_air_quality()
            
            timestamps.append(current_time)
            temperatures.append(temp)
            humidities.append(hum)
            air_qualities.append(air)
            
            time.sleep(self.sample_interval)
        
        # 1. 时间断裂检测
        time_diffs = [timestamps[i] - timestamps[i-1] for i in range(1, len(timestamps))]
        
        # 找出所有超过阈值的时间间隔
        time_breaks = []
        for i, diff in enumerate(time_diffs):
            if diff > self.sample_interval * 1.5:  # 允许50%的误差
                time_breaks.append({
                    'index': i,
                    'actual_interval': diff,
                    'expected_interval': self.sample_interval,
                    'start_time': timestamps[i],
                    'end_time': timestamps[i+1]
                })
        
        # 报告时间断裂
        if time_breaks:
            print(f"⚠️ 检测到 {len(time_breaks)} 处时间断裂:")
            for break_info in time_breaks:
                delay = break_info['actual_interval'] - break_info['expected_interval']
                print(f"   第{break_info['index']+1}次采样延迟 {delay:.3f}s "
                    f"(实际: {break_info['actual_interval']:.3f}s, 预期: {break_info['expected_interval']:.3f}s)")
        
        # 2. 数据突变检测
        max_temp_change = 0
        max_hum_change = 0
        max_air_change = 0
        
        temp_mutations = []
        hum_mutations = []
        air_mutations = []
        
        for i in range(1, len(temperatures)):
            # 温度突变检测
            temp_change = abs(temperatures[i] - temperatures[i-1])
            max_temp_change = max(max_temp_change, temp_change)
            if temp_change > 5.0:  # 温度突变阈值
                temp_mutations.append({
                    'index': i,
                    'change': temp_change,
                    'from': temperatures[i-1],
                    'to': temperatures[i]
                })
            
            # 湿度突变检测
            hum_change = abs(humidities[i] - humidities[i-1])
            max_hum_change = max(max_hum_change, hum_change)
            if hum_change > 20.0:  # 湿度突变阈值
                hum_mutations.append({
                    'index': i,
                    'change': hum_change,
                    'from': humidities[i-1],
                    'to': humidities[i]
                })
            
            # 空气质量突变检测
            air_change = abs(air_qualities[i] - air_qualities[i-1])
            max_air_change = max(max_air_change, air_change)
            if air_change > 100.0:  # 空气质量突变阈值
                air_mutations.append({
                    'index': i,
                    'change': air_change,
                    'from': air_qualities[i-1],
                    'to': air_qualities[i]
                })
        
        # 报告数据突变
        all_mutations = temp_mutations + hum_mutations + air_mutations
        if all_mutations:
            print(f"⚠️ 检测到 {len(all_mutations)} 处数据突变:")
            for mutation in all_mutations[:5]:  # 只显示前5个避免输出过多
                print(f"   第{mutation['index']+1}次采样变化: {mutation['change']:.1f}")
        
        # 3. 断言检查
        # 时间连续性断言
        assert len(time_breaks) < 3, f"时间断裂过多: {len(time_breaks)} 处"
        
        # 数据突变断言
        assert max_temp_change < 10.0, f"温度突变过大: {max_temp_change:.1f}°C"
        assert max_hum_change < 30.0, f"湿度突变过大: {max_hum_change:.1f}%"
        assert max_air_change < 200.0, f"空气质量突变过大: {max_air_change:.1f}"
        
        # 4. 输出统计信息
        avg_interval = statistics.mean(time_diffs) if time_diffs else 0
        max_interval = max(time_diffs) if time_diffs else 0
        
        print(f"📊 采样统计:")
        print(f"   平均间隔: {avg_interval:.3f}s, 最大间隔: {max_interval:.3f}s")
        print(f"   时间断裂: {len(time_breaks)} 处")
        print(f"   最大变化 - 温度: {max_temp_change:.2f}°C, 湿度: {max_hum_change:.2f}%, 空气质量: {max_air_change:.2f}")
        
        print("✅ 数据连续性测试通过")

    

if __name__ == "__main__":
    # 直接运行测试
    test = TestSensorModule()
    test.setup_class()
    
    print("🚀 开始传感器完整测试...")
    try:
        print("✅ 传感器库加载成功")

        test.test_sensor_data_validity()
        test.test_sensor_data_continuity()

        print("🎉 所有传感器测试通过！")
    except Exception as e:
        print(f"❌ 传感器测试失败: {e}")
        raise

