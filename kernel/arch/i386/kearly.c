#include "kearly.h"

void kearly(void) {
    terminal_initialize();
    printf("Kernel early initialization started.\n");
    serial_init();
    printf("Serial port initialized.\n");

    // Initialize PIC
    pic_remap(0x20, 0x28);
    printf("PIC remapped.\n");
    pic_disable();
    printf("PIC disabled.\n");

    // Initialize IDT
    idt_init();
    printf("IDT initialized.\n");

    pit_init(100); // Set PIT to 100Hz
    printf("PIT initialized to 100Hz.\n");

    // Unmask IRQs
    //irq_clear_mask(0); // PIT
    //printf("PIT IRQ unmasked.\n");
    irq_clear_mask(1); // Keyboard
    printf("Keyboard IRQ unmasked.\n");
    printf("IRQs unmasked.\n");
    
    // Initialize physical memory manager
    // Start after kernel end, use first 16MB for now
    pmm_init((uint32_t)&kernel_end, 16 * 1024 * 1024);
    printf("Physical Memory Manager initialized.\n");
    
    // Initialize virtual memory manager
    vmm_init();
    printf("Virtual Memory Manager initialized.\n");
    
    // Initialize kernel heap
    kheap_init();
    printf("Heap initialized.\n");

    vfs_init();
    printf("Virtual Filesystem initialized.\n");

    __asm__ volatile ("sti"); // Enable interrupts
    printf("Interrupts enabled.\n");

    printf("Kernel early initialization complete.\n");
    return;
}