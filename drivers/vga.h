#ifndef VGA_H
#define VGA_H
#include <stdint.h>

void kputc(char c);
void kcls();
void kputs(char *c);
void kputl(char *c);
void kputi(uint32_t i);

#endif // MONITOR_H