#include <kernel/pmm.h>

static uint32_t* bitmap;
static uint32_t total_pages;
static uint32_t used_pages;

#define BITMAP_INDEX(x) ((x) / 32)
#define BITMAP_BIT(x) ((x) % 32)

void pmm_init(uint32_t start, uint32_t size) {
    bitmap = (uint32_t*)start;
    total_pages = size / PAGE_SIZE;
    used_pages = 0;

    // Initialize bitmap - mark all pages as free
    for (uint32_t i = 0; i < BITMAP_INDEX(total_pages); i++) {
        bitmap[i] = 0;
    }
}

void* pmm_alloc_page() {
    if (used_pages >= total_pages) {
        return NULL;  // Out of memory
    }

    // Find first free page
    for (uint32_t i = 0; i < total_pages; i++) {
        if (!(bitmap[BITMAP_INDEX(i)] & (1 << BITMAP_BIT(i)))) {
            bitmap[BITMAP_INDEX(i)] |= (1 << BITMAP_BIT(i));
            used_pages++;
            return (void*)(i * PAGE_SIZE);
        }
    }

    return NULL;
}

void pmm_free_page(void* page) {
    uint32_t page_num = (uint32_t)page / PAGE_SIZE;
    
    if (page_num >= total_pages) {
        return;  // Invalid page
    }

    if (bitmap[BITMAP_INDEX(page_num)] & (1 << BITMAP_BIT(page_num))) {
        bitmap[BITMAP_INDEX(page_num)] &= ~(1 << BITMAP_BIT(page_num));
        used_pages--;
    }
}

size_t pmm_get_free_pages() {
    return total_pages - used_pages;
}
