#include <stdint.h>

/// @brief Write a string to the serial port debug stream.
/// @param s
void kerr(char *s);

/// @brief Write a string to serial port with a newline.
/// @param s The string to write to COM1
void kerr_ln(char *s);

void kerr_named_dword(char *name, uint32_t n);