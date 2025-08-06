#include <kernel/pic.h>
#include <kernel/io.h>

void pic_send_eoi(uint8_t irq) {
    if(irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void irq_set_mask(uint8_t line) {
    uint16_t port;
    uint8_t value;
 
    if(line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        line -= 8;
    }
    value = inb(port) | (1 << line);
    outb(port, value);        
}

void irq_clear_mask(uint8_t line) {
    uint16_t port;
    uint8_t value;
 
    if(line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        line -= 8;
    }
    value = inb(port) & ~(1 << line);
    outb(port, value);
}

void pic_disable(void) {
    // Mask all interrupts
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_remap(uint8_t master_offset, uint8_t slave_offset) {
    uint8_t master_mask, slave_mask;

    // Save masks
    master_mask = inb(PIC1_DATA);
    slave_mask = inb(PIC2_DATA);

    // Start initialization sequence (cascade mode)
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Set vector offsets
    outb(PIC1_DATA, master_offset);    // Master PIC vector offset
    outb(PIC2_DATA, slave_offset);     // Slave PIC vector offset

    // Tell Master PIC there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC1_DATA, 4);
    // Tell Slave PIC its cascade identity (0000 0010)
    outb(PIC2_DATA, 2);

    // Set both PICs to 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // Restore saved masks
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);
}

void pic_init(void) {
    // Remap PIC vectors so they don't overlap with CPU exceptions
    // Master PIC: 0x20-0x27
    // Slave PIC: 0x28-0x2F
    pic_remap(0x20, 0x28);

    // By default, mask all interrupts
    pic_disable();
}
