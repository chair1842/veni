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

    if (scancode & 0x80) return;

    // Convert scancode to ASCII character
    char ascii = scancode_to_ascii[scancode];
    if (ascii) {
        buffer_push(ascii);
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