#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
char *strcpy(char* __restrict, const char* __restrict);
int strncmp(const char *s1, const char *s2, register size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strtok(char *str, const char *delim);
char *strchr(const char *s, int c);
int strcmp(const char *s1, const char *s2);
char *strrchr(const char *s, int c);

#ifdef __cplusplus
}
#endif

#endif
