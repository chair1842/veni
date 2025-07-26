#include <kernel/pic.h>
#include <stdint.h>

/* These functions need to be implemented separately for your architecture */
extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);

static uint16_t pic_mask = 0xFFFF; /* All interrupts masked by default */

void pic_init(void) {
    uint8_t a1, a2;

    /* Save masks */
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    /* Start initialization sequence in cascade mode */
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    /* Set vector offset for master PIC (IRQ 0-7 -> INT 0x20-0x27) */
    outb(PIC1_DATA, 0x20);
    /* Set vector offset for slave PIC (IRQ 8-15 -> INT 0x28-0x2F) */
    outb(PIC2_DATA, 0x28);

    /* Tell master PIC there is a slave PIC at IRQ2 */
    outb(PIC1_DATA, 4);
    /* Tell slave PIC its cascade identity */
    outb(PIC2_DATA, 2);

    /* Set both PICs to 8086 mode */
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    /* Restore saved masks */
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_send_eoi(unsigned char irq) {
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_set_mask(uint16_t mask) {
    pic_mask = mask;
    outb(PIC1_DATA, mask & 0xFF);
    outb(PIC2_DATA, (mask >> 8) & 0xFF);
}

uint16_t pic_get_mask(void) {
    return pic_mask;
}

void pic_disable(void) {
    pic_set_mask(0xFFFF); /* Mask all interrupts */
}
