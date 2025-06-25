#include <reg52.h>
#include "iic.h"
#include <intrins.h>

sbit SDA = P1^1;
sbit SCL = P1^0;

void IIC_Delay(void) {
    _nop_();_nop_();_nop_();_nop_();
}

void IIC_Start(void) {
    SDA = 1; SCL = 1; IIC_Delay();
    SDA = 0; IIC_Delay();
    SCL = 0;
}

void IIC_Stop(void) {
    SDA = 0; SCL = 1; IIC_Delay();
    SDA = 1; IIC_Delay();
}

void IIC_WaitAck(void) {
    SDA = 1; IIC_Delay();
    SCL = 1; IIC_Delay();
    SCL = 0;
}

void IIC_SendByte(unsigned char byte) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        SDA = (byte & 0x80);
        byte <<= 1;
        SCL = 1; IIC_Delay();
        SCL = 0; IIC_Delay();
    }
    IIC_WaitAck();
}