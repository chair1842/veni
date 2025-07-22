#ifndef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H

#include <stdint.h>

// GDT segment selectors
#define GDT_NULL_SEGMENT    0x00
#define GDT_CODE_SEGMENT    0x08
#define GDT_DATA_SEGMENT    0x10

// GDT access flags
#define GDT_PRESENT        0x80
#define GDT_RING0          0x00
#define GDT_RING3          0x60
#define GDT_SYSTEM         0x00
#define GDT_CODE_DATA      0x10
#define GDT_EXECUTABLE     0x08
#define GDT_RW             0x02
#define GDT_ACCESSED       0x01

// GDT granularity flags
#define GDT_4K_GRAN        0x80
#define GDT_32BIT          0x40

// GDT entry structure
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Function declarations
void gdt_init(void);

#endif
