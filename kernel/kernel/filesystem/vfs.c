#include <kernel/vfs.h>
#include "ramfs.h"

void vfs_init(void) {
    ramfs_init();
}

int vfs_create(const char *name) {
    return ramfs_create(name);
}

int vfs_open(const char *name) {
    return ramfs_open(name);
}

size_t vfs_read(int fd, void *buf, size_t size) {
    return ramfs_read(fd, buf, size);
}

size_t vfs_write(int fd, const void *buf, size_t size) {
    return ramfs_write(fd, buf, size);
}
