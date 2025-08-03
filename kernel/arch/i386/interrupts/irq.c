#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <stdio.h>
#include <kernel/io.h>
#include <stddef.h>

// Array of IRQ handlers
static isr_handler_t irq_handlers[16];

// External declarations for IRQ stubs
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Simple keyboard handler
static void keyboard_handler(struct registers* r) {
    printf("Keyboard handler called\n");  // Debug print
    
    // Read from keyboard's data port
    uint8_t scancode = inb(0x60);
    printf("Key event! Scancode: 0x%x\n", scancode);
}

void irq_register_handler(uint8_t irq, isr_handler_t handler) {
    printf("Registering handler for IRQ %d\n", irq);  // Debug print
    if (irq < 16) {
        irq_handlers[irq] = handler;
        // Unmask (enable) the IRQ line
        irq_clear_mask(irq);
        printf("Handler registered and IRQ %d unmasked\n", irq);  // Debug print
    }
}

void irq_unregister_handler(uint8_t irq) {
    if (irq < 16) {
        irq_handlers[irq] = NULL;
        // Mask (disable) the IRQ line
        irq_set_mask(irq);
    }
}

void irq_handler(struct registers* r) {
    // Get the IRQ number (subtract 32 from the interrupt number)
    uint8_t irq = r->int_no - 32;

    printf("IRQ %d received\n", irq);  // Debug print

    // Handle the IRQ if we have a handler
    if (irq < 16 && irq_handlers[irq]) {
        printf("Calling handler for IRQ %d\n", irq);  // Debug print
        irq_handlers[irq](r);
    } else {
        printf("No handler for IRQ %d\n", irq);  // Debug print
    }

    // Send EOI to the PIC
    pic_send_eoi(irq);
}

void irq_install(void) {
    // Initialize handlers array
    for (int i = 0; i < 16; i++) {
        irq_handlers[i] = NULL;
    }

    // Install IRQ handlers into IDT (starting at entry 32)
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    // Register keyboard handler
    irq_register_handler(1, keyboard_handler);  // IRQ1 is keyboard
    
    // Explicitly unmask (enable) the keyboard interrupt
    irq_clear_mask(1);
    
    printf("Keyboard handler installed\n");  // Debug message
}
