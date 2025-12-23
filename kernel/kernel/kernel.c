#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/kb_keys.h>
#include <string.h>

void kernel_main(void) {
    terminal_clear();
    printf("This is the veni typing playground\nYou can erase us\n");
    while (1) {
        struct KeyPacket pkt = keyboard_getkey();
        char ascii = key_to_ascii(pkt);
        if (pkt.keycode == KEY_BACKSPACE && pkt.pressed) {
            terminal_backspace();
        }
        if (pkt.keycode == KEY_TAB && pkt.pressed) {
            printf("    ");
        }
        if (pkt.pressed && ascii) {
            printf("%c", ascii);
        }
    }
    // never return
}
