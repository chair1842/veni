#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H

#include <stdbool.h>
#include <kernel/kb_keys.h>

bool keyboard_available();
struct KeyPacket keyboard_getkey(); // blocking
struct KeyPacket keyboard_readkey(); // non-blocking

#endif