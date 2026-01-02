/* Virtual File System implementation */

#include <kernel/vfs.h>
#include <kernel/io.h>
#include "ramfs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// new implementation!!!

static vfs_file_t vfs_files[VFS_MAX_OPEN];

static vfs_mount_t vfs_mounts[VFS_MAX_FS];
static int vfs_mount_count;

// Helper: find child node by name in linked-list
static vfs_node_t *find_child(vfs_node_t *parent, const char *name) {
    if (!parent || !parent->children) return NULL;

    vfs_node_t *child = parent->children;
    while (child) {
        if (strcmp(child->name, name) == 0) return child;
        child = child->next;
    }
    return NULL;
}

// Resolve a path like "/dir/sub/file"
vfs_node_t *vfs_resolve(const char *path) {
    if (!path || path[0] != '/' || vfs_mount_count == 0) return NULL;

    // Start from root of first mount
    vfs_node_t *node = vfs_mounts[0].node;
    if (!node) return NULL;

    // Duplicate path for strtok
    char temp[256];
    strncpy(temp, path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *token = strtok(temp, "/");
    while (token != NULL) {
        node = find_child(node, token);
        if (!node) return NULL;
        token = strtok(NULL, "/");
    }

    return node;
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
	for (int i = 0; i < VFS_MAX_OPEN; i++) {
		vfs_files[i].used = 0;
	}
	vfs_mount_count = 0;
}

// Create a new file, path must start with '/' and be absolute
int vfs_create(const char *path) {
    if (!path || path[0] != '/' || vfs_mount_count == 0) {
        return -1;
    }

    // Split path into components
    char temp[256];
    strncpy(temp, path, sizeof(temp)-1);
    temp[sizeof(temp)-1] = '\0';

    vfs_mount_t *mount = &vfs_mounts[0];
    vfs_node_t *current = mount->node; // start at root

    char *token = strtok(temp, "/");
    char *next_token = NULL;
    char *prev_token = NULL;

    // Keep last token for the file itself
    while (token) {
        next_token = strtok(NULL, "/");
        if (!next_token) break; // last component = file
        // Intermediate directories
        vfs_node_t *child = find_child(current, token);

        if (!child) {
            // Create directory node
            child = malloc(sizeof(vfs_node_t));
            if (!child) return -1;
            strncpy(child->name, token, sizeof(child->name)-1);
            child->name[sizeof(child->name)-1] = '\0';
            child->type = VFS_TYPE_DIR;
            child->parent = current;
            child->children = NULL;
            child->fs = mount->fs;
            child->next = NULL;

            // Add to parent's children
            if (!current->children)
                current->children = child;
            else {
                vfs_node_t *last = current->children;
                while (last->next) last = last->next;
                last->next = child;
            }
        }

        current = child;
        token = next_token;
    }

    // Now token = last component = file
    if (find_child(current, token)) return -1; // file already exists

    int fs_fd = mount->fs->ops.create(mount->fs, path);
    if (fs_fd < 0) return -1;

    vfs_node_t *file_node = malloc(sizeof(vfs_node_t));
    if (!file_node) return -1;

    strncpy(file_node->name, token, sizeof(file_node->name)-1);
    file_node->name[sizeof(file_node->name)-1] = '\0';
    file_node->type = VFS_TYPE_FILE;
    file_node->fs = mount->fs;
    file_node->fs_fd = fs_fd;
    file_node->parent = current;
    file_node->children = NULL;
    file_node->next = NULL;

    // Add file to parent
    if (!current->children)
        current->children = file_node;
    else {
        vfs_node_t *last = current->children;
        while (last->next) last = last->next;
        last->next = file_node;
    }

    // Allocate VFS FD
    int fd = alloc_fd();
    if (fd < 0) return -1;
    vfs_files[fd].node = file_node;

    return fd;
}

// Open an existing file
int vfs_open(const char *path) {
    vfs_node_t *node = vfs_resolve(path);
    if (!node) return -1;

    int fd = alloc_fd();
    if (fd < 0) return -1;

    vfs_files[fd].node = node;
    return fd;
}

// Read data from a file
size_t vfs_read(int fd, void *buf, size_t size) {
    if (fd < 0 || fd >= VFS_MAX_OPEN || !vfs_files[fd].used) {
        return 0;
	}

    vfs_file_t *f = &vfs_files[fd];
    return f->node->fs->ops.read(f->node->fs, f->node->fs_fd, buf, size, &f->offset);
}

// Write data to a file
size_t vfs_write(int fd, const void *buf, size_t size) {
    if (fd < 0 || fd >= VFS_MAX_OPEN || !vfs_files[fd].used) {
        return 0;
	}

    vfs_file_t *f = &vfs_files[fd];
    return f->node->fs->ops.write(f->node->fs, f->node->fs_fd, buf, size, &f->offset);
}

// Set the file offset
int vfs_lseek(int fd, size_t offset) {
	if (fd < 0 || fd >= VFS_MAX_OPEN || !vfs_files[fd].used) {
		return -1;
	}

    vfs_files[fd].offset = offset;
    return 0;
}

int vfs_close(int fd) {
	if (fd < 0 || fd >= VFS_MAX_OPEN || !vfs_files[fd].used) {
		return -1;
	}

	vfs_file_t *f = &vfs_files[fd];
	int res = f->node->fs->ops.close(f->node->fs, f->node->fs_fd);
	free_fd(fd);
	return res;
}

int vfs_unlink(const char *path) {
	vfs_node_t *node = vfs_resolve(path);
	if (!node) return -1;

	vfs_mount_t *mount = &vfs_mounts[0]; // resolve mount point properly later
    node->parent->children = NULL; // remove from parent's children list properly later
    free(node); // free node properly later
	return mount->fs->ops.unlink(mount->fs, path);
}

// Mount a filesystem
int vfs_mount(vfs_filesystem_t *fs) {
    if (vfs_mount_count >= VFS_MAX_FS) return -1;

    // Allocate root node for mount
    vfs_node_t *root = malloc(sizeof(vfs_node_t));
    if (!root) return -1;

    strncpy(root->name, "/", sizeof(root->name));
    root->type = VFS_TYPE_DIR;
    root->fs = fs;
    root->fs_data = NULL;
    root->parent = NULL;
    root->children = NULL;
    root->next = NULL;

    vfs_mounts[vfs_mount_count].fs = fs;
    vfs_mounts[vfs_mount_count].node = root;
    vfs_mount_count++;

    return 0;
}
