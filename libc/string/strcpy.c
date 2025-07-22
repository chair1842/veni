#include <string.h>

char* strcpy(char* restrict dest, const char* restrict src) {
    char* d = dest;
    while ((*d++ = *src++) != '\0')
        ;
    return dest;
}
