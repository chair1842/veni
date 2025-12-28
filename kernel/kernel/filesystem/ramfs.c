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

static int find_file(const char *name) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (files[i].used &&
            strncmp(files[i].name, name, RAMFS_NAME_MAX) == 0) {
            return i;
        }
    }
    return -1;
}

static int alloc_file(void) {
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

void ramfs_init(void) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        files[i].used = 0;
    }
}

int ramfs_create(const char *name) {
    if (!name || strlen(name) >= RAMFS_NAME_MAX)
        return -1;

    if (find_file(name) >= 0)
        return -1;

    int idx = alloc_file();
    if (idx < 0)
        return -1;

    strcpy(files[idx].name, name);
    return idx;
}

int ramfs_open(const char *name) {
    return find_file(name);
}

size_t ramfs_read(int fd, void *buf, size_t size) {
    if (fd < 0 || fd >= RAMFS_MAX_FILES || !files[fd].used)
        return 0;

    ramfs_file_t *f = &files[fd];

    if (size > f->size)
        size = f->size;

    memcpy(buf, f->data, size);
    return size;
}

size_t ramfs_write(int fd, const void *buf, size_t size) {
    if (fd < 0 || fd >= RAMFS_MAX_FILES || !files[fd].used)
        return 0;

    ramfs_file_t *f = &files[fd];

    if (size > f->capacity) {
        uint8_t *newbuf = kmalloc(size);
        if (!newbuf)
            return 0;

        if (f->data) {
            memcpy(newbuf, f->data, f->size);
            kfree(f->data);
        }

        f->data = newbuf;
        f->capacity = size;
    }

    memcpy(f->data, buf, size);
    f->size = size;
    return size;
}
