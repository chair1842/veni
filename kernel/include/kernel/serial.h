#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <kernel/io.h>

// Serial port I/O ports
#define SERIAL_COM1_BASE    0x3F8
#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

// Serial port configuration
#define SERIAL_LINE_ENABLE_DLAB         0x80

void serial_init();
void serial_putchar(char c);
void serial_write(const char* data, size_t size);
void serial_writestring(const char* data);

#endif