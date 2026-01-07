/* Kernel panic implementation */
#include <kernel/kpanic.h>
#include <kernel/tty.h>
#include <stdio.h>

void kpanic(const char* message) {
    // Disable interrupts
    __asm__ volatile ("cli");

    // Clear the screen
    terminal_clear();

    terminal_fsetcolor(0x4, 0x0); // Red text on black background

    // Print panic message
    printf("KERNEL PANIC: %s\n", message);
    printf("System halted.\n");

    // Halt the CPU
    while (1) {
        __asm__ volatile ("hlt");
    }
}