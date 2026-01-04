/* RAMFS filesystem implementation */

#include <kernel/vfs.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#define RAMFS_MAX_FILES 64
#define RAMFS_NAME_MAX  32

typedef struct ramfs_dirent {
    char name[RAMFS_NAME_MAX];
    int inode_idx;
} ramfs_dirent_t;

typedef struct ramfs_inode {
    int type; // 0 file, 1 dir
    uint8_t *data;
    size_t size;
    size_t capacity;
    ramfs_dirent_t entries[256]; // for dirs
    int num_entries;
    int used;
} ramfs_inode_t;

static ramfs_inode_t inodes[RAMFS_MAX_FILES];

// Allocate a new inode slot
static int alloc_inode() {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (!inodes[i].used) {
            inodes[i].used = 1;
            inodes[i].type = 0;
            inodes[i].data = NULL;
            inodes[i].size = 0;
            inodes[i].capacity = 0;
            inodes[i].num_entries = 0;
            return i;
        }
    }
    return -1;
}

// Find inode by path, return index or -1
static int ramfs_find_inode(const char *path) {
    if (!path || path[0] == '\0') return 0; // root

    int current = 0;
    char temp[256];
    strncpy(temp, path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *token = strtok(temp, "/");
    while (token) {
        if (inodes[current].type != 1) return -1; // not dir

        int found = -1;
        for (int i = 0; i < inodes[current].num_entries; i++) {
            if (strcmp(inodes[current].entries[i].name, token) == 0) {
                found = inodes[current].entries[i].inode_idx;
                break;
            }
        }
        if (found == -1) return -1;
        current = found;
        token = strtok(NULL, "/");
    }
    return current;
}

// Initialize RAMFS
void ramfs_init() {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        inodes[i].used = 0;
    }
    // Set up root inode
    inodes[0].used = 1;
    inodes[0].type = 1; // dir
    inodes[0].data = NULL;
    inodes[0].size = 0;
    inodes[0].capacity = 0;
    inodes[0].num_entries = 0;
}

int ramfs_create(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    if (!path) return -1;

    // Parse path to get dirname and basename
    char temp[256];
    strncpy(temp, path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *last_slash = strrchr(temp, '/');
    char *dirname, *basename;
    if (last_slash) {
        *last_slash = '\0';
        dirname = temp;
        basename = last_slash + 1;
    } else {
        dirname = "";
        basename = temp;
    }

    int parent_idx = ramfs_find_inode(dirname);
    if (parent_idx < 0 || inodes[parent_idx].type != 1) return -1;

    // Check if already exists
    for (int i = 0; i < inodes[parent_idx].num_entries; i++) {
        if (strcmp(inodes[parent_idx].entries[i].name, basename) == 0) return -1;
    }

    int idx = alloc_inode();
    if (idx < 0) return -1;

    inodes[idx].type = 0; // file

    // Add to parent directory
    if (inodes[parent_idx].num_entries >= 256) return -1;

    strncpy(inodes[parent_idx].entries[inodes[parent_idx].num_entries].name, basename, RAMFS_NAME_MAX - 1);
    inodes[parent_idx].entries[inodes[parent_idx].num_entries].name[RAMFS_NAME_MAX - 1] = '\0';
    inodes[parent_idx].entries[inodes[parent_idx].num_entries].inode_idx = idx;
    inodes[parent_idx].num_entries++;

    return idx;
}

int ramfs_open(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    return ramfs_find_inode(path);
}

size_t ramfs_read(vfs_filesystem_t *fs, void *fd, void *buf, size_t size, size_t *offset) {
    (void)fs;
    int idx = (int)(uintptr_t)fd;
    if (idx < 0 || idx >= RAMFS_MAX_FILES || !inodes[idx].used || inodes[idx].type != 0) {
        return 0;
    }

    ramfs_inode_t *inode = &inodes[idx];

    if (*offset >= inode->size) {
        return 0;
    }

    if (size > inode->size - *offset) {
        size = inode->size - *offset;
    }

    if (!inode->data) {
        return 0;
    }

    memcpy(buf, inode->data + *offset, size);
    *offset += size;
    return size;
}

size_t ramfs_write(vfs_filesystem_t *fs, void *fd, const void *buf, size_t size, size_t *offset) {
	(void)fs;
	int idx = (int)(uintptr_t)fd;
	if (idx < 0 || idx >= RAMFS_MAX_FILES || !inodes[idx].used || inodes[idx].type != 0) {
		return 0;
	}

    ramfs_inode_t *inode = &inodes[idx];
    size_t needed = *offset + size;

    if (needed > inode->capacity) {
        size_t newcap = needed;
        uint8_t *newbuf = malloc(newcap);
        if (!newbuf) {
            return 0;
        }

        if (inode->data) {
            memcpy(newbuf, inode->data, inode->size);
            free(inode->data);
        }

        inode->data = newbuf;
        inode->capacity = newcap;
    }

    // Fill gap with zeros if writing beyond current size
    if (*offset > inode->size) {
        memset(inode->data + inode->size, 0, *offset - inode->size);
    }

    memcpy(inode->data + *offset, buf, size);
    *offset += size;
    if (*offset > inode->size) {
        inode->size = *offset;
    }

    return size;
}

// close file
int ramfs_close(vfs_filesystem_t *fs, void *fd) {
	(void)fs;
	int idx = (int)(uintptr_t)fd;
    if (idx < 0 || idx >= RAMFS_MAX_FILES || !inodes[idx].used) {
		return -1;
	}
    return 0; // nothing special to do in RAMFS
}

// unlink (delete) a file
int ramfs_unlink(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    // Parse path
    char temp[256];
    strncpy(temp, path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *last = strrchr(temp, '/');
    char *dirname, *basename;
    if (last) {
        *last = '\0';
        dirname = temp;
        basename = last + 1;
    } else {
        dirname = "";
        basename = temp;
    }

    int parent_idx = ramfs_find_inode(dirname);
    if (parent_idx < 0 || inodes[parent_idx].type != 1) return -1;

    // Find the entry
    int entry_idx = -1;
    for (int i = 0; i < inodes[parent_idx].num_entries; i++) {
        if (strcmp(inodes[parent_idx].entries[i].name, basename) == 0) {
            entry_idx = i;
            break;
        }
    }
    if (entry_idx == -1) return -1;

    int inode_idx = inodes[parent_idx].entries[entry_idx].inode_idx;

    // Remove entry
    for (int i = entry_idx; i < inodes[parent_idx].num_entries - 1; i++) {
        inodes[parent_idx].entries[i] = inodes[parent_idx].entries[i + 1];
    }
    inodes[parent_idx].num_entries--;

    // Free inode
    if (inodes[inode_idx].data) free(inodes[inode_idx].data);
    inodes[inode_idx].used = 0;

    return 0;
}

// create directory
int ramfs_mkdir(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    if (!path) return -1;

    // Parse path
    char temp[256];
    strncpy(temp, path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *last = strrchr(temp, '/');
    char *dirname, *basename;
    if (last) {
        *last = '\0';
        dirname = temp;
        basename = last + 1;
    } else {
        dirname = "";
        basename = temp;
    }

    int parent_idx = ramfs_find_inode(dirname);
    if (parent_idx < 0 || inodes[parent_idx].type != 1) return -1;

    // Check if exists
    for (int i = 0; i < inodes[parent_idx].num_entries; i++) {
        if (strcmp(inodes[parent_idx].entries[i].name, basename) == 0) return 0; // exists
    }

    int idx = alloc_inode();
    if (idx < 0) return -1;

    inodes[idx].type = 1; // dir

    // Add to parent
    if (inodes[parent_idx].num_entries >= 256) return -1;

    strncpy(inodes[parent_idx].entries[inodes[parent_idx].num_entries].name, basename, RAMFS_NAME_MAX - 1);
    inodes[parent_idx].entries[inodes[parent_idx].num_entries].name[RAMFS_NAME_MAX - 1] = '\0';
    inodes[parent_idx].entries[inodes[parent_idx].num_entries].inode_idx = idx;
    inodes[parent_idx].num_entries++;

    return 0;
}

// remove directory
int ramfs_rmdir(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    // Similar to unlink, but check if dir is empty
    int idx = ramfs_find_inode(path);
    if (idx < 0 || inodes[idx].type != 1) return -1;
    if (inodes[idx].num_entries > 0) return -1; // not empty

    // Parse to find parent
    char temp[256];
    strncpy(temp, path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *last = strrchr(temp, '/');
    char *dirname, *basename;
    if (last) {
        *last = '\0';
        dirname = temp;
        basename = last + 1;
    } else {
        dirname = "";
        basename = temp;
    }

    int parent_idx = ramfs_find_inode(dirname);
    if (parent_idx < 0) return -1;

    // Remove entry
    int entry_idx = -1;
    for (int i = 0; i < inodes[parent_idx].num_entries; i++) {
        if (strcmp(inodes[parent_idx].entries[i].name, basename) == 0) {
            entry_idx = i;
            break;
        }
    }
    if (entry_idx == -1) return -1;

    for (int i = entry_idx; i < inodes[parent_idx].num_entries - 1; i++) {
        inodes[parent_idx].entries[i] = inodes[parent_idx].entries[i + 1];
    }
    inodes[parent_idx].num_entries--;

    // Free inode
    inodes[idx].used = 0;

    return 0;
}

// get file metadata
int ramfs_stat(vfs_filesystem_t *fs, const char *path, vfs_stat_t *st) {
    (void)fs;
    int idx = ramfs_find_inode(path);
    if (idx < 0) return -1;

    st->type = inodes[idx].type ? VFS_TYPE_DIR : VFS_TYPE_FILE;
    st->size = inodes[idx].size;
    return 0;
}
