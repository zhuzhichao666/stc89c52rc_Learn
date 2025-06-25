#include <reg52.h>
#include <stdio.h>
#include <math.h>

// LCD控制引脚定义
sbit RS = P2^0;      // 寄存器选择信号，RS=0选择指令寄存器，RS=1选择数据寄存器
sbit RW = P2^1;      // 读写控制信号，RW=0写操作，RW=1读操作
sbit EN = P2^2;      // 使能信号，下降沿触发数据传输

// ADC0804控制引脚定义
sbit ADC_CS = P2^3;  // 片选信号，低电平有效
sbit ADC_RD = P2^4;  // 读数据信号，低电平有效
sbit ADC_WR = P2^5;  // 启动转换信号，低电平有效
sbit ADC_INTR = P2^6;// 转换完成中断信号，低电平表示转换完成
#define ADC_DATA P1  

// 毫秒级延时函数
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++); 
}

// LCD控制相关函数
// 触发LCD使能信号，产生下降沿
void LCD_Enable() {
    EN = 1;           // 使能信号置高
    delay_ms(1);      // 延时等待信号稳定
    EN = 0;           // 使能信号置低，产生下降沿
}

// 向LCD发送命令
void LCD_SendCmd(unsigned char cmd) {
    RS = 0;           // 选择指令寄存器
    RW = 0;           // 设置为写操作
    P0 = cmd;         // 将命令字送到数据端口
    LCD_Enable();     // 触发使能信号，发送命令
    delay_ms(2);      // 等待LCD执行命令
}

// 向LCD发送数据
void LCD_SendData(unsigned char dat) {
    RS = 1;           // 选择数据寄存器
    RW = 0;           // 设置为写操作
    P0 = dat;         // 将数据送到数据端口
    LCD_Enable();     // 触发使能信号，发送数据
    delay_ms(2);      // 等待LCD显示数据
}

// 初始化LCD
void LCD_Init() {
    LCD_SendCmd(0x38); // 设置8位数据总线，2行显示，5x7点阵
    LCD_SendCmd(0x0C); // 开显示，关光标
    LCD_SendCmd(0x06); // 文字不动，地址自动加1
    LCD_SendCmd(0x01); // 清屏
    delay_ms(10);      // 等待清屏完成
}

// 在指定位置显示字符串
void LCD_ShowString(unsigned char x, unsigned char y, char *str) {
    unsigned char addr = (y == 0) ? 0x80 + x : 0xC0 + x; // 计算显示地址
    LCD_SendCmd(addr);        // 设置显示起始地址
    while (*str) {            // 循环显示每个字符
        LCD_SendData(*str++); // 发送字符并指向下一个字符
    }
}

// 从ADC0804读取数据
unsigned char ADC_Read() {
    unsigned char value;
    
    // 启动ADC转换
    ADC_CS = 0;         // 使能ADC
    ADC_WR = 0;         // 启动转换信号置低
    delay_ms(1);        // 延时等待
    ADC_WR = 1;         // 启动转换信号置高
    
    // 等待转换完成
    while (ADC_INTR);   // 等待INTR引脚变为低电平，表示转换完成
    
    // 读取转换结果
    ADC_RD = 0;         // 读信号置低
    value = ADC_DATA;   // 读取数据
    ADC_RD = 1;         // 读信号置高
    ADC_CS = 1;         // 禁用ADC
    
    return value;       // 返回读取的ADC值
}

// 将ADC值转换为温度值(基于PT100传感器)
float get_temperature(unsigned char adc_val) {
    // 1. 将ADC值(0-255)转换为对应的电压值(0-5V)
    float Vadc = adc_val * (5.0 / 255.0); // ADC输出电压
    
    // 2. 计算PT100传感器上的实际电压(假设电路中有20倍放大)
    float Vpt100 = Vadc / 20.0;           // PT100两端电压
    
    // 3. 根据欧姆定律计算PT100的电阻值(假设恒流源为2mA)
    float Rpt100 = Vpt100 / 0.002;        // PT100电阻值
    
    // 4. 根据PT100的温度特性计算温度(简化公式，T=(R-R0)/α)
    //    PT100在0℃时电阻为100Ω，温度系数α=0.385Ω/℃
    float T = (Rpt100 - 100.0) / 0.385;   // 计算温度值
    
    return T;
}

// 主函数
void main() {
    char str[16];         // 用于格式化温度显示的字符串缓冲区
    float temp;           // 存储温度值
    unsigned char adc_val;// 存储ADC采集值

    LCD_Init();           // 初始化LCD
    // 在LCD第一行显示学号
    LCD_ShowString(0, 0, "xuehao:23112123"); 

    // 进入无限循环，持续采集和显示温度
    while (1) {
        adc_val = ADC_Read();      // 读取ADC值
        temp = get_temperature(adc_val); // 计算温度值

        // 格式化温度显示字符串，保留一位小数
        sprintf(str, "T=%.1f C", temp);
        // 在LCD第二行显示温度值
        LCD_ShowString(0, 1, str);

        delay_ms(500);             // 延时500ms，控制更新频率
    }
}