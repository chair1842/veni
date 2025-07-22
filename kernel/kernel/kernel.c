#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_early(void) {
    gdt_init();
}


void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
