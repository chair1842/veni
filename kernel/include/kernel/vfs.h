/* VFS definitions, nothing else */
#ifndef _KERNEL_VFS_H
#define _KERNEL_VFS_H

#include <stddef.h>
#include <stdbool.h>

#define VFS_MAX_OPEN 64
#define VFS_MAX_FS 16

// Forward declaration for vfs_filesystem_t
typedef struct vfs_filesystem vfs_filesystem_t;

typedef enum {
    VFS_TYPE_FILE,
    VFS_TYPE_DIR
} vfs_node_type_t;

typedef struct vfs_stat {
    vfs_node_type_t type;
    size_t size;
} vfs_stat_t;

// File operations structure
typedef struct vfs_file_ops {
    int  (*create)(vfs_filesystem_t *fs, const char *path);
    int  (*open)(vfs_filesystem_t *fs, const char *path);
    size_t (*read)(vfs_filesystem_t *fs, void* fd, void *buf, size_t size, size_t *offset);
    size_t (*write)(vfs_filesystem_t *fs, void *fd, const void *buf, size_t size, size_t *offset);
    int  (*close)(vfs_filesystem_t *fs, void *fd);
    int  (*unlink)(vfs_filesystem_t *fs, const char *path);
    int  (*mkdir)(vfs_filesystem_t *fs, const char *path);
    int  (*rmdir)(vfs_filesystem_t *fs, const char *path);
    int  (*stat)(vfs_filesystem_t *fs, const char *path, vfs_stat_t *st);
} vfs_file_ops_t;

// VFS filesystem structure
typedef struct vfs_filesystem {
    const char *name;
    vfs_file_ops_t ops;
    void *data; // filesystem-specific data (e.g., RAMFS file array)
} vfs_filesystem_t;

typedef struct vfs_node {
    char name[32];
    vfs_node_type_t type; // file or directory
    vfs_filesystem_t *fs;
    void *fs_data;          // inode or FS-specific handle
	void *fs_fd;           // file descriptor in the underlying FS
    struct vfs_node *parent;
    struct vfs_node *children; // linked list for directories
    struct vfs_node *next; // next sibling in the linked list
} vfs_node_t;

typedef struct vfs_mount {
    char mount_path[256]; // the path where the filesystem is mounted
    vfs_node_t *node; // the root node of the mounted filesystem
    vfs_filesystem_t *fs;
} vfs_mount_t;

// VFS file structure
typedef struct vfs_file {
    vfs_node_t *node;
    size_t offset;
    bool used;
} vfs_file_t;

// create a file, return VFS fd
int vfs_create(const char *path);
// clear all vfs structures and prepare for use
void vfs_init();
// open a file, return VFS fd
int vfs_open(const char *path);
// read from a file and return number of bytes read
size_t vfs_read(int fd, void *buf, size_t size);
// write to a file and return number of bytes written
size_t vfs_write(int fd, const void *buf, size_t size);
// close a file
int vfs_close(int fd);
// delete a file
int vfs_unlink(const char *path);
// create a directory
int vfs_mkdir(const char *path);
// remove a directory
int vfs_rmdir(const char *path);
// get file metadata
int vfs_stat(const char *path, vfs_stat_t *st);
// seek to offset in file, absolute seek only
int vfs_lseek(int fd, size_t offset);

// mount a filesystem
int vfs_mount(vfs_filesystem_t *fs, const char *path);
// unmount a filesystem
int vfs_unmount(const char *path);
// resolve a path to a vfs_node_t pointer
vfs_node_t *vfs_resolve(const char *path);

#endif