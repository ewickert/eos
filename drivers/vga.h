#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

#include "io.h"

void set_cursor(int);
int get_cursor();
void print_backspace();
void set_char(char, int);
void print_string(char *s);
int get_row_from_offset(int);
int get_offset(int, int);
int print_newline_at(int);
int print_newline();
int scroll_ln(int);
void clear_screen();
