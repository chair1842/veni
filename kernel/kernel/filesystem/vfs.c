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

// Helper: add child node to parent
static void vfs_add_child(vfs_node_t *parent, vfs_node_t *child) {
	if (!parent->children) {
		parent->children = child;
	} else {
		vfs_node_t *last = parent->children;
		while (last->next) last = last->next;
		last->next = child;
	}
	child->parent = parent;
}


// Helper: find the mount with the longest path prefix
static vfs_mount_t *find_mount(const char *path) {
	vfs_mount_t *best_mount = NULL;
	size_t best_len = 0;

	for (int i = 0; i < vfs_mount_count; i++) {
		vfs_mount_t *mount = &vfs_mounts[i];
		size_t len = strlen(mount->mount_path);
		if (len > best_len && strncmp(path, mount->mount_path, len) == 0) {
			// Special case for root mount
			if (strcmp(mount->mount_path, "/") == 0 && path[0] == '/') {
				best_mount = mount;
				best_len = len;
			} else if (path[len] == '/' || path[len] == '\0') {
				best_mount = mount;
				best_len = len;
			}
		}
	}
	return best_mount;
}

// Remove a child from parent
static void vfs_remove_child(vfs_node_t *parent, vfs_node_t *child) {
	if (!parent || !child) return;
	if (parent->children == child) {
		parent->children = child->next;
	} else {
		vfs_node_t *prev = parent->children;
		while (prev && prev->next != child) prev = prev->next;
		if (prev) prev->next = child->next;
	}
	child->next = NULL;
	child->parent = NULL;
}

// Recursive free of a node
static void vfs_free_node(vfs_node_t *node) {
	if (!node) return;
	vfs_node_t *child = node->children;
	while (child) {
		vfs_node_t *next = child->next;
		vfs_free_node(child);
		child = next;
	}
	free(node);
}

// Resolve a path like "/dir/sub/file"
vfs_node_t *vfs_resolve(const char *path) {
	if (!path || path[0] != '/' || vfs_mount_count == 0) return NULL;

	// Find the best mount
	vfs_mount_t *mount = find_mount(path);
	if (!mount) return NULL;

	vfs_node_t *node = mount->node;
	if (!node) return NULL;

	// Get the remaining path after the mount point
	const char *remaining = path + strlen(mount->mount_path);
	if (*remaining == '/') remaining++; // skip leading /

	if (*remaining == '\0') return node; // exactly the mount point

	// Duplicate remaining path for strtok
	char temp[256];
	strncpy(temp, remaining, sizeof(temp));
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

	// Find the mount
	vfs_mount_t *mount = find_mount(path);
	if (!mount) return -1;

	// Get relative path
	const char *rel_path = path + strlen(mount->mount_path);
	if (*rel_path == '/') rel_path++;

	// Split path into components
	char temp[256];
	strncpy(temp, rel_path, sizeof(temp)-1);
	temp[sizeof(temp)-1] = '\0';

	vfs_node_t *current = mount->node; // start at root

	char current_path[256] = "";
	char *token = strtok(temp, "/");
	char *next_token = NULL;

	// Keep last token for the file itself
	while (token) {
		next_token = strtok(NULL, "/");
		if (!next_token) break; // last component = file
		// Intermediate directories
		vfs_node_t *child = find_child(current, token);

		if (!child) {
			// Build path for mkdir
			char mkdir_path[256];
			size_t pos = 0;
			if (strlen(current_path) > 0) {
				strcpy(mkdir_path, current_path);
				pos = strlen(mkdir_path);
				if (pos + 1 < sizeof(mkdir_path)) {
					mkdir_path[pos] = '/';
					mkdir_path[pos + 1] = '\0';
					pos++;
				}
			}
			size_t tlen = strlen(token);
			if (pos + tlen < sizeof(mkdir_path)) {
				memcpy(mkdir_path + pos, token, tlen + 1);
			}
			// Call fs mkdir
			mount->fs->ops.mkdir(mount->fs, mkdir_path);

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
				vfs_add_child(current, child);
				last->next = child;
			}
		}

		current = child;
		// Update current_path
		size_t len = strlen(current_path);
		if (len > 0 && len + 1 < sizeof(current_path)) {
			current_path[len] = '/';
			current_path[len + 1] = '\0';
		}
		len = strlen(current_path);
		size_t tok_len = strlen(token);
		if (len + tok_len < sizeof(current_path)) {
			memcpy(current_path + len, token, tok_len + 1);
		}
		token = next_token;
	}

	// Now token = last component = file
	if (find_child(current, token)) return -1; // file already exists

	int fs_fd_int = mount->fs->ops.create(mount->fs, rel_path);
	if (fs_fd_int < 0) return -1;
	void *fs_fd = (void*)(uintptr_t)fs_fd_int;

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
		vfs_add_child(current, file_node);
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
	if (!node || !node->parent) return -1;

	vfs_mount_t *mount = find_mount(path);
	if (!mount) return -1;

	const char *rel_path = path + strlen(mount->mount_path);
	if (*rel_path == '/') rel_path++;

	int res = mount->fs->ops.unlink(mount->fs, rel_path);
	if (res == 0) vfs_remove_child(node->parent, node);
	vfs_free_node(node);

	return res;
}

int vfs_mkdir(const char *path) {
	if (!path || path[0] != '/' || vfs_mount_count == 0) {
		return -1;
	}

	vfs_mount_t *mount = find_mount(path);
	if (!mount) return -1;

	const char *rel_path = path + strlen(mount->mount_path);
	if (*rel_path == '/') rel_path++;

	return mount->fs->ops.mkdir(mount->fs, rel_path);
}

int vfs_rmdir(const char *path) {
	if (!path || path[0] != '/' || vfs_mount_count == 0) {
		return -1;
	}

	vfs_mount_t *mount = find_mount(path);
	if (!mount) return -1;

	const char *rel_path = path + strlen(mount->mount_path);
	if (*rel_path == '/') rel_path++;

	return mount->fs->ops.rmdir(mount->fs, rel_path);
}

int vfs_stat(const char *path, vfs_stat_t *st) {
	if (!path || path[0] != '/' || vfs_mount_count == 0) {
		return -1;
	}

	vfs_mount_t *mount = find_mount(path);
	if (!mount) return -1;

	const char *rel_path = path + strlen(mount->mount_path);
	if (*rel_path == '/') rel_path++;

	return mount->fs->ops.stat(mount->fs, rel_path, st);
}

int vfs_unmount(const char *path) {
	vfs_mount_t *mount = find_mount(path);
	if (!mount || strcmp(mount->mount_path, path) != 0) return -1;

	int idx = -1;
	for (int i = 0; i < vfs_mount_count; i++) {
		if (&vfs_mounts[i] == mount) { idx = i; break; }
	}
	if (idx == -1) return -1;

	vfs_free_node(mount->node); // recursive free

	for (int i = idx; i < vfs_mount_count - 1; i++) {
		vfs_mounts[i] = vfs_mounts[i + 1];
	}
	vfs_mount_count--;
	return 0;
}

// Mount a filesystem
int vfs_mount(vfs_filesystem_t *fs, const char *path) {
	if (vfs_mount_count >= VFS_MAX_FS) return -1;

	// Allocate root node
	vfs_node_t *root = malloc(sizeof(vfs_node_t));
	if (!root) return -1;

	// Extract last component of path
	const char *name = path;
	const char *slash = strrchr(path, '/');
	if (slash) name = slash + 1;
	if (*name == '\0') name = "/"; // root mount

	strncpy(root->name, name, sizeof(root->name) - 1);
	root->name[sizeof(root->name) - 1] = '\0';
	root->type = VFS_TYPE_DIR;
	root->fs = fs;
	root->fs_data = NULL;
	root->parent = NULL;
	root->children = NULL;
	root->next = NULL;

	strncpy(vfs_mounts[vfs_mount_count].mount_path, path, sizeof(vfs_mounts[vfs_mount_count].mount_path) - 1);
	vfs_mounts[vfs_mount_count].mount_path[sizeof(vfs_mounts[vfs_mount_count].mount_path) - 1] = '\0';
	vfs_mounts[vfs_mount_count].fs = fs;
	vfs_mounts[vfs_mount_count].node = root;
	vfs_mount_count++;

	return 0;
}

