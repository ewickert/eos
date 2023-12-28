#include "vga.h"
#include "io.h"
#include <stdint.h>

#define FRAMEB 0xB8000
#define C_REG 0x3D4
#define C_SET_HIGH 14
#define C_SET_LOW 15
#define D_REG 0x3D5
#define GET_OFFSET(x, y) ((uint16_t)((y * 80 + x)))
#define HI(x) (x >> 8)
#define ROW_MAX 25
#define COL_MAX 80

#define BLK 0
#define BLU 1
#define GRN 2
#define CYN 3
#define RED 4
#define MAG 5
#define BRN 6
#define LGR 7
#define DGR 8
#define LBL 9
#define LGR 10
#define LCY 11
#define LRD 12
#define LMG 13
#define LBR 14
#define WHT 15
uint16_t *video_memory = (uint16_t *)FRAMEB;

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

static void move_cursor()
{
    uint16_t loc = cursor_y * 80 + cursor_x;
    p_write_byte(C_REG, C_SET_HIGH);
    p_write_byte(D_REG, HI(loc));
    p_write_byte(C_REG, C_SET_LOW);
    p_write_byte(D_REG, loc);
}

static void scroll()
{
    uint8_t attribute = (0 << 4) | (15 & 0x0F);
    uint8_t blank = 0x20 | (attribute << 8);

    if (cursor_y >= ROW_MAX)
    {
        int i;
        for (i = 0; i < 24 * 80; i++)
        {
            video_memory[i] = video_memory[i + 80];
        }
        for (i = 24 * 80; i < 25 * 80; i++)
        {
            video_memory[i] = blank;
        }
        cursor_y = 24;
    }
}

void kputc(char c)
{
    uint8_t back = BLK;
    uint8_t fore = WHT;

    uint16_t attr = ((back << 4) | (fore & 0x0F)) << 8;
    uint16_t *location;
    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
        location = video_memory + GET_OFFSET(cursor_x, cursor_y);
        *location = 0x20 | attr;
    }
    else if (c == 0x09)
    {
        cursor_x = (cursor_x + 8) & (~8 - 1);
    }
    else if (c == '\r')
    {
        cursor_x = 0;
    }
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if (c >= ' ')
    {
        location = video_memory + GET_OFFSET(cursor_x, cursor_y);
        *location = c | attr;
        cursor_x++;
    }

    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }
    scroll();
    move_cursor();
}

void kcls()
{
    uint8_t attribute = (0 << 4) | (15 & 0x0F);
    uint8_t blank = 0x20 | (attribute << 8);

    int i;
    for (i = 24 * 80; i < 25 * 80; i++)
    {
        video_memory[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void kputs(char *c)
{
    int i = 0;
    while (c[i])
        kputc(c[i++]);
}
void kputl(char *c)
{
    kputs(c);
    kputs("\r\n");
}