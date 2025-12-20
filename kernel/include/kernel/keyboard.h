#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H

#include <stdbool.h>

bool keyboard_available();
char keyboard_getchar();
char keyboard_readchar();

#endif