#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/heap.h>
#include <string.h>

// Define kernel end symbol (this should be provided by the linker)
extern uint32_t kernel_end;

void kernel_early(void) {
    gdt_init();
    idt_init();
    pic_init();    // Initialize and configure the PIC
    
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
    for (int i = 0; i < 100; i++) {
        printf("%d\n", i);
        for (volatile int j = 0; j < 100000000; j++); // Simple delay for visibility
    }
}
