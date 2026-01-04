/* RAMFS definitions */
#ifndef KERNEL_FILESYSTEM_RAMFS_H
#define KERNEL_FILESYSTEM_RAMFS_H
#include <stddef.h>

void ramfs_init();
int ramfs_create(vfs_filesystem_t *fs, const char *path);
int ramfs_open(vfs_filesystem_t *fs, const char *path);
size_t ramfs_read(vfs_filesystem_t *fs, void* fd, void *buf, size_t size, size_t *offset);
size_t ramfs_write(vfs_filesystem_t *fs, void *fd, const void *buf, size_t size, size_t *offset);
int ramfs_close(vfs_filesystem_t *fs, void *fd);
int ramfs_unlink(vfs_filesystem_t *fs, const char *path);
int ramfs_mkdir(vfs_filesystem_t *fs, const char *path);
int ramfs_rmdir(vfs_filesystem_t *fs, const char *path);
int ramfs_stat(vfs_filesystem_t *fs, const char *path, vfs_stat_t *st);

#endif