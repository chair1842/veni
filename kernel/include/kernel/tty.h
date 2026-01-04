#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize();
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_fsetcolor(uint8_t fg, uint8_t bg);
void terminal_clear();
void terminal_tab(size_t spaces);
void terminal_backspace();
void terminal_setcursorpos(size_t row, size_t column);

#endif
