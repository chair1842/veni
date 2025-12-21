#include "keyboard.h"

#define DATA_PORT   0x60
#define STATUS_PORT 0x64
#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;

void buffer_push(char c);
bool buffer_empty();
char buffer_pop();

// I'll move to keycode later :o
static KeyCode scancode_to_keycode[128] = {
    0, 0, KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE,
    KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE, KEY_ZERO,
    KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_TAB, KEY_Q,
    KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O,
    KEY_P, KEY_LEFTBRACKET, KEY_RIGHTBRACKET, KEY_ENTER,
    0, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J,
    KEY_K, KEY_L, KEY_SEMICOLON, KEY_SINGLEQUOTE, KEY_BACKTICK,
    0, KEY_BACKSLASH, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B,
    KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH,
    0, 0, 0, KEY_SPACE, 0,
};

static char keycode_to_ascii[50] = {
    0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
    'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', ' ', '[', ']', ';', '\'',
    ',', '.', '/', '`', '\\', '\n', '\b', '\t'
};

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

        KeyCode keycode = 0;
        if (code < 128) {
            keycode = scancode_to_keycode[code];
        }

        if (keycode != KEY_NONE) {
            char c = keycode_to_ascii[keycode];
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