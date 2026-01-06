#include <kernel/kb_keys.h>
#include <stdio.h>

static char keycode_to_ascii[74] = {
    0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
    'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', ' ', '[', ']', ';', '\'',
    ',', '.', '/', '\\', '`', '\n', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char keycode_to_ascii_shift[74] = {
    0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U',
    'V', 'W', 'X', 'Y', 'Z', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', ' ', '{', '}', ':', '"',
    '<', '>', '?', '|', '~', '\n', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char keycode_to_ascii_caps[74] = {
    0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U',
    'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', ' ', '[', ']', ';', '\'',
    ',', '.', '/', '\\', '`', '\n', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char key_to_ascii(KeyPacket_t pkt) {
    if (!pkt.pressed)
    return 0;

    bool is_letter = (pkt.keycode >= KEY_A && pkt.keycode <= KEY_Z);

    if (is_letter && (pkt.capslock ^ pkt.shift)) {
        return keycode_to_ascii_caps[pkt.keycode];
    }

    if (pkt.shift) {
        return keycode_to_ascii_shift[pkt.keycode];
    }

    return keycode_to_ascii[pkt.keycode];
}