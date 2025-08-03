#include <kernel/isr.h>
#include <kernel/idt.h>
#include <stdio.h>

// Array of interrupt handlers
static isr_handler_t interrupt_handlers[32];

// Default exception handler that halts the system
static void default_exception_handler(struct registers* r) {
    printf("Received interrupt: %d\n", r->int_no);
    printf("Error code: %d\n", r->err_code);
    printf("Exception: %s\n", exception_messages[r->int_no]);
    
    // Print registers for debugging
    printf("Register dump:\n");
    printf("EAX: 0x%x  EBX: 0x%x  ECX: 0x%x  EDX: 0x%x\n", r->eax, r->ebx, r->ecx, r->edx);
    printf("ESI: 0x%x  EDI: 0x%x  EBP: 0x%x  ESP: 0x%x\n", r->esi, r->edi, r->ebp, r->esp);
    printf("EIP: 0x%x  CS:  0x%x  DS:  0x%x  SS:  0x%x\n", r->eip, r->cs, r->ds, r->ss);
    printf("EFLAGS: 0x%x\n", r->eflags);
    
    printf("\nSystem halted!\n");
    for(;;);
}

// External declarations for ISR stubs
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void isr_register_handler(uint8_t n, isr_handler_t handler) {
    if (n < 32) {
        interrupt_handlers[n] = handler;
    }
}

void isr_unregister_handler(uint8_t n) {
    if (n < 32) {
        interrupt_handlers[n] = default_exception_handler;
    }
}

void isr_install(void) {
    // Initialize all handlers to the default handler
    for (int i = 0; i < 32; i++) {
        interrupt_handlers[i] = default_exception_handler;
    }

    // Install the ISRs
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
}

void isr_handler(struct registers *r) {
    if (r->int_no < 32 && interrupt_handlers[r->int_no]) {
        interrupt_handlers[r->int_no](r);
    } else {
        default_exception_handler(r);
    }
}
