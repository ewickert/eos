#include "gdt.h"
#include "../drivers/vga.h"
extern void gdt_flush(u32int);
gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;
static void gdt_set_gate(int32_t n, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[n].base_low = (base & 0xFFFF);
    gdt_entries[n].base_middle = (base >> 16) & 0xFF;
    gdt_entries[n].base_high = (base >> 24) & 0xFF;

    gdt_entries[n].limit_low = (limit & 0xffff);
    gdt_entries[n].granularity = (limit >> 16) & 0x0f;

    gdt_entries[n].granularity |= gran & 0xF0;
    gdt_entries[n].access = access;
}
void gdt_init()
{
    kerr_ln("begin gdt init");
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;
    gdt_set_gate(0, 0, 0, 0, 0);                // null
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9a, 0xcf); // code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xcf); // data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xcf); // user code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xcf); // user data
    gdt_flush((uint32_t)&gdt_ptr);
    kerr_ln("Global Descriptor Table Initialized.");
}