#pragma once
#include <stddef.h>

void ramfs_init(void);
int ramfs_create(filesystem_t *fs, const char *path);
int ramfs_open(filesystem_t *fs, const char *path);
size_t ramfs_read(filesystem_t *fs, int fd, void *buf, size_t size, size_t *offset);
size_t ramfs_write(filesystem_t *fs, int fd, const void *buf, size_t size, size_t *offset);