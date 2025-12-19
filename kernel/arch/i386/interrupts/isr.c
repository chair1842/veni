#include <kernel/tty.h>
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

void irq_handler(uint32_t vector, uint32_t error) {
    (void)error;

    uint8_t irq = vector - 32;

    // Acknowledge FIRST
    pic_send_eoi(irq);

    // Minimal handling only
    switch (irq) {
        case 1:
            uint8_t scancode = inb(0x60);
            printf("Keyboard interrupt received. Scancode: %d\n", scancode);
            break;
        default:
            printf("Unhandled IRQ: %d\n", irq);
            break;
    }
}
