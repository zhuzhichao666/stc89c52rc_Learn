#include <reg52.h>

#define CRYSTAL_FREQ 11059200UL
#define TIMER_DIVIDER 12UL

#define RldTmr(frq) (65536 - (uint)(CRYSTAL_FREQ / TIMER_DIVIDER / ((uint)(frq) * 2UL)))

typedef unsigned char uchar;
typedef unsigned int uint;

sbit BUZZ = P0^0;

bit enable = 0;
uint T0RH = 0, T0RL = 0;

uint code noteFreq[] = {
    262, 294, 330, 349, 392, 440, 494, 523,
    587, 659, 698, 784, 880, 988, 1047, 1175
};

void delay(uint ms) {
    uint i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++);
}

void timer0_ISR(void) interrupt 1 using 1 {
    TH0 = T0RH;
    TL0 = T0RL;

    if(enable)
        BUZZ = ~BUZZ;
    else
        BUZZ = 1;
}

// 4x4键盘扫描，返回0~15，没按返回0xFF
uchar keyscan() {
    uchar i, temp, keynum = 0xFF;

    for(i = 0; i < 4; i++) {
        P2 = ~(0x01 << i);
        temp = P2 & 0xF0;
        if(temp != 0xF0) {
            delay(10);  // 防抖
            temp = P2 & 0xF0;
            if(temp != 0xF0) {
                if((temp & 0x10) == 0) keynum = i * 4 + 0;
                else if((temp & 0x20) == 0) keynum = i * 4 + 1;
                else if((temp & 0x40) == 0) keynum = i * 4 + 2;
                else if((temp & 0x80) == 0) keynum = i * 4 + 3;
                break;
            }
        }
    }
    return keynum;
}

// 小星星曲调（数字表示音调索引）
uchar code twinkleNotes[] = {0,0,4,4,5,5,4,3,3,2,2,1,1,0};
uchar code twinkleBeats[] = {1,1,1,1,1,1,2,1,1,1,1,1,1,2};
#define TWINKLE_LEN (sizeof(twinkleNotes)/sizeof(twinkleNotes[0]))

// 两只老虎曲调
uchar code tigerNotes[] = {0,1,2,0,0,1,2,0,2,3,4,0,2,3,4,0};
uchar code tigerBeats[] = {1,1,1,2,1,1,1,2,1,1,2,2,1,1,2,2};
#define TIGER_LEN (sizeof(tigerNotes)/sizeof(tigerNotes[0]))

// 播放单个音符（noteIndex是noteFreq数组索引，beat是节拍，单位为250ms）
void playNote(uchar noteIndex, uchar beat) {
    uint rld;

    if(noteIndex >= sizeof(noteFreq)/sizeof(noteFreq[0])) {
        enable = 0;
        return;
    }

    rld = RldTmr(noteFreq[noteIndex]);
    T0RH = (rld >> 8) & 0xFF;
    T0RL = rld & 0xFF;

    enable = 1;
    delay(beat * 250);
    enable = 0;
    delay(50);
}

// 播放整首歌
void playSong(const uchar code *notes, const uchar code *beats, uchar length) {
    uchar i;
    for(i = 0; i < length; i++) {
        playNote(notes[i], beats[i]);
    }
}

// 简单10ms延时
void delayMs(uint ms) {
    uint i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++);
}

void main() {
    uchar key_now = 0xFF;
    uchar key_last = 0xFF;

    uint current_time = 0;
    uint last_press_time = 0;

    uint double_click_max_interval = 300; // 300ms双击判定

    bit is_playing_song = 0;   // 标记是否正在播放歌曲（阻止按键冲突）
    uchar playing_song_key = 0xFF;
    TMOD = 0x01;  // 定时器0模式1
    EA = 1;
    ET0 = 1;
    TR0 = 1;



    while(1) {
        key_now = keyscan();

        if(is_playing_song) {
            // 正在播放歌曲，忽略按键
            delayMs(10);
            current_time += 10;
            if(current_time > 60000) current_time = 0;
            continue;
        }

        if(key_now != 0xFF) {
            if(key_now == key_last) {
                // 双击检测
                if(current_time - last_press_time < double_click_max_interval) {
                    // 触发双击
                    if(key_now == 0) {
                        is_playing_song = 1;
                        playing_song_key = 0;
                        playSong(twinkleNotes, twinkleBeats, TWINKLE_LEN);
                        is_playing_song = 0;
                        playing_song_key = 0xFF;
                    } else if(key_now == 1) {
                        is_playing_song = 1;
                        playing_song_key = 1;
                        playSong(tigerNotes, tigerBeats, TIGER_LEN);
                        is_playing_song = 0;
                        playing_song_key = 0xFF;
                    }
                    last_press_time = 0; // 处理完毕，重置计时
                } else {
                    last_press_time = current_time;
                }
            } else {
                last_press_time = current_time;
            }

            // 如果是非0和1按键，长按播放对应音调
            if(key_now >= 2 && !is_playing_song) {
                // 连续发声直到松开
                uchar noteIndex = key_now;
                uint rld = RldTmr(noteFreq[noteIndex]);
                T0RH = (rld >> 8) & 0xFF;
                T0RL = rld & 0xFF;
                enable = 1;
                // 等待松开
                while(keyscan() == key_now) {
                    delayMs(10);
                }
                enable = 0;
            }

            key_last = key_now;
        } else {
            key_last = 0xFF;
            enable = 0; // 松开按键，停止声音
        }

        delayMs(10);
        current_time += 10;
        if(current_time > 60000) current_time = 0;  // 避免溢出
    }
}
