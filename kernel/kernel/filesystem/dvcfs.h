/*
DVCFS - Device Filesystem
DVCF means Device File
*/
#ifndef KERNEL_FILESYSTEM_DVCFS_H
#define KERNEL_FILESYSTEM_DVCFS_H

#include <kernel/vfs.h>

// initialize dvcfs
void dvcfs_init();
// register device
int dvcfs_regdvc(const char *dvc_name, vfs_file_ops_t *ops, void *data);

// device file operations

int dvcfs_create(vfs_filesystem_t *fs, const char *path);
int dvcfs_open(vfs_filesystem_t *fs, const char *path);
size_t dvcfs_read(vfs_filesystem_t *fs, void* fd, void *buf, size_t size, size_t *offset);
size_t dvcfs_write(vfs_filesystem_t *fs, void *fd, const void *buf, size_t size, size_t *offset);
int dvcfs_close(vfs_filesystem_t *fs, void *fd);
int dvcfs_unlink(vfs_filesystem_t *fs, const char *path);
int dvcfs_mkdir(vfs_filesystem_t *fs, const char *path);
int dvcfs_rmdir(vfs_filesystem_t *fs, const char *path);
int dvcfs_stat(vfs_filesystem_t *fs, const char *path, vfs_stat_t *st);
#endif