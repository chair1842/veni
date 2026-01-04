/* Dynamic memory allocation implementation */
#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/heap.h>
#endif

void *malloc(size_t size) {
#if defined(__is_libk)
    return kmalloc(size);
#else
    // TODO: Implement userland malloc
    return NULL; // Userland malloc not implemented
#endif
}