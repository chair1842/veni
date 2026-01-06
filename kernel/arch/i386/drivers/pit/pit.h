#pragma once
#include <stdint.h>
#include <kernel/timer.h>
#include <kernel/io.h>
#include <kernel/vfs.h>

#define PIT_CHANNEL0    0x40
#define PIT_COMMAND     0x43
#define PIT_MODE 0x36  // channel0, lobyte/hibyte, mode3, binary

void pit_handler();
void pit_init(uint32_t frequency);
size_t timer_read(vfs_filesystem_t *fs, void *data, void *buf, size_t size, size_t *offset);
size_t timer_write(vfs_filesystem_t *fs, void *data, const void *buf, size_t size, size_t *offset);
int timer_close(vfs_filesystem_t *fs, void *data);