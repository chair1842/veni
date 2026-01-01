#ifndef _KERNEL_VFS_H
#define _KERNEL_VFS_H

#include <stddef.h>

#define VFS_MAX_OPEN 64
#define VFS_MAX_FS 16

typedef struct filesystem filesystem_t;

typedef struct file_ops {
    int  (*create)(filesystem_t *fs, const char *path);
    int  (*open)(filesystem_t *fs, const char *path);
    size_t (*read)(filesystem_t *fs, int fd, void *buf, size_t size, size_t *offset);
    size_t (*write)(filesystem_t *fs, int fd, const void *buf, size_t size, size_t *offset);
} file_ops_t;

struct filesystem {
    const char *name;
    file_ops_t ops;
    void *data; // filesystem-specific data (e.g., RAMFS file array)
};

typedef struct vfs_file {
    filesystem_t *fs;   // which FS this file belongs to
    int fs_fd;          // file handle inside the FS
    size_t offset;      // read/write offset
    int used;           // is this descriptor in use
} vfs_file_t;

int vfs_create(const char *path);
void vfs_init();
int vfs_open(const char *path);
size_t vfs_read(int fd, void *buf, size_t size);
size_t vfs_write(int fd, const void *buf, size_t size);
int vfs_lseek(int fd, size_t offset);

int vfs_mount(filesystem_t *fs);


#endif