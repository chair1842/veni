#ifndef _KERNEL_PMM_H
#define _KERNEL_PMM_H

#include <stddef.h>
#include <stdint.h>

// Size of a page (4KB)
#define PAGE_SIZE 4096

// Function declarations for Physical Memory Manager
void pmm_init(uint32_t start, uint32_t size);
void* pmm_alloc_page(void);
void pmm_free_page(void* page);
size_t pmm_get_free_pages(void);

#endif
