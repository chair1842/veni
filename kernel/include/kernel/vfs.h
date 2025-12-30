#ifndef _KERNEL_VFS_H
#define _KERNEL_VFS_H

#include <stddef.h>

int vfs_create(const char *name);
int vfs_open(const char *name);
size_t vfs_read(int fd, void *buf, size_t size);
size_t vfs_write(int fd, const void *buf, size_t size);
void vfs_init(void);

#endif