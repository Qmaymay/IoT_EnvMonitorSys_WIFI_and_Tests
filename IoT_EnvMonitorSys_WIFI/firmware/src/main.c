/**
 * IoT环境监测系统 - 主程序
 * 平台: simulator
 * 构建时间: __DATE__ __TIME__
 */

#include <stdio.h>

int main(void)
{
    printf("========================================\n");
    printf("IoT环境监测系统\n");
    printf("平台: simulator\n");
    printf("========================================\n\n");
    
    printf("系统启动成功！\n");
    printf("等待传感器数据...\n");
    
    // 简单循环
    int count = 0;
    while (count < 5) {
        printf("运行中... %d\n", ++count);
        #ifdef PLATFORM_SIMULATOR
        for (int i = 0; i < 100000000; i++); // 简单延时
        #endif
    }
    
    printf("\n系统停止\n");
    return 0;
}
