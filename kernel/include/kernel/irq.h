#ifndef _KERNEL_IRQ_H
#define _KERNEL_IRQ_H

#include <stdint.h>
#include <kernel/isr.h>

// IRQ numbers
#define IRQ0    32      // Timer
#define IRQ1    33      // Keyboard
#define IRQ2    34      // Cascade for IRQs 8-15
#define IRQ3    35      // COM2
#define IRQ4    36      // COM1
#define IRQ5    37      // LPT2
#define IRQ6    38      // Floppy
#define IRQ7    39      // LPT1
#define IRQ8    40      // RTC
#define IRQ9    41      // Free
#define IRQ10   42      // Free
#define IRQ11   43      // Free
#define IRQ12   44      // PS2 Mouse
#define IRQ13   45      // FPU
#define IRQ14   46      // Primary ATA
#define IRQ15   47      // Secondary ATA

void irq_install(void);
void irq_register_handler(uint8_t irq, isr_handler_t handler);
void irq_unregister_handler(uint8_t irq);

#endif
