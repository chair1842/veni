.section .text
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

// IRQ handler stub macro
.macro IRQ_STUB num
    .global irq\num
    irq\num:
        cli
        push $0          // Push dummy error code
        push $(\num + 32)  // Push interrupt number (IRQs start at 32)
        jmp irq_common_stub
.endm

// Common IRQ handler stub
irq_common_stub:
    pusha               // Push all registers

    mov %ds, %ax        // Save data segment
    push %eax

    mov $0x10, %ax      // Load kernel data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call irq_handler    // Call C handler

    pop %eax            // Restore data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa                // Restore registers

    add $8, %esp        // Clean up error code and IRQ number
    iret                // Return from interrupt

// Generate stubs for all IRQs
IRQ_STUB 0
IRQ_STUB 1
IRQ_STUB 2
IRQ_STUB 3
IRQ_STUB 4
IRQ_STUB 5
IRQ_STUB 6
IRQ_STUB 7
IRQ_STUB 8
IRQ_STUB 9
IRQ_STUB 10
IRQ_STUB 11
IRQ_STUB 12
IRQ_STUB 13
IRQ_STUB 14
IRQ_STUB 15
