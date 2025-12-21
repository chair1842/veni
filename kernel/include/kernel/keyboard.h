#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H

#include <stdbool.h>

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
};

bool keyboard_available();
char keyboard_getchar();
char keyboard_readchar();

#endif