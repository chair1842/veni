#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/heap.h>
#include <string.h>

// Define kernel end symbol (this should be provided by the linker)
extern uint32_t kernel_end;

void kernel_early(void) {
    gdt_init();
    idt_init();
    
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
    
    // Test terminal colors
    terminal_set_fg_bg(CYAN, BLACK);
    printf("Veni OS - System Test\n");
    terminal_set_fg_bg(LIGHT_GREY, BLACK);
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
    terminal_reset_color();

    // Test division by zero interrupt
    terminal_set_fg_bg(LIGHT_RED, BLACK);
    printf("\nInterrupt Test:\n");
    printf("  Testing division by zero (INT 0)...\n");
    int a = 42;
    int b = 0;
    printf("  Attempting to divide %d by %d...\n", a, b);
    __asm__ __volatile__(
        "movl %0, %%eax\n"
        "movl %1, %%ebx\n"
        "divl %%ebx"
        : /* no outputs */
        : "r"(a), "r"(b)
        : "eax", "ebx"
    );
    // We shouldn't reach this point due to the interrupt
    printf("  This line should not be printed!\n");
}
