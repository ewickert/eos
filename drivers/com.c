#include <stdint.h>
#include "../lib/stdlib.h"
#include "com.h"
#include "io.h"

#define COM1 0x3f8

void kerr(char *s)
{
    int i = 0;
    while (s[i] != '\0')
        p_write_byte(COM1, s[i++]);
}

void kerr_ln(char *s)
{
    kerr(s);
    p_write_byte(COM1, '\r');
    p_write_byte(COM1, '\n');
}

void kerr_lpad(char *s, char p, size_t width)
{
    size_t l = strlen(s);
    while (l < width)
    {
        char pad[2] = {p, '\0'};
        // kprint(pad);
        l++;
    }
    // kprint(s);
}

void kerr_named_dword(char *name, uint32_t n)
{
    kerr(name);
    kerr("=0x");
    char n_str[20];
    memset(n_str, '\0', 10);
    citoa(n, n_str, 16);
    kerr_ln(n_str);
    // kerr_lpad(n_str, '0', 4);
    // kerr_ln("");
}