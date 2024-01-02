#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>
#include <stdbool.h>
#include "../cpu/isr.h"
void init_paging();
void switch_paged(uint32_t *n);
void page_fault(registers_t regs);
#endif // PAGING_H