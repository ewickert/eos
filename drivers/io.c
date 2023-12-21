#include "io.h"
unsigned char p_read_byte(unsigned short port)
{
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void p_write_byte(unsigned short port, unsigned char data)
{
    __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}
void memcpy_io(char *src, char *dst, int count)
{
    int i;
    for (i = 0; i < count; i++)
        *(dst + i) = *(src + i);
}