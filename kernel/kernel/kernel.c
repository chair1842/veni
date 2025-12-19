#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <string.h>

void kernel_main(void) {
    terminal_clear();
    printf("Veni says hello\n");
    char c;
    while (1) {
        while((c = keyboard_get_char())) {
            printf(c == '\r' ? "\n" : "%c", c);
        }
    }
    // never return
}
