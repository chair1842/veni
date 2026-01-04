#include <kernel/heap.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <string.h>

#define HEAP_START      0xD0000000
#define HEAP_END        0xE0000000
#define HEAP_MAX_SIZE   (HEAP_END - HEAP_START)
#define BLOCK_MAGIC     0x464E4942  // "BINF" in ASCII

typedef struct block_header {
    uint32_t magic;              // Magic number to detect corruption
    size_t size;                // Size of the block (including header)
    uint8_t is_free;            // 1 if block is free, 0 if used
    struct block_header* next;   // Next block in the list
    struct block_header* prev;   // Previous block in the list
} block_header_t;

static block_header_t* heap_start = NULL;
static uint32_t heap_current_break = HEAP_START;

// Helper function to extend the heap
static void* sbrk(size_t size) {
    if (heap_current_break + size > HEAP_END) {
        return NULL;  // Out of memory
    }

    uint32_t old_break = heap_current_break;
    heap_current_break += size;

    // Map the new pages
    for (uint32_t addr = old_break; addr < heap_current_break; addr += PAGE_SIZE) {
        void* page = pmm_alloc_page();
        if (!page) {
            // Out of physical memory, roll back
            heap_current_break = old_break;
            return NULL;
        }
        map_page(addr, (uint32_t)page, PTE_PRESENT | PTE_RW);
    }

    return (void*)old_break;
}

void kheap_init() {
    // Create initial heap block
    heap_start = sbrk(PAGE_SIZE);
    if (!heap_start) {
        return;  // Failed to initialize heap
    }

    heap_start->magic = BLOCK_MAGIC;
    heap_start->size = PAGE_SIZE;
    heap_start->is_free = 1;
    heap_start->next = NULL;
    heap_start->prev = NULL;
}

static block_header_t* find_free_block(size_t size) {
    block_header_t* current = heap_start;
    while (current) {
        if (current->magic != BLOCK_MAGIC) {
            // Memory corruption detected
            return NULL;
        }
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void split_block(block_header_t* block, size_t size) {
    // Only split if we can create a new block of at least sizeof(block_header_t)
    if (block->size > size + sizeof(block_header_t) + 16) {
        block_header_t* new_block = (block_header_t*)((uint8_t*)block + size);
        new_block->magic = BLOCK_MAGIC;
        new_block->size = block->size - size;
        new_block->is_free = 1;
        new_block->next = block->next;
        new_block->prev = block;
        
        if (block->next) {
            block->next->prev = new_block;
        }
        
        block->next = new_block;
        block->size = size;
    }
}

void* kmalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Align size to 8 bytes and add space for header
    size = ((size + 7) & ~7) + sizeof(block_header_t);

    block_header_t* block = find_free_block(size);
    if (!block) {
        // No suitable block found, extend heap
        size_t alloc_size = size > PAGE_SIZE ? size : PAGE_SIZE;
        block = sbrk(alloc_size);
        if (!block) {
            return NULL;
        }

        block->magic = BLOCK_MAGIC;
        block->size = alloc_size;
        block->is_free = 1;
        block->next = NULL;
        block->prev = NULL;

        // Insert into list
        if (!heap_start) {
            heap_start = block;
        } else {
            block_header_t* current = heap_start;
            while (current->next) {
                current = current->next;
            }
            current->next = block;
            block->prev = current;
        }
    }

    // Split block if it's too large
    split_block(block, size);

    block->is_free = 0;
    return (void*)((uint8_t*)block + sizeof(block_header_t));
}

static void merge_free_blocks(block_header_t* block) {
    // Merge with next block if it's free
    while (block->next && block->next->is_free) {
        block->size += block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }

    // Merge with previous block if it's free
    if (block->prev && block->prev->is_free) {
        block->prev->size += block->size;
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
        block = block->prev;
    }
}

void kfree(void* ptr) {
    if (!ptr) {
        return;
    }

    block_header_t* block = (block_header_t*)((uint8_t*)ptr - sizeof(block_header_t));
    
    if (block->magic != BLOCK_MAGIC) {
        // Memory corruption or invalid pointer
        return;
    }

    block->is_free = 1;
    merge_free_blocks(block);
}
