#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/heap.h>
#include <string.h>

// Define kernel end symbol (this should be provided by the linker)
extern uint32_t kernel_end;

void kernel_early(void) {
    gdt_init();
    idt_install();
    isr_install();
    pic_init();
    irq_install();
    
    // Enable interrupts
    asm volatile("sti");
    
    // Initialize physical memory manager
    // Start after kernel end, use first 16MB for now
    pmm_init((uint32_t)&kernel_end, 16 * 1024 * 1024);
    
    // Initialize virtual memory manager
    vmm_init();
    
    // Initialize kernel heap
    kheap_init();
}

void kernel_main(void) {
    terminal_initialize();
    printf("Veni OS - Memory Management Test\n");
    printf("--------------------------------\n");
    
    // Test physical page allocation
    void* page = pmm_alloc_page();
    if (page) {
        printf("Physical Memory Test:\n");
        printf("  Allocated page at: %p\n", page);
        pmm_free_page(page);
        printf("  Successfully freed page\n");
        printf("  Free pages available: %zu\n\n", pmm_get_free_pages());
    }
    
    // Test kernel heap allocation
    printf("Kernel Heap Test:\n");
    
    // Allocate a small buffer
    char* buffer1 = kmalloc(32);
    if (buffer1) {
        printf("  Allocated 32 bytes at: %p\n", buffer1);
        strcpy(buffer1, "Hello from kernel heap!");
        printf("  Buffer contains: %s\n", buffer1);
        kfree(buffer1);
        printf("  Freed 32 byte buffer\n");
    }
    
    // Test larger allocation
    int* numbers = kmalloc(5 * sizeof(int));
    if (numbers) {
        printf("  Allocated array at: %p\n", numbers);
        for (int i = 0; i < 5; i++) {
            numbers[i] = i * i;
            printf("  numbers[%d] = %d\n", i, numbers[i]);
        }
        kfree(numbers);
        printf("  Freed array\n");
    }
}
