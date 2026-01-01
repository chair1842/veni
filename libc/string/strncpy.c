#include <stddef.h>
#include <string.h>

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;

    // Copy until we hit n or a null byte
    for (; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // Pad the rest with '\0'
    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}
