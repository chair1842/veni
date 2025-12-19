    .section .text
    .code32

    .extern exception_handler
    .extern irq_handler

/* -------------------------
 * ISR stub macros
 * ------------------------- */

.macro isr_err_stub num
.globl isr_stub_\num
isr_stub_\num:
    pushl $\num        # interrupt number
    call exception_handler
    add $8, %esp       # pop int_no + error_code
    iret
.endm

.macro isr_no_err_stub num
.globl isr_stub_\num
isr_stub_\num:
    pushl $0           # fake error code
    pushl $\num
    call exception_handler
    add $8, %esp
    iret
.endm

.macro irq_stub num
.globl irq_stub_\num
irq_stub_\num:
    pushl $0              # fake error code
    pushl $\num           # vector number
    call irq_handler
    add $8, %esp
    iret
.endm


/* -------------------------
 * ISR stubs 0–31
 * ------------------------- */

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

// IRQ stubs 32–47

irq_stub 32
irq_stub 33
irq_stub 34
irq_stub 35
irq_stub 36
irq_stub 37
irq_stub 38
irq_stub 39
irq_stub 40
irq_stub 41
irq_stub 42
irq_stub 43
irq_stub 44
irq_stub 45
irq_stub 46
irq_stub 47

/* -------------------------
 * ISR stub table
 * ------------------------- */

    .section .data
    .globl isr_stub_table
isr_stub_table:
    .long isr_stub_0
    .long isr_stub_1
    .long isr_stub_2
    .long isr_stub_3
    .long isr_stub_4
    .long isr_stub_5
    .long isr_stub_6
    .long isr_stub_7
    .long isr_stub_8
    .long isr_stub_9
    .long isr_stub_10
    .long isr_stub_11
    .long isr_stub_12
    .long isr_stub_13
    .long isr_stub_14
    .long isr_stub_15
    .long isr_stub_16
    .long isr_stub_17
    .long isr_stub_18
    .long isr_stub_19
    .long isr_stub_20
    .long isr_stub_21
    .long isr_stub_22
    .long isr_stub_23
    .long isr_stub_24
    .long isr_stub_25
    .long isr_stub_26
    .long isr_stub_27
    .long isr_stub_28
    .long isr_stub_29
    .long isr_stub_30
    .long isr_stub_31
.globl irq_stub_table
irq_stub_table:
    .long irq_stub_32
    .long irq_stub_33
    .long irq_stub_34
    .long irq_stub_35
    .long irq_stub_36
    .long irq_stub_37
    .long irq_stub_38
    .long irq_stub_39
    .long irq_stub_40
    .long irq_stub_41
    .long irq_stub_42
    .long irq_stub_43
    .long irq_stub_44
    .long irq_stub_45
    .long irq_stub_46
    .long irq_stub_47
