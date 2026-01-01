/* VFS definitions, nothing else */
#ifndef _KERNEL_VFS_H
#define _KERNEL_VFS_H

#include <stddef.h>
#include <stdbool.h>

#define VFS_MAX_OPEN 64
#define VFS_MAX_FS 16

typedef struct filesystem filesystem_t;

// File operations structure
typedef struct file_ops {
	int  (*create)(filesystem_t *fs, const char *path);
	int  (*open)(filesystem_t *fs, const char *path);
	size_t (*read)(filesystem_t *fs, int fd, void *buf, size_t size, size_t *offset);
	size_t (*write)(filesystem_t *fs, int fd, const void *buf, size_t size, size_t *offset);
	int  (*close)(filesystem_t *fs, int fd);
	int  (*unlink)(filesystem_t *fs, const char *path);
} file_ops_t;

// VFS filesystem structure
typedef struct filesystem {
	const char *name;
	file_ops_t ops;
	void *data; // filesystem-specific data (e.g., RAMFS file array)
} filesystem_t;

typedef enum {
	VFS_TYPE_FILE,
	VFS_TYPE_DIR
} vfs_node_type_t;

typedef struct vfs_node {
	char name[32];
	vfs_node_type_t type; // file or directory
	filesystem_t *fs;
	void *fs_data;          // inode or FS-specific handle
	struct vfs_node *parent;
	struct vfs_node *children; // linked list for directories
	struct vfs_node *next; // next sibling in the linked list
} vfs_node_t;

typedef struct vfs_mount {
	vfs_node_t *node; // must be an existing directory node
	filesystem_t *fs;
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
// seek to offset in file, absolute seek only
int vfs_lseek(int fd, size_t offset);

// mount a filesystem
int vfs_mount(filesystem_t *fs);


#endif