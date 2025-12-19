#pragma once
#include <stdint.h>
#include <kernel/timer.h>
#include <kernel/io.h>

#define PIT_CHANNEL0    0x40
#define PIT_COMMAND     0x43
#define PIT_MODE 0x36  // channel0, lobyte/hibyte, mode3, binary

void pit_handler();
void pit_init(uint32_t frequency);