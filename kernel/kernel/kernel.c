#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/heap.h>
#include <kernel/serial.h>
#include <string.h>

// Define kernel end symbol (this should be provided by the linker)
extern uint32_t kernel_end;

void kernel_early(void) {
    terminal_initialize();
    printf("Kernel early initialization started.\n");
    serial_init();
    printf("Serial port initialized.\n");
    
    // Enable interrupts
    __asm__ volatile("sti");
    printf("Interrupts enabled.\n");
    
    // Initialize physical memory manager
    // Start after kernel end, use first 16MB for now
    pmm_init((uint32_t)&kernel_end, 16 * 1024 * 1024);
    printf("Physical Memory Manager initialized.\n");
    
    // Initialize virtual memory manager
    vmm_init();
    printf("Virtual Memory Manager initialized.\n");
    
    // Initialize kernel heap
    kheap_init();
    printf("Kernel early initialization complete.\n");
    return;
}

void kernel_main(void) {
    terminal_clear();
    printf("Veni says hello\n");
}
