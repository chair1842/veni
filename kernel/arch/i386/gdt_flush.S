.section .text
.global gdt_flush
.type gdt_flush, @function
gdt_flush:
    movl 4(%esp), %eax    # Get the pointer to the GDT, passed as a parameter
    lgdt (%eax)           # Load the GDT pointer

    movw $0x10, %ax      # 0x10 is the offset in the GDT to our data segment
    movw %ax, %ds        # Load all data segment selectors
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $flush   # 0x08 is the offset to our code segment: far jump
flush:
    ret
