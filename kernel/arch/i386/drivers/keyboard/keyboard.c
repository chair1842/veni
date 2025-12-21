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
    while (inb(STATUS_PORT) & 1) {
        uint8_t sc = inb(DATA_PORT);

        bool key_released = sc & 0x80;
        uint8_t code = sc & 0x7F;

        if (key_released)
            continue;

        char c = 0;
        if (code < 128) {
            c = scancode_to_ascii[code];
        }

        if (c) {
            buffer_push(c);
        }
    }
}



bool keyboard_available() {
    return !buffer_empty();
}

char keyboard_getchar() {
    while (buffer_empty()) {
        asm volatile ("hlt");
    }
    return buffer_pop();
}

char keyboard_readchar() {
    if (buffer_empty()) {
        return 0; // No character available
    }
    return buffer_pop();
}