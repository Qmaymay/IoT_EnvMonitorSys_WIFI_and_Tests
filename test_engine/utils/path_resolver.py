# -*- coding: utf-8 -*-
"""
路径解析工具 - 用于跨平台定位构建产物
"""
import platform
from pathlib import Path

class PathResolver:
    """构建产物路径解析器"""
    
    def __init__(self, project_root=None):
        self.project_root = project_root or Path(__file__).parent.parent.parent
        
    def _find_artifact(self, name):
        """查找构建产物"""
        # 搜索路径优先级
        search_paths = [
            self.project_root / "build" / "bin",           # 固定位置
            self.project_root / "build" / "bin" / "Debug", # Debug 版本
            self.project_root / "build" / "bin" / "Release", # Release 版本
        ]
        
        for path in search_paths:
            artifact_path = path / name
            if artifact_path.exists():
                return artifact_path
        
        return search_paths[0] / name
    
    def get_library_path(self):
        """获取动态库路径"""
        lib_name = "env_monitor.dll" if platform.system() == "Windows" else "libenv_monitor.so"
        return self._find_artifact(lib_name)
    
    def get_executable_path(self):
        """获取可执行文件路径"""
        exe_name = "env_monitor_app.exe" if platform.system() == "Windows" else "env_monitor_app"
        return self._find_artifact(exe_name)

# 创建全局实例
path_resolver = PathResolver()

# 便捷函数
def get_library_path():
    return path_resolver.get_library_path()

def get_executable_path():
    return path_resolver.get_executable_path()

if __name__ == "__main__":
    # 简单测试
    print(f"动态库: {get_library_path()} - {get_library_path().exists()}")
    print(f"可执行文件: {get_executable_path()} - {get_executable_path().exists()}")
    