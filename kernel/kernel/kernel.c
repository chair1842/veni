#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>

// Define kernel end symbol (this should be provided by the linker)
extern uint32_t kernel_end;

void kernel_early(void) {
    gdt_init();
    
    // Initialize physical memory manager
    // Start after kernel end, use first 16MB for now
    pmm_init((uint32_t)&kernel_end, 16 * 1024 * 1024);
    
    // Initialize virtual memory manager
    vmm_init();
}

void kernel_main(void) {
    terminal_initialize();
    printf("Hello, kernel World!\n");
    
    // Test memory allocation
    void* page = pmm_alloc_page();
    if (page) {
        printf("Allocated physical page at %p\n", page);
        pmm_free_page(page);
        printf("Freed physical page\n");
    }
    
    printf("Free pages: %zu\n", pmm_get_free_pages());
}
