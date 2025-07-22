#ifndef _KERNEL_HEAP_H
#define _KERNEL_HEAP_H

#include <stddef.h>
#include <stdint.h>

// Initialize the kernel heap
void kheap_init(void);

// Allocate memory from kernel heap
void* kmalloc(size_t size);

// Free memory allocated by kmalloc
void kfree(void* ptr);

#endif
