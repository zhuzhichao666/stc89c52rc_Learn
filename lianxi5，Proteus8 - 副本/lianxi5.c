#include <reg51.h>
#include <intrins.h>

sbit SCL = P0^0;
sbit SDA = P0^1;

// ????(12MHz????)
void delay_us(unsigned int us)
{
    while (us--) _nop_();
}

// I2C??  
void I2C_Start(void)
{
    SDA = 1;
    SCL = 1;
    delay_us(5);
    SDA = 0;
    delay_us(5);
    SCL = 0;
}

// I2C??  
void I2C_Stop(void)
{
    SDA = 0;
    SCL = 1;
    delay_us(5);
    SDA = 1;
    delay_us(5);
}

// I2C?????  
bit I2C_WriteByte(unsigned char byte)
{
    unsigned char i;
    bit ack;
    for (i = 0; i < 8; i++)
    {
        SDA = (byte & 0x80) ? 1 : 0;
        delay_us(2);
        SCL = 1;
        delay_us(2);
        SCL = 0;
        delay_us(2);
        byte <<= 1;
    }
    SDA = 1;
    delay_us(2);
    SCL = 1;
    delay_us(2);
    ack = SDA;
    SCL = 0;
    delay_us(2);
    return ack;
}

// OLED???(SSD1306 128x64)
void OLED_Init(void)
{
    I2C_Start();
    I2C_WriteByte(0x78);  // OLED I2C??
    I2C_WriteByte(0x00);  // ????
    I2C_WriteByte(0xAE);  // ????
    I2C_WriteByte(0xD5);  // ??????
    I2C_WriteByte(0x80);  // ??????
    I2C_WriteByte(0xA8);  // ???????
    I2C_WriteByte(0x3F);  // 128x64???,63?(0x3F)
    I2C_WriteByte(0xD3);  // ??????
    I2C_WriteByte(0x00);  // ???
    I2C_WriteByte(0x40);  // ?????  
    I2C_WriteByte(0x8D);  // ????? 
    I2C_WriteByte(0x14);  // ?????  
    I2C_WriteByte(0x20);  // ??????
    I2C_WriteByte(0x00);  // ??????
    I2C_WriteByte(0xA0);  // ??????
    I2C_WriteByte(0xC0);  // ??????
    I2C_WriteByte(0xDA);  // ??COM??????
    I2C_WriteByte(0x12);  // ??COM????
    I2C_WriteByte(0x81);  // ??????
    I2C_WriteByte(0xCF);  // ??????
    I2C_WriteByte(0xAF);  // ????
    I2C_Stop();
}

// ??OLED????(x, page)
void OLED_SetPos(unsigned char x, unsigned char page)
{
    I2C_Start();
    I2C_WriteByte(0x78);
    I2C_WriteByte(0x00);  // ????
    I2C_WriteByte(0xB0 | page);  // ?????(0-7)
    I2C_WriteByte(x & 0x0F);     // ??????4?
    I2C_WriteByte(0x10 | (x >> 4));  // ??????4?
    I2C_Stop();
}

// 16x16??(??'1')
const unsigned char code font16x16[][32] = {
    // ?? '1'
    {0x00, 0x18, 0x38, 0x58, 0x78, 0x18, 0x18, 0x18, 
     0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // Add other digits if necessary for '0', '2', '3', etc.
};

// ??????
void OLED_Clear(void)
{
    unsigned char page, col;
    for (page = 0; page < 8; page++)
    {
        OLED_SetPos(0, page);
        I2C_Start();
        I2C_WriteByte(0x78);
        I2C_WriteByte(0x40);  // ????  
        for (col = 0; col < 128; col++)
        {
            I2C_WriteByte(0x00);  // ????  
        }
        I2C_Stop();
    }
}

// ????  
void OLED_DisplayChar(char c)
{
    unsigned char j, pos = 56;  // ??????,(128 - 16) / 2 = 56
    unsigned char char_index = (c == '1') ? 0 : 0;  // ????? '1'

    // ?????????  
    OLED_SetPos(pos, 2);
    I2C_Start();
    I2C_WriteByte(0x78);
    I2C_WriteByte(0x40);  // ????  
    for (j = 0; j < 16; j++)
        I2C_WriteByte(font16x16[char_index][j]);
    I2C_Stop();

    // ?????????  
    OLED_SetPos(pos, 3);
    I2C_Start();
    I2C_WriteByte(0x78);
    I2C_WriteByte(0x40);  // ????  
    for (j = 16; j < 32; j++)
        I2C_WriteByte(font16x16[char_index][j]);
    I2C_Stop();
}

// ????  
void OLED_DisplayNumber(int num)
{
    char digit;
    if (num >= 0 && num <= 9)
    {
        digit = num + '0';  // Convert number to character
        OLED_DisplayChar(digit);  // Display the character on the screen
    }
}

// ???  
void main(void)
{
    int countdown = 10;  // Start from 10

    OLED_Init();             // ???OLED
    OLED_Clear();            // ????  

    while (countdown >= 0)
    {
        OLED_Clear();            // Clear the screen each time to update the countdown
        OLED_DisplayNumber(countdown);  // Display the countdown number
        delay_us(1000000);  // Delay for 1 second
        countdown--;  // Decrement the countdown
    }

    while (1);  // End of countdown, keep the microcontroller in a loop
}
