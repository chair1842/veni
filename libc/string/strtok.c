#include <stddef.h>
#include <string.h>

// Simple strtok implementation
char *strtok(char *str, const char *delim) {
    static char *next = NULL;

    // Start from the new string or continue from previous
    if (str) next = str;
    if (!next) return NULL;

    // Skip leading delimiters
    while (*next && strchr(delim, *next)) next++;

    if (!*next) return NULL;

    char *token_start = next;

    // Find the end of the token
    while (*next && !strchr(delim, *next)) next++;

    if (*next) {
        *next = '\0';
        next++;
    }

    return token_start;
}
