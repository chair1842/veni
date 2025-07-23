#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

// Initialize the IDT
void idt_init(void);

// Structure for IDT entries
typedef struct {
    uint16_t base_low;    // Lower 16 bits of handler address
    uint16_t selector;    // Kernel segment selector
    uint8_t zero;         // Must be zero
    uint8_t flags;        // Type and attributes
    uint16_t base_high;   // Upper 16 bits of handler address
} __attribute__((packed)) idt_entry_t;

// IDT pointer structure
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

// ISR handlers
void isr0(void);  // Division by zero
void isr1(void);  // Debug
void isr2(void);  // Non-maskable interrupt
void isr3(void);  // Breakpoint
void isr4(void);  // Overflow
void isr5(void);  // Bound range exceeded
void isr6(void);  // Invalid opcode
void isr7(void);  // Device not available
void isr8(void);  // Double fault
void isr9(void);  // Coprocessor segment overrun
void isr10(void); // Invalid TSS
void isr11(void); // Segment not present
void isr12(void); // Stack-segment fault
void isr13(void); // General protection fault
void isr14(void); // Page fault
void isr15(void); // Reserved
void isr16(void); // x87 FPU error
void isr17(void); // Alignment check
void isr18(void); // Machine check
void isr19(void); // SIMD Floating-Point Exception
void isr20(void); // Virtualization Exception
void isr21(void); // Control Protection Exception
// ISRs 22-31 are reserved

// Register structure for interrupt handlers
typedef struct {
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;                      // Interrupt number and error code
    uint32_t eip, cs, eflags, esp, ss;             // Pushed by the processor automatically
} registers_t;

// Interrupt handler function type
typedef void (*isr_t)(registers_t*);

// Register an interrupt handler
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif
