#pragma once

#include <kernel/keyboard.h>
#include <stdint.h>
#include <kernel/io.h>

#define DATA_PORT   0x60
#define STATUS_PORT 0x64
#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;

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

static char scancode_to_ascii[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',
    0,'*', 0,' ', 0,
};

void buffer_push(char c);

bool buffer_empty();

char buffer_pop();

void keyboard_handler();