#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/heap.h>
#endif

void free(void *ptr) {
#if defined(__is_libk)
    kfree(ptr);
#else
    // TODO: Implement userland free
#endif
}