#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

// GDT segment descriptors
#define GDT_ENTRY_NULL    0
#define GDT_ENTRY_KERNEL_CODE 1
#define GDT_ENTRY_KERNEL_DATA 2
#define GDT_ENTRY_USER_CODE   3
#define GDT_ENTRY_USER_DATA   4
#define GDT_NUM_ENTRIES   5

// Access byte flags
#define GDT_ACCESS_PRESENT    (1 << 7)
#define GDT_ACCESS_RING0      (0 << 5)
#define GDT_ACCESS_RING3      (3 << 5)
#define GDT_ACCESS_SEGMENT    (1 << 4)
#define GDT_ACCESS_EXECUTABLE (1 << 3)
#define GDT_ACCESS_RW        (1 << 1)

// Flags nibble
#define GDT_FLAG_32BIT       (1 << 6)
#define GDT_FLAG_4K_GRAN     (1 << 7)

void gdt_init(void);

#endif
