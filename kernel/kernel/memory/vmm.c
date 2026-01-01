#include <kernel/vmm.h>
#include <kernel/pmm.h>

// Page directory structure
typedef struct {
    uint32_t entries[1024];
} page_directory_t;

// Page table structure
typedef struct {
    uint32_t entries[1024];
} page_table_t;

static page_directory_t* kernel_directory;

static void page_fault_handler();

void vmm_init() {
    // Allocate kernel page directory
    kernel_directory = (page_directory_t*)pmm_alloc_page();
    
    // Clear the page directory
    for (int i = 0; i < 1024; i++) {
        kernel_directory->entries[i] = 0;
    }

    // Identity map first 4MB of memory (kernel space)
    for (uint32_t i = 0; i < 1024; i++) {
        map_page(i * PAGE_SIZE, i * PAGE_SIZE, PTE_PRESENT | PTE_RW);
    }

    // Load page directory
    asm volatile("mov %0, %%cr3":: "r"(kernel_directory));

    // Enable paging
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags) {
    uint32_t pd_index = virtual_addr >> 22;
    uint32_t pt_index = (virtual_addr >> 12) & 0x3FF;

    // Check if page table exists
    if (!(kernel_directory->entries[pd_index] & PDE_PRESENT)) {
        // Create new page table
        page_table_t* new_table = (page_table_t*)pmm_alloc_page();
        kernel_directory->entries[pd_index] = ((uint32_t)new_table) | PDE_PRESENT | PDE_RW;
        
        // Clear the new table
        for (int i = 0; i < 1024; i++) {
            ((page_table_t*)((uint32_t)new_table & ~0xFFF))->entries[i] = 0;
        }
    }

    // Get page table
    page_table_t* table = (page_table_t*)(kernel_directory->entries[pd_index] & ~0xFFF);
    
    // Map the page
    table->entries[pt_index] = (physical_addr & ~0xFFF) | flags;

    // Invalidate TLB entry
    asm volatile("invlpg (%0)" : : "r"(virtual_addr));
}

void unmap_page(uint32_t virtual_addr) {
    uint32_t pd_index = virtual_addr >> 22;
    uint32_t pt_index = (virtual_addr >> 12) & 0x3FF;

    if (kernel_directory->entries[pd_index] & PDE_PRESENT) {
        page_table_t* table = (page_table_t*)(kernel_directory->entries[pd_index] & ~0xFFF);
        table->entries[pt_index] = 0;
        
        // Invalidate TLB entry
        asm volatile("invlpg (%0)" : : "r"(virtual_addr));
    }
}

uint32_t get_physical_address(uint32_t virtual_addr) {
    uint32_t pd_index = virtual_addr >> 22;
    uint32_t pt_index = (virtual_addr >> 12) & 0x3FF;
    uint32_t offset = virtual_addr & 0xFFF;

    if (!(kernel_directory->entries[pd_index] & PDE_PRESENT)) {
        return 0;
    }

    page_table_t* table = (page_table_t*)(kernel_directory->entries[pd_index] & ~0xFFF);
    
    if (!(table->entries[pt_index] & PTE_PRESENT)) {
        return 0;
    }

    return (table->entries[pt_index] & ~0xFFF) | offset;
}
