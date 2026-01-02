#pragma once
#include <stddef.h>

void ramfs_init();
int ramfs_create(vfs_filesystem_t *fs, const char *path);
int ramfs_open(vfs_filesystem_t *fs, const char *path);
size_t ramfs_read(vfs_filesystem_t *fs, int fd, void *buf, size_t size, size_t *offset);
size_t ramfs_write(vfs_filesystem_t *fs, int fd, const void *buf, size_t size, size_t *offset);
int ramfs_close(vfs_filesystem_t *fs, int fd);
int ramfs_unlink(vfs_filesystem_t *fs, const char *path);