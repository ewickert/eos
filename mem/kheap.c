#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct heap_node
{
    uint32_t size;
    bool free;
    struct heap_node *next;
    struct heap_node *last;

} heap_node_t;

static heap_node_t *start;
void init_heap(void *s)
{
    start = s;
    start->size = 0x1000 - sizeof(heap_node_t);
    start->next = NULL;
    start->last = NULL;
}

void *kmalloc(size_t size)
{
    // if we get called before the heap is set up
    if (!start)
    {
        return 0;
    }
    heap_node_t *cur = start;
    while ((cur->size + sizeof(heap_node_t) < size || !cur->free) && cur)
    {
        cur = cur->next;
    }
    if (!cur)
    {
        // OOM, need new page
        return 0;
    }
    cur->size = cur->size - size - sizeof(heap_node_t);
    heap_node_t *new = (heap_node_t *)(((uint8_t)cur) + sizeof(heap_node_t) + cur->size);

    new->size = size;
    new->free = false;
    new->next = cur->next;
    new->last = cur;
    if (cur->next)
    {
        cur->next->last = new;
    }
    cur->next = new;
    return (void *)((uint8_t *)new + sizeof(heap_node_t));
}

void kfree(void *p)
{
    if (!p)
        return;
    heap_node_t *cur = (uint8_t *)p - sizeof(heap_node_t);
    if (!cur)
        return;
    cur->free = true;
    if (cur->next && cur->next->free)
    {
        cur += cur->next->size;
        cur += sizeof(heap_node_t);
        if (cur->next->next)
        {
            cur->next->next->last = cur;
        }
        cur->next = cur->next->next;
    }

    if (cur->last && cur->last->free)
    {
        cur->last->size += cur->size;
        cur->last->size += sizeof(heap_node_t);
        if (cur->next)
        {
            cur->next->last = cur->last;
        }
        cur->last->next = cur->next;
    }
}