#include "keyboard.h"

inline void buffer_push(char c) {
    int next = (buffer_head + 1) % BUFFER_SIZE;
    if (next != buffer_tail) {
        buffer[buffer_head] = c;
        buffer_head = next;
    }
}

inline bool buffer_empty() {
    return buffer_head == buffer_tail;
}

inline char buffer_pop() {
    if (buffer_empty()) return 0;
    char c = buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
    return c;
}

void keyboard_handler() {
    // Read the scancode from the keyboard data port
    uint8_t scancode = inb(0x60);
}

