.section .text
.global gdt_flush
gdt_flush:
    movl 4(%esp), %eax    # Load GDT pointer from parameter
    lgdt (%eax)           # Load new GDT pointer

    movw $0x10, %ax      # 0x10 is the data segment offset
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $flush   # 0x08 is the code segment offset
flush:
    ret
