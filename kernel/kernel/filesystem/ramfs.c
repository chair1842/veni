/* RAMFS filesystem implementation */

#include <kernel/vfs.h>
#include <kernel/heap.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#define RAMFS_MAX_FILES 64
#define RAMFS_NAME_MAX  32

typedef struct ramfs_file {
	char     name[RAMFS_NAME_MAX];
	uint8_t *data;
	size_t   size;
	size_t   capacity;
	int      used;
} ramfs_file_t;

static ramfs_file_t files[RAMFS_MAX_FILES];

// Find a file by name, return index or -1
static int find_file(const char *name) {
	for (int i = 0; i < RAMFS_MAX_FILES; i++) {
		if (files[i].used &&
			strncmp(files[i].name, name, RAMFS_NAME_MAX) == 0) {
			return i;
		}
	}
	return -1;
}

// Allocate a new file slot
static int alloc_file() {
	for (int i = 0; i < RAMFS_MAX_FILES; i++) {
		if (!files[i].used) {
			files[i].used = 1;
			files[i].data = NULL;
			files[i].size = 0;
			files[i].capacity = 0;
			return i;
		}
	}
	return -1;
}

// Initialize RAMFS
void ramfs_init() {
	for (int i = 0; i < RAMFS_MAX_FILES; i++) {
		files[i].used = 0;
	}
}

int ramfs_create(filesystem_t *fs, const char *path) {
	if (!path || strlen(path) > RAMFS_NAME_MAX - 1) {
		return -1;
	}

	if (find_file(path) >= 0) {
		return -1;
	}

	int idx = alloc_file();
	if (idx < 0) return -1;

	strncpy(files[idx].name, path, RAMFS_NAME_MAX - 1);
	files[idx].name[RAMFS_NAME_MAX - 1] = '\0';
	return idx;
}

int ramfs_open(filesystem_t *fs, const char *path) {
	return find_file(path);
}

size_t ramfs_read(filesystem_t *fs, int fd, void *buf, size_t size, size_t *offset) {
	if (fd < 0 || fd >= RAMFS_MAX_FILES || !files[fd].used) {
		return 0;
	}

	ramfs_file_t *f = &files[fd];

	if (*offset >= f->size) {
		return 0;
	}

	if (size > f->size - *offset) {
		size = f->size - *offset;
	}

	if (!f->data) {
		return 0;
	}

	memcpy(buf, f->data + *offset, size);
	*offset += size;
	return size;
}

size_t ramfs_write(filesystem_t *fs, int fd, const void *buf, size_t size, size_t *offset) {
	if (fd < 0 || fd >= RAMFS_MAX_FILES || !files[fd].used) {
		return 0;
	}

    ramfs_file_t *f = &files[fd];
    size_t needed = *offset + size;

    if (needed > f->capacity) {
        size_t newcap = needed;
        uint8_t *newbuf = kmalloc(newcap);
        if (!newbuf) {
            return 0;
		}

        if (f->data) {
            memcpy(newbuf, f->data, f->size);
            kfree(f->data);
        }

        f->data = newbuf;
        f->capacity = newcap;
    }

    // Fill gap with zeros if writing beyond current size
    if (*offset > f->size) {
        memset(f->data + f->size, 0, *offset - f->size);
    }

    memcpy(f->data + *offset, buf, size);
    *offset += size;
    if (*offset > f->size) {
        f->size = *offset;
    }

    return size;
}
