# Defined in idt.c
.extern isr_handler

# Common ISR stub. Saves processor state, sets up for kernel mode segments,
# calls C-level fault handler, and finally restores stack frame.
.global isr_common_stub
isr_common_stub:
    pusha                   # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    movw %ds, %ax          # Lower 16-bits of eax = ds
    pushl %eax             # Save the data segment descriptor

    movw $0x10, %ax        # Load the kernel data segment descriptor
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl %esp             # Push pointer to registers_t struct as argument
    call isr_handler
    addl $4, %esp          # Clean up pushed parameter

    popl %eax              # Reload the original data segment descriptor
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    popa                   # Pops edi,esi,ebp,esp,ebx,edx,ecx,eax
    addl $8, %esp         # Cleans up the pushed error code and pushed ISR number
    iret                  # Returns from interrupt

# Create ISRs
.macro ISR_NOERRCODE num
.global isr\num
isr\num:
    pushl $0              # Push dummy error code
    pushl $\num           # Push the interrupt number
    jmp isr_common_stub
.endm

.macro ISR_ERRCODE num
.global isr\num
isr\num:
    pushl $\num           # Push the interrupt number
    jmp isr_common_stub
.endm

# Generate ISRs
ISR_NOERRCODE 0          # Division by zero
ISR_NOERRCODE 1          # Debug
ISR_NOERRCODE 2          # Non-maskable interrupt
ISR_NOERRCODE 3          # Breakpoint
ISR_NOERRCODE 4          # Overflow
ISR_NOERRCODE 5          # Bound range exceeded
ISR_NOERRCODE 6          # Invalid opcode
ISR_NOERRCODE 7          # Device not available
ISR_ERRCODE   8          # Double fault
ISR_NOERRCODE 9          # Coprocessor segment overrun
ISR_ERRCODE   10         # Invalid TSS
ISR_ERRCODE   11         # Segment not present
ISR_ERRCODE   12         # Stack-segment fault
ISR_ERRCODE   13         # General protection fault
ISR_ERRCODE   14         # Page fault
ISR_NOERRCODE 15         # Reserved
ISR_NOERRCODE 16         # x87 FPU error
ISR_ERRCODE   17         # Alignment check
ISR_NOERRCODE 18         # Machine check
ISR_NOERRCODE 19         # SIMD Floating-Point Exception
ISR_NOERRCODE 20         # Virtualization Exception
ISR_NOERRCODE 21         # Control Protection Exception

# Load IDT
.global idt_flush
idt_flush:
    movl 4(%esp), %eax    # Get the pointer to the IDT, passed as a parameter
    lidt (%eax)           # Load the IDT pointer
    ret
