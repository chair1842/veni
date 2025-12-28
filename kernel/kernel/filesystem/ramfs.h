#pragma once
#include <stddef.h>

void ramfs_init(void);
int ramfs_create(const char *name);
int ramfs_open(const char *name);
size_t ramfs_read(int fd, void *buf, size_t size);
size_t ramfs_write(int fd, const void *buf, size_t size);
