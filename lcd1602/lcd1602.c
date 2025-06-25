#include <reg52.h>
#include <intrins.h>

// 定义LCD1602引脚
sbit RS = P1^2;      // 寄存器选择信号
sbit RW = P1^1;      // 读写控制信号
sbit EN = P1^0;      // 使能信号
#define LCD_DATA P2  // 数据端口

// 定义舵机控制引脚和按钮引脚
sbit SG90_PIN = P0^0;      // 舵机信号线连接到P1.3
sbit BUTTON1 = P3^2;       // 按钮1，连接到P3.2
sbit BUTTON2 = P3^3;       // 按钮2，连接到P3.3
sbit BUTTON3 = P3^4;       // 按钮3，连接到P3.4

// 定义舵机角度对应的PWM值
// 为了调试，我们使用更精确的PWM值
#define ANGLE_0     460   // 0度对应的计数值
#define ANGLE_45    730   // 45度对应的计数值
#define ANGLE_90    1000  // 90度对应的计数值
#define ANGLE_135   1270  // 135度对应的计数值
#define ANGLE_180   1540  // 180度对应的计数值

// 全局变量
unsigned int angle_value = ANGLE_90;  // 当前舵机角度值，默认为90度
bit pwm_output = 0;                   // PWM输出状态
unsigned int pwm_count = 0;           // PWM计数器
unsigned char debug_state = 0;        // 调试状态变量

// 函数声明
void Delay_ms(unsigned int ms);
void LCD_WriteCmd(unsigned char cmd);
void LCD_WriteData(unsigned char dat);
void LCD_Init(void);
void LCD_SetCursor(unsigned char line, unsigned char column);
void LCD_ShowString(unsigned char line, unsigned char column, unsigned char *str);
void LCD_ShowNum(unsigned char line, unsigned char column, unsigned int num, unsigned char len);
void Timer0_Init(void);
void Key_Scan(void);
void Servo_Test(void);

// 延时函数
void Delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 110; j++);
}

// LCD1602写命令函数
void LCD_WriteCmd(unsigned char cmd)
{
    RS = 0;       // 选择命令寄存器
    RW = 0;       // 写操作
    LCD_DATA = cmd;
    EN = 1;       // 使能信号高电平
    Delay_ms(5);
    EN = 0;       // 使能信号低电平
}

// LCD1602写数据函数
void LCD_WriteData(unsigned char dat)
{
    RS = 1;       // 选择数据寄存器
    RW = 0;       // 写操作
    LCD_DATA = dat;
    EN = 1;       // 使能信号高电平
    Delay_ms(5);
    EN = 0;       // 使能信号低电平
}

// LCD1602初始化函数
void LCD_Init()
{
    Delay_ms(15);     // 等待LCD1602初始化完成
    LCD_WriteCmd(0x38); // 设置16x2显示，5x7点阵，8位数据接口
    Delay_ms(5);
    LCD_WriteCmd(0x38);
    Delay_ms(5);
    LCD_WriteCmd(0x38);
    LCD_WriteCmd(0x0C); // 显示开，光标关，闪烁关
    LCD_WriteCmd(0x06); // 写入新数据后光标右移，显示不移动
    LCD_WriteCmd(0x01); // 清屏
    Delay_ms(2);
}

// 设置LCD1602显示位置
void LCD_SetCursor(unsigned char line, unsigned char column)
{
    if(line == 1)
        LCD_WriteCmd(0x80 + column - 1);
    else if(line == 2)
        LCD_WriteCmd(0xC0 + column - 1);
}

// 在LCD1602上显示字符串
void LCD_ShowString(unsigned char line, unsigned char column, unsigned char *str)
{
    LCD_SetCursor(line, column);
    while(*str != '\0')
    {
        LCD_WriteData(*str++);
    }
}

// 在LCD1602上显示数字
void LCD_ShowNum(unsigned char line, unsigned char column, unsigned int num, unsigned char len)
{
    unsigned char i;
    LCD_SetCursor(line, column);
    for(i = 0; i < len; i++)
    {
        LCD_WriteData('0' + num / 10000 % 10);
        num = num * 10;
    }
}

// 定时器0初始化，用于产生PWM信号
void Timer0_Init(void)
{
    TMOD = 0x01;    // 设置定时器0为16位模式
    TH0 = 0xFC;     // 定时1ms的初值(11.0592MHz晶振)
    TL0 = 0x66;
    ET0 = 1;        // 使能定时器0中断
    EA = 1;         // 开总中断
    TR0 = 1;        // 启动定时器0
}

// 按键扫描函数
void Key_Scan(void)
{
    // 检测按钮1是否按下（转到0度）
    if(BUTTON1 == 0)
    {
        Delay_ms(10);  // 消抖
        if(BUTTON1 == 0)
        {
            while(!BUTTON1);  // 等待按键释放
            angle_value = ANGLE_0;  // 设置舵机角度为0度
            LCD_ShowString(1, 1, "Hello, World!");
            LCD_ShowString(2, 1, "button1:0");
        }
    }
    
    // 检测按钮2是否按下（转到90度）
    if(BUTTON2 == 0)
    {
        Delay_ms(10);  // 消抖
        if(BUTTON2 == 0)
        {
            while(!BUTTON2);  // 等待按键释放
            angle_value = ANGLE_90;  // 设置舵机角度为90度
            LCD_ShowString(1, 1, "Hello, World!");
            LCD_ShowString(2, 1, "button2:90");
        }
    }
    
    // 检测按钮3是否按下（转到180度）
    if(BUTTON3 == 0)
    {
        Delay_ms(10);  // 消抖
        if(BUTTON3 == 0)
        {
            while(!BUTTON3);  // 等待按键释放
            angle_value = ANGLE_180;  // 设置舵机角度为180度
            LCD_ShowString(1, 1, "Hello, World!");
            LCD_ShowString(2, 1, "button3:180");
        }
    }
}

// 舵机测试函数
void Servo_Test(void)
{
    // 每500ms改变一次舵机角度，用于测试
    static unsigned int test_count = 0;
    
    test_count++;
    if(test_count >= 50)  // 50 * 10ms = 500ms
    {
        test_count = 0;
        debug_state++;
        
        if(debug_state > 4)
            debug_state = 0;
            
        switch(debug_state)
        {
            case 0:
                angle_value = ANGLE_0;
                LCD_ShowString(2, 1, "Test: 0   ");
                break;
            case 1:
                angle_value = ANGLE_45;
                LCD_ShowString(2, 1, "Test: 45  ");
                break;
            case 2:
                angle_value = ANGLE_90;
                LCD_ShowString(2, 1, "Test: 90  ");
                break;
            case 3:
                angle_value = ANGLE_135;
                LCD_ShowString(2, 1, "Test: 135 ");
                break;
            case 4:
                angle_value = ANGLE_180;
                LCD_ShowString(2, 1, "Test: 180 ");
                break;
        }
    }
}

// 定时器0中断服务函数，用于产生PWM信号
void Timer0_ISR(void) interrupt 1
{
    // 重新加载定时器初值，定时1ms
    TH0 = 0xFC;
    TL0 = 0x66;
    
    // PWM周期为20ms(50Hz)
    pwm_count++;
    if(pwm_count >= 20)
    {
        pwm_count = 0;
    }
    
    // 根据当前角度值产生PWM信号
    if(pwm_count < (angle_value / 100))  // 缩放计数值以适应PWM周期
    {
        SG90_PIN = 1;  // 输出高电平
    }
    else
    {
        SG90_PIN = 0;  // 输出低电平
    }
}

// 主函数
void main()
{
    LCD_Init(); // 初始化LCD1602
    Timer0_Init(); // 初始化定时器
    
    // 初始状态，舵机转到90度位置
    angle_value = ANGLE_90;
    LCD_ShowString(1, 1, "Hello, World!");
    LCD_ShowString(2, 1, "button2:90");
    
    // 延时2秒，确保舵机有足够时间转到初始位置
    Delay_ms(2000);
    
    // 显示测试提示
    LCD_ShowString(1, 1, "Servo Test...");
    
    while(1)
    {
        // 舵机测试，自动循环不同角度
        Servo_Test();
        
        // 扫描按键
        Key_Scan();
        
        // 延时10ms
        Delay_ms(10);
    }
}