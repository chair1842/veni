#ifndef _KERNEL_ISR_H
#define _KERNEL_ISR_H

#include <stdint.h>

// Structure for storing processor state
struct registers {
    uint32_t ds;                  // Data segment
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;    // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically
} __attribute__((packed));

// Exception messages for the first 32 interrupts
static const char *exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// Define handler function type
typedef void (*isr_handler_t)(struct registers*);

// Function declarations
void isr_install(void);
void isr_handler(struct registers *r);
void isr_register_handler(uint8_t n, isr_handler_t handler);
void isr_unregister_handler(uint8_t n);

#endif
