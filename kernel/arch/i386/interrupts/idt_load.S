.section .text
.global idt_load

idt_load:
    # Load the IDT pointer
    movl 4(%esp), %eax
    lidt (%eax)
    ret
