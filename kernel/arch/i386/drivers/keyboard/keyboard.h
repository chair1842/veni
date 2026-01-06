#pragma once

#include <kernel/keyboard.h>
#include <stdint.h>
#include <kernel/io.h>
#include <kernel/vfs.h>
#include <string.h>

void keyboard_handler();
void keyboard_flush();
size_t kbd_read(vfs_filesystem_t *fs, void *data, void *buf, size_t size, size_t *offset);
size_t kbd_write(vfs_filesystem_t *fs, void *data, const void *buf, size_t size, size_t *offset);
int kbd_close(vfs_filesystem_t *fs, void *data);