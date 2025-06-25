#include "oled.h"
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// 输入：灰度图（64行*128列），阈值；输出：oled点阵缓冲区（8页×128列）
// 参数改为指针，内部使用时按一维数组访问
void GrayToOledBinary(const unsigned char *gray_img, unsigned char *oled_buf, unsigned char threshold) {
    unsigned char page, col, bit0;
    for (page = 0; page < 8; page++) {
        for (col = 0; col < 128; col++) {
            unsigned char byte = 0;
            for (bit0 = 0; bit0 < 8; bit0++) {
                unsigned char row = page * 8 + bit0;
                // gray_img[row][col] 变成 *(gray_img + row*128 + col)
                if (*(gray_img + row*128 + col) > threshold) {
                    byte |= (1 << bit0);
                }
            }
            // oled_buf[page][col] 改成 *(oled_buf + page*128 + col)
            *(oled_buf + page*128 + col) = byte;
        }
    }
}

void OLED_ShowImage(const unsigned char *oled_buf) {
    unsigned char page, col;
    for (page = 0; page < 8; page++) {
        OLED_SetPos(0, page);
        for (col = 0; col < 128; col++) {
            OLED_WriteData(*(oled_buf + page*128 + col));
        }
    }
}

