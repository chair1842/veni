#include <kernel/tty.h>
#include "../drivers/pit/pit.h"
#include "../drivers/keyboard/keyboard.h"
#include "pic.h"
#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn))
void exception_handler(uint32_t int_no, uint32_t err);
void exception_handler(uint32_t int_no, uint32_t err) {
    terminal_clear();
    terminal_fsetcolor(12, 0);
    printf("An exception has occured.\nPlease reboot the system.\n");
    abort();
}

void irq_handler(uint32_t vector) {
    uint8_t irq = vector - 32;

        // Minimal handling only
    switch (irq) {
        case 0:
            pit_handler();
            break;
        case 1:
            keyboard_handler(); // FUCK FUCK FUCK
            break;
        default:
            printf("Unhandled IRQ: %d\n", irq);
            break;
    }

    pic_send_eoi(irq);
}
