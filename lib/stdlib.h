#include <stdint.h>
#include <stddef.h>
char *citoa(int n, char *str, int base);
char *lltoa(uint64_t n, char *str, int base);
void *memset(void *str, int c, size_t n);