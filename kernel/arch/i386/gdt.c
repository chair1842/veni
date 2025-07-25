#include <kernel/gdt.h>

// GDT entries
static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

// External assembly function to load GDT
extern void gdt_flush(uint32_t);

// Set a GDT gate
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);

    gdt[num].access = access;
}

// Initialize GDT
void gdt_init(void) {
    // Setup GDT pointer and limit
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;

    // NULL descriptor
    gdt_set_gate(0, 0, 0, 0, 0);

    // Code segment
    gdt_set_gate(1, 0, 0xFFFFFFFF,
        GDT_PRESENT | GDT_RING0 | GDT_CODE_DATA | GDT_EXECUTABLE | GDT_RW,
        GDT_4K_GRAN | GDT_32BIT);

    // Data segment
    gdt_set_gate(2, 0, 0xFFFFFFFF,
        GDT_PRESENT | GDT_RING0 | GDT_CODE_DATA | GDT_RW,
        GDT_4K_GRAN | GDT_32BIT);

    // Load GDT
    gdt_flush((uint32_t)&gp);
}