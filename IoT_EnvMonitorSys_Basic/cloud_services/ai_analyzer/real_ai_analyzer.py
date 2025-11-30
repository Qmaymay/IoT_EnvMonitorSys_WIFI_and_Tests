"""
AI分析器 - 使用机器学习模型
"""
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.cluster import KMeans
import joblib
import os

class RealAIAnalyzer:
    def __init__(self):
        # 获取当前文件所在目录
        current_dir = os.path.dirname(os.path.abspath(__file__))
        self.models_dir = os.path.join(current_dir, "ai_models")

        self.model_path = os.path.join(self.models_dir, "environment_model.pkl")
        self.cluster_path = os.path.join(self.models_dir, "cluster_model.pkl")
        
        # # 创建多级目录，且目录已存在时不报错
        os.makedirs(self.models_dir, exist_ok=True)
        
        # self.model_path = "ai_models/environment_model.pkl"
        # self.cluster_path = "ai_models/cluster_model.pkl"
        self.labels = ['舒适', '炎热', '寒冷', '潮湿', '干燥', '空气质量差']
        
        # 加载或训练模型
        self.model = self.load_or_train_model()
        self.cluster_model = self.load_or_train_cluster()
    
    def load_or_train_model(self):
        """加载或训练分类模型"""
        if os.path.exists(self.model_path):
            return joblib.load(self.model_path)
        else:
            # 生成训练数据（模拟真实环境数据）
            X, y = self.generate_training_data()
            model = RandomForestClassifier(n_estimators=100, random_state=42)
            model.fit(X, y)
            
            joblib.dump(model, self.model_path)
            return model
    
    def load_or_train_cluster(self):
        """加载或训练聚类模型（用于异常检测）"""
        if os.path.exists(self.cluster_path):
            return joblib.load(self.cluster_path)
        else:
            # 生成正常环境数据用于聚类
            X_normal = self.generate_normal_data()

            model = KMeans(n_clusters=3, random_state=42)
            model.fit(X_normal)
            
            joblib.dump(model, self.cluster_path)
            return model
    
    def generate_training_data(self):
        """生成训练数据"""
        np.random.seed(42)
        n_samples = 1000
        
        # 生成各种环境条件的数据
        X = []
        y = []
        
        # 舒适环境
        for _ in range(200):
            temp = np.random.normal(22, 2)
            hum = np.random.normal(55, 10)
            air = np.random.normal(85, 10)
            X.append([temp, hum, air])
            y.append(0)  # 舒适
        
        # 炎热环境
        for _ in range(200):
            temp = np.random.normal(32, 3)
            hum = np.random.normal(40, 15)
            air = np.random.normal(70, 15)
            X.append([temp, hum, air])
            y.append(1)  # 炎热
        
        # 寒冷环境
        for _ in range(200):
            temp = np.random.normal(5, 3)
            hum = np.random.normal(30, 10)
            air = np.random.normal(90, 5)
            X.append([temp, hum, air])
            y.append(2)  # 寒冷
            
        # 潮湿环境
        for _ in range(200):
            temp = np.random.normal(25, 3)
            hum = np.random.normal(85, 5)
            air = np.random.normal(60, 10)
            X.append([temp, hum, air])
            y.append(3)  # 潮湿
            
        # 干燥环境
        for _ in range(100):
            temp = np.random.normal(24, 3)
            hum = np.random.normal(20, 5)
            air = np.random.normal(80, 10)
            X.append([temp, hum, air])
            y.append(4)  # 干燥
            
        # 空气质量差
        for _ in range(100):
            temp = np.random.normal(23, 3)
            hum = np.random.normal(50, 10)
            air = np.random.normal(25, 10)
            X.append([temp, hum, air])
            y.append(5)  # 空气质量差
            
        return np.array(X), np.array(y)
    
    def generate_normal_data(self):
        """生成正常环境数据用于异常检测"""
        np.random.seed(42)
        n_samples = 500
        
        X = []
        for _ in range(n_samples):
            temp = np.random.normal(22, 5)    # 正常温度范围
            hum = np.random.normal(55, 15)    # 正常湿度范围
            air = np.random.normal(80, 15)    # 正常空气质量范围
            X.append([temp, hum, air])
            
        return np.array(X)
    
    def predict_environment(self, temp, hum, air):
        """使用AI模型预测环境类型"""
        features = np.array([[temp, hum, air]])
        prediction = self.model.predict(features)[0]
        probability = np.max(self.model.predict_proba(features))
        
        return self.labels[prediction], round(probability * 100, 1)
    
    def detect_anomaly(self, temp, hum, air):
        """使用聚类检测异常环境"""
        features = np.array([[temp, hum, air]])
        distance = np.min(np.linalg.norm(self.cluster_model.cluster_centers_ - features, axis=1))
        
        # 距离越大，异常可能性越高
        anomaly_score = min(distance / 10, 1.0)  # 归一化到0-1
        return round(anomaly_score * 100, 1)
    
    def analyze_with_ai(self, device_id, temp, hum, air):
        """使用真正的AI进行分析"""
        # 预测环境类型
        env_type, confidence = self.predict_environment(temp, hum, air)
        
        # 检测异常
        anomaly_score = self.detect_anomaly(temp, hum, air)
        
        # 生成智能建议
        suggestions = self.generate_ai_suggestions(env_type, anomaly_score, temp, hum, air)
        
        return {
            "environment_type": env_type,
            "prediction_confidence": f"{confidence}%",
            "anomaly_score": f"{anomaly_score}%",
            "ai_suggestions": suggestions,
            "model_used": "RandomForest + KMeans"
        }
    
    def generate_ai_suggestions(self, env_type, anomaly_score, temp, hum, air):
        """基于AI结果生成建议"""
        suggestions = []
        
        if anomaly_score > 70:
            suggestions.append("⚠️ 环境异常！建议立即检查设备")
        
        if env_type == "炎热":
            if temp > 35:
                suggestions.append("🔥 极端高温！建议开启空调并补充水分")
            else:
                suggestions.append("🌡️ 温度较高，建议适当降温")
                
        elif env_type == "寒冷":
            suggestions.append("❄️ 温度较低，建议保暖")
            
        elif env_type == "潮湿":
            suggestions.append("💧 湿度较高，建议使用除湿器")
            
        elif env_type == "干燥":
            suggestions.append("🏜️ 空气干燥，建议使用加湿器")
            
        elif env_type == "空气质量差":
            suggestions.append("🌫️ 空气质量差，建议开启空气净化器")
        
        # 基于具体数值的精确建议
        if air < 50:
            suggestions.append(f"🫁 空气质量({air})较差，注意通风")
        if hum < 30:
            suggestions.append(f"🏜️ 湿度({hum}%)过低，建议增加湿度")
        if temp > 28:
            suggestions.append(f"🌡️ 温度({temp}°C)偏高，适当降温")
            
        return suggestions if suggestions else ["✅ 环境舒适，保持现状"]
    