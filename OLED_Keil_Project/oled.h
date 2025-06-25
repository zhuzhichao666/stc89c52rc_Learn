#ifndef __OLED_H__
#define __OLED_H__

void OLED_Init(void);
void OLED_Clear(void);
void OLED_WriteData(unsigned char dat);
void OLED_ShowChar(unsigned char x, unsigned char y, char chr);
void OLED_ShowString(unsigned char x, unsigned char y, char *str);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_ShowImage(const unsigned char *oled_buf);
void GrayToOledBinary(const unsigned char *gray_img, unsigned char *oled_buf, unsigned char threshold);

#endif