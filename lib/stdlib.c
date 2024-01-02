#include <stdbool.h>
#include <stdint.h>
#include "stdlib.h"
void reverse(char str[], int l)
{
    int start = 0;
    int end = l - 1;
    while (start < end)
    {
        char tmp = str[start];
        str[start] = str[end];
        str[end] = tmp;
        end--;
        start++;
    }
}

char *citoa(int value, char *sp, int radix)
{

    char tmp[16]; // be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign)
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}

void *memset(void *str, int c, size_t n)
{
    unsigned char *p = str;
    for (int x = 0; x < n; x++)
    {
        *p = c;
        p++;
    }
}