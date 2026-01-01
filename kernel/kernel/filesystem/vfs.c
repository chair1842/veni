/* Virtual File System implementation */

#include <kernel/vfs.h>
#include <kernel/io.h>
#include "ramfs.h"

// :p i hate copypasting from chatgpt

static vfs_file_t vfs_files[VFS_MAX_OPEN];
static filesystem_t *vfs_mounts[VFS_MAX_FS];
static int vfs_fs_count = 0;

// Get the filesystem for a given path
static filesystem_t* get_fs_for_path(const char *path) {
	if (vfs_fs_count == 0) return NULL;
	return vfs_mounts[0]; // for now, single FS only
}

// Allocate a file descriptor
static int alloc_fd() {
	for (int i = 0; i < VFS_MAX_OPEN; i++) {
		if (!vfs_files[i].used) {
			vfs_files[i].used = 1;
			vfs_files[i].offset = 0;
			return i;
		}
	}
	return -1;
}

// Free a file descriptor
static void free_fd(int fd) {
	if (fd >= 0 && fd < VFS_MAX_OPEN) {
		vfs_files[fd].used = 0;
	}
}

// Initialize the VFS
void vfs_init() {
	for (int i = 0; i < VFS_MAX_OPEN; i++) vfs_files[i].used = 0;
	vfs_fs_count = 0;
}

// Create a new file
int vfs_create(const char *path) {
	filesystem_t *fs = get_fs_for_path(path);
	if (!fs) return -1;

	int fs_fd = fs->ops.create(fs, path);
	if (fs_fd < 0) return -1;

	int fd = alloc_fd();
	if (fd < 0) return -1;

	vfs_files[fd].fs = fs;
	vfs_files[fd].fs_fd = fs_fd;
	io_wait();
	return fd;
}

// Open an existing file
int vfs_open(const char *path) {
	filesystem_t *fs = get_fs_for_path(path);
    if (!fs) return -1;

    int fs_fd = fs->ops.open(fs, path);
    if (fs_fd < 0) return -1;

    int fd = alloc_fd();
    if (fd < 0) return -1;
	
    vfs_files[fd].fs = fs;
    vfs_files[fd].fs_fd = fs_fd;
	io_wait();
    return fd;
}

// Read data from a file
size_t vfs_read(int fd, void *buf, size_t size) {
	if (fd < 0 || fd >= VFS_MAX_OPEN || !vfs_files[fd].used) return 0;

    vfs_file_t *f = &vfs_files[fd];
	io_wait();
    return f->fs->ops.read(f->fs, f->fs_fd, buf, size, &f->offset);
}

// Write data to a file
size_t vfs_write(int fd, const void *buf, size_t size) {
	if (fd < 0 || fd >= VFS_MAX_OPEN || !vfs_files[fd].used) return 0;

    vfs_file_t *f = &vfs_files[fd];
	io_wait();
    return f->fs->ops.write(f->fs, f->fs_fd, buf, size, &f->offset);
}

// Set the file offset
int vfs_lseek(int fd, size_t offset) {
	if (fd < 0 || fd >= VFS_MAX_OPEN || !vfs_files[fd].used) return -1;

    vfs_files[fd].offset = offset;
	io_wait();
    return 0;
}

// Mount a filesystem
int vfs_mount(filesystem_t *fs) {
    if (vfs_fs_count >= VFS_MAX_FS) return -1;
	
    vfs_mounts[vfs_fs_count++] = fs;
	io_wait();
    return 0;
}