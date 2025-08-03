#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

// IDT entry structure
struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

// IDTR structure
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Function declarations
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install(void);
void idt_load(void);

#endif
