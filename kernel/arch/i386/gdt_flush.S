.section .text
.global gdt_flush

# gdt_flush(uint32_t gdt_ptr)
gdt_flush:
    movl 4(%esp), %eax    # Get the pointer to the GDT
    lgdt (%eax)           # Load the GDT

    # Reload segment registers
    movw $0x10, %ax      # 0x10 is the offset to our data segment
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $flush   # 0x08 is the offset to our code segment
flush:
    ret
