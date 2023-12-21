#include "vga.h"
void set_cursor(int offset)
{
    offset /= 2;
    p_write_byte(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    p_write_byte(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));
    p_write_byte(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    p_write_byte(VGA_DATA_REGISTER, (unsigned char)(offset & 0xff));
}

int get_cursor()
{
    p_write_byte(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = p_read_byte(VGA_DATA_REGISTER) << 8;
    p_write_byte(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += p_read_byte(VGA_DATA_REGISTER);
    return offset * 2;
}

void print_backspace()
{
    int offset = get_cursor() - 2;
    set_char('\0', offset);
    set_cursor(offset);
}

void set_char(char c, int offset)
{
    unsigned char *vmem = (unsigned char *)VIDEO_ADDRESS;
    vmem[offset] = c;
    vmem[offset + 1] = WHITE_ON_BLACK;
}

void print_string(char *str)
{
    int offset = get_cursor();
    int i = 0;
    // set_char(str[0], 0);
    while (str[i] != 0)
    {
        if (offset >= MAX_ROWS * MAX_COLS * 2)
            offset = scroll_ln(offset);

        if (str[i] == '\n')
        {
            offset = print_newline_at(offset);
        }
        else
        {
            set_char(str[i], offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
    // set_char('9', 0);
}

int get_row_from_offset(int offset)
{
    return offset / (2 * MAX_COLS);
}

int get_offset(int col, int row)
{
    return 2 * (row * MAX_COLS + col);
}

int print_newline_at(int offset)
{
    return get_offset(0, get_row_from_offset(offset) + 1);
}

int print_newline()
{
    int offset = get_cursor();
    return print_newline_at(offset);
}

int scroll_ln(int offset)
{
    memcpy_io(
        (char *)(get_offset(0, 1) + VIDEO_ADDRESS),
        (char *)(get_offset(0, 0) + VIDEO_ADDRESS),
        MAX_COLS * (MAX_ROWS - 1) * 2);
    for (int col = 0; col < MAX_COLS; col++)
    {
        set_char(' ', get_offset(col, MAX_ROWS - 1));
    }
    return offset - 2 * MAX_COLS;
}

void clear_screen()
{
    for (int i = 0; i < MAX_COLS; ++i)
    {
        set_char(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}
