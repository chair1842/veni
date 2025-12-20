#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <string.h>

void kernel_main(void) {
    terminal_clear();
    printf("Veni says hello\n");
    while (1) {
        asm volatile ("hlt");
    }
    // never return
}
