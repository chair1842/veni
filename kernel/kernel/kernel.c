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
    printf("Initializing kernel...\n");
    
    printf("Initializing GDT...\n");
    gdt_init();
    
    printf("Installing IDT...\n");
    idt_install();
    
    printf("Installing ISRs...\n");
    isr_install();
    
    printf("Initializing PIC...\n");
    pic_init();
    
    printf("Installing IRQs...\n");
    irq_install();
    
    printf("Enabling interrupts...\n");
    // Enable interrupts
    asm volatile("sti");
    printf("Interrupts enabled\n");
    
    // Initialize physical memory manager
    // Start after kernel end, use first 16MB for now
    printf("Initializing physical memory manager...\n");
    pmm_init((uint32_t)&kernel_end, 16 * 1024 * 1024);
    
    // Initialize virtual memory manager
    vmm_init();
    
    // Initialize kernel heap
    kheap_init();
}

void kernel_main(void) {
    terminal_initialize();
}
