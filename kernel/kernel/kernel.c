#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <string.h>

void kernel_main(void) {
    terminal_clear();
    printf("Veni says hello\n");
    while (1) {
        __asm__ volatile ("hlt");
        char c = keyboard_getchar();
        printf("%c", c);
    }
    // never return
}
