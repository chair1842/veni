#ifndef KERNEL_KB_KEYS_H
#define KERNEL_KB_KEYS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum KeyCode {
    KEY_NONE,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_ONE,
    KEY_TWO,
    KEY_THREE,
    KEY_FOUR,
    KEY_FIVE,
    KEY_SIX,
    KEY_SEVEN,
    KEY_EIGHT,
    KEY_NINE,
    KEY_ZERO,
    KEY_MINUS,
    KEY_EQUAL,
    KEY_SPACE,
    KEY_LEFTBRACKET,
    KEY_RIGHTBRACKET,
    KEY_SEMICOLON,
    KEY_SINGLEQUOTE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_BACKSLASH,
    KEY_BACKTICK,
    KEY_ENTER,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_ESCAPE,
    KEY_CAPSLOCK,
    KEY_LEFTSHIFT,
    KEY_RIGHTSHIFT,
    KEY_LEFTCONTROL,
    KEY_RIGHTCONTROL,
    KEY_LEFTALT,
    KEY_RIGHTALT,
    KEY_DELETE,
    KEY_SCROLLLOCK,
    KEY_NUMLOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
} KeyCode_t;

typedef struct KeyPacket {
    KeyCode_t keycode;
    bool pressed; // false if released
    bool shift;
    bool ctrl;
    bool alt;
    bool capslock;
    bool scrolllock;
    bool numlock;
} KeyPacket_t;

char key_to_ascii(KeyPacket_t pkt);

#endif