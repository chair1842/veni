#include "keyboard.h"
#include <stdio.h>

#define DATA_PORT   0x60
#define STATUS_PORT 0x64
#define BUFFER_SIZE 128

static KeyPacket_t buffer[BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;

void buffer_push(KeyPacket_t c);
bool buffer_empty();
KeyPacket_t buffer_pop();

static bool shift = false;
static bool ctrl = false;
static bool alt = false;
static bool capslock = false;
static bool numlock = false;
static bool scrolllock = false;

// I'll move to keycode later :o
static KeyCode_t set1_scancode_to_keycode[128] = {
    0, 0, KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE,
    KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE, KEY_ZERO,
    KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_TAB, KEY_Q,
    KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O,
    KEY_P, KEY_LEFTBRACKET, KEY_RIGHTBRACKET, KEY_ENTER,
    KEY_LEFTCONTROL, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H,
    KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_SINGLEQUOTE, 
    KEY_BACKTICK, KEY_LEFTSHIFT, KEY_BACKSLASH, KEY_Z, KEY_X,
    KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, 
    KEY_SLASH, KEY_RIGHTSHIFT, 0, KEY_LEFTALT, KEY_SPACE, 0, 
    KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, 
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUMLOCK, 
    KEY_SCROLLLOCK, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, KEY_F11, KEY_F12
};

inline void buffer_push(KeyPacket_t pkt) {
    int next = (buffer_head + 1) % BUFFER_SIZE;
    if (next != buffer_tail) {
        buffer[buffer_head] = pkt;
        buffer_head = next;
    }
}

inline bool buffer_empty() {
    return buffer_head == buffer_tail;
}

inline KeyPacket_t buffer_pop() {
    KeyPacket_t pkt = {0};
    if (buffer_empty()) return pkt;

    io_wait();

    pkt = buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
    return pkt;
}

void keyboard_handler() {
    while (inb(STATUS_PORT) & 1) {
        uint8_t sc = inb(DATA_PORT);

        bool released = sc & 0x80;
        uint8_t code = sc & 0x7F;

        if (code >= 128)
            continue;

        KeyCode_t keycode = set1_scancode_to_keycode[code];
        if (keycode == KEY_NONE)
            continue;

        // Modifier handling
        switch (keycode) {
            case KEY_LEFTSHIFT:
            case KEY_RIGHTSHIFT:
                shift = !released;
                continue;

            case KEY_LEFTCONTROL:
            case KEY_RIGHTCONTROL:
                ctrl = !released;
                continue;

            case KEY_LEFTALT:
            case KEY_RIGHTALT:
                alt = !released;
                continue;

            case KEY_CAPSLOCK:
                if (!released) capslock = !capslock;
                continue;

            case KEY_NUMLOCK:
                if (!released) numlock = !numlock;
                continue;

            case KEY_SCROLLLOCK:
                if (!released) scrolllock = !scrolllock;
                continue;

            default:
                break;
        }

        // Emit KeyPacket
        KeyPacket_t pkt = {
            .keycode = keycode,
            .pressed = !released,
            .shift = shift,
            .ctrl = ctrl,
            .alt = alt,
            .capslock = capslock,
            .numlock = numlock,
            .scrolllock = scrolllock
        };

        buffer_push(pkt);
    }
}

bool keyboard_available() {
    return !buffer_empty();
}

KeyPacket_t keyboard_getkey() {
    while (buffer_empty()) {
        io_wait();
    }
    return buffer_pop();
}

KeyPacket_t keyboard_readkey() {
    if (buffer_empty()) {
        KeyPacket_t pkt = {0};
        return pkt; // No character available
    }
    return buffer_pop();
}

size_t kbd_read(vfs_filesystem_t *fs, void *data, void *buf, size_t size, size_t *offset) {
    (void)fs;
    (void)offset;
    (void)data;
    (void)size;

    if (size < sizeof(KeyPacket_t)) {
        return 0; // or error later
    }

    KeyPacket_t pkt = keyboard_getkey(); // BLOCKS
    memcpy(buf, &pkt, sizeof(pkt));
    return sizeof(pkt);
}

size_t kbd_write(vfs_filesystem_t *fs, void *data, const void *buf, size_t size, size_t *offset) {
    (void)fs; (void)data; (void)buf; (void)offset;
    return 0; // deny write
}

int kbd_close(vfs_filesystem_t *fs, void *data) {
    (void)fs; (void)data;
    return 0;
}