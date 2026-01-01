#ifndef _KERNEL_VMM_H
#define _KERNEL_VMM_H

#include <stdint.h>

// Page directory entry flags
#define PDE_PRESENT 0x1
#define PDE_RW      0x2
#define PDE_USER    0x4
#define PDE_SIZE    0x80

// Page table entry flags
#define PTE_PRESENT 0x1
#define PTE_RW      0x2
#define PTE_USER    0x4

// Function declarations for Virtual Memory Manager
void vmm_init();
void map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
void unmap_page(uint32_t virtual_addr);
uint32_t get_physical_address(uint32_t virtual_addr);

#endif
