#include <stdio.h>

#include <kernel/tty.h>

void kernel_early(void) {
    // do nothing for now
    return;
}


void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
