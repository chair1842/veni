#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H

#include <stdbool.h>
#include <kernel/kb_keys.h>

bool keyboard_available();
KeyPacket_t keyboard_getkey(); // blocking
KeyPacket_t keyboard_readkey(); // non-blocking

#endif