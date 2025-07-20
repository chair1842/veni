#include <stdio.h>
#include <kernel/tty.h>
#include "../arch/i386/gdt.h"

void kernel_main(void) {
    gdt_init();          // Initialize GDT first
    terminal_initialize();
    printf("Hello, kernel World!\n");
    printf("GDT initialized successfully!\n");
}
