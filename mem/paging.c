#include <stdint.h>
#include <stdbool.h>
#include "paging.h"
#include "../lib/stdlib.h"
#include "../drivers/com.h"

uint32_t *frames;
uint32_t nframes;

// page_directory_t *kernel_d = 0;
// page_directory_t *current_d = 0;

uint32_t *kernel_d = 0x0;
uint32_t *current_d = 0x0;
void *kpage_cur;
void *kheap_cur;
extern uint32_t _kernel_end;
extern

#define BOUNDARY 0x1000;
#define pd_idx(vaddr) ((uint32_t)vaddr >> 22)
#define pt_idx(vaddr) ((uint32_t)vaddr >> 12 & 0x03FF)

    void *
    get_kpage();
void *get_page();
void *get_paddr(void *vaddr);
void init_paging()
{
    register_interrupt_handler(14, page_fault);
    kernel_d = (uint32_t *)0xFFFFF000;
    kpage_cur = ((uint32_t)&_kernel_end & 0xFFFFF000);
    kpage_cur = (uint32_t)kpage_cur + 0x1000;

    uint32_t *pt = (uint32_t *)(0xFFC00000) + (0x400 * 768); // kernel_d + 768;
    // Find where the kernel ends, physically, because I want to hand out
    // kernel frames starting here
    kerr_named_dword("kernel_end_p:", get_paddr(&_kernel_end));
    kerr_named_dword("kernel_end_v:", &_kernel_end);

    uint32_t p_count = 0;
    for (int x = 0; x < 1023; x++)
    {
        if (!(*(pt + x) & 1))
        {

            kerr_named_dword("page absent", p_count);
            p_count += 1;
            continue;
        }
        p_count += 1;
        kerr_named_dword("page present", p_count);
    }
    kheap_cur = get_kpage();
    kerr_named_dword("kheap", kheap_cur);
    uint16_t foo = (uint16_t *)kheap_cur;
}
void *get_kpage()
{
    void *tmp = kpage_cur;
    kpage_cur += 0x1000;
    return tmp;
}
void *palloc()
{
}
void *get_paddr(void *vaddr)
{
    uint32_t pd_i = pd_idx(vaddr);
    uint32_t pt_i = pt_idx(vaddr);
    uint32_t *dir = (uint32_t)0xFFFFF000;
    uint32_t *pt = ((uint32_t *)0xFFC00000) + (0x400 * pd_i);
    return (void *)((pt[pt_i] & ~0xFFF) + ((uint32_t)vaddr & 0xFFF));
}

void page_fault(registers_t regs)
{
    kerr_ln("PAGEFAULT");
    while (1)
        ;
}

void switch_paged(uint32_t *new)
{
    current_d = new;
    asm volatile("mov %0, %%cr3" ::"r"(new));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0" ::"r"(cr0));
}