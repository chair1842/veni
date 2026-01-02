#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
// Abort
void abort();
// Allocate dynamic memory
void *malloc(size_t size);
// Free dynamic memory
void free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif
