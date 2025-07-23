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

// Division by zero handler
static void divide_by_zero_handler(registers_t* regs) {
    printf("\nDivision by Zero Exception! Details:\n");
    printf("  Instruction pointer (EIP): 0x%x\n", regs->eip);
    printf("  Code segment (CS): 0x%x\n", regs->cs);
    printf("  Flags (EFLAGS): 0x%x\n", regs->eflags);
    printf("System halted.\n");
    for(;;); // Halt the system
}

// Example page fault handler
static void page_fault_handler(registers_t* regs) {
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    printf("Page Fault! Details:\n");
    printf("  Address: 0x%x\n", faulting_address);
    printf("  Error Code: %d\n", regs->err_code);
    printf("  Present: %d\n", (regs->err_code & 0x1));
    printf("  Write: %d\n", (regs->err_code & 0x2) >> 1);
    printf("  User-mode: %d\n", (regs->err_code & 0x4) >> 2);
    printf("  Instruction pointer (EIP): 0x%x\n", regs->eip);
}

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
    
    // Register interrupt handlers
    register_interrupt_handler(0, divide_by_zero_handler);  // Division by zero
    register_interrupt_handler(14, page_fault_handler);     // Page fault
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

    // Test division by zero interrupt
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
