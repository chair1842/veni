/* DVCFS Implementation */
#include "dvcfs.h"
#include <kernel/vfs.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DVCFS_MAX_DEVICES 32

typedef struct dvcfs_device {
    char name[32];
    vfs_file_ops_t *ops;
    void *data;
    int used;
} dvcfs_device_t;

static dvcfs_device_t devices[DVCFS_MAX_DEVICES];

// Standard device operations

// /dev/null operations
size_t null_read(vfs_filesystem_t *fs, void *data, void *buf, size_t size, size_t *offset) {
    (void)fs; (void)data; (void)buf; (void)size; (void)offset;
    return 0; // EOF
}

size_t null_write(vfs_filesystem_t *fs, void *data, const void *buf, size_t size, size_t *offset) {
    (void)fs; (void)data; (void)buf; (void)offset;
    return size; // pretend to write all
}

int null_close(vfs_filesystem_t *fs, void *data) {
    (void)fs; (void)data;
    return 0;
}

static vfs_file_ops_t null_ops = {
    .read = null_read,
    .write = null_write,
    .close = null_close
};

// /dev/zero operations
size_t zero_read(vfs_filesystem_t *fs, void *data, void *buf, size_t size, size_t *offset) {
    (void)fs; (void)data; (void)offset;
    memset(buf, 0, size);
    return size;
}

size_t zero_write(vfs_filesystem_t *fs, void *data, const void *buf, size_t size, size_t *offset) {
    (void)fs; (void)data; (void)buf; (void)offset;
    return size; // pretend to write all
}

int zero_close(vfs_filesystem_t *fs, void *data) {
    (void)fs; (void)data;
    return 0;
}

static vfs_file_ops_t zero_ops = {
    .read = zero_read,
    .write = zero_write,
    .close = zero_close
};

// Initialize DVCFS
void dvcfs_init() {
    for (int i = 0; i < DVCFS_MAX_DEVICES; i++) {
        devices[i].used = 0;
    }

    // Register standard devices
    dvcfs_regdvc("null", &null_ops, NULL);
    dvcfs_regdvc("zero", &zero_ops, NULL);
}

// Register a device
int dvcfs_regdvc(const char *dvc_name, vfs_file_ops_t *ops, void *data) {
    if (!dvc_name || !ops) return -1;

    for (int i = 0; i < DVCFS_MAX_DEVICES; i++) {
        if (!devices[i].used) {
            strncpy(devices[i].name, dvc_name, sizeof(devices[i].name) - 1);
            devices[i].name[sizeof(devices[i].name) - 1] = '\0';
            devices[i].ops = ops;
            devices[i].data = data;
            devices[i].used = 1;
            return 0;
        }
    }
    return -1; // no space
}

// Find device by name
static dvcfs_device_t *find_device(const char *name) {
    for (int i = 0; i < DVCFS_MAX_DEVICES; i++) {
        if (devices[i].used && strcmp(devices[i].name, name) == 0) {
            return &devices[i];
        }
    }
    return NULL;
}

// DVCFS operations

int dvcfs_create(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    (void)path;
    // Device files are not created by users
    return -1;
}

int dvcfs_open(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    if (!path) return -1;

    // Remove leading /
    if (path[0] == '/') path++;

    dvcfs_device_t *dev = find_device(path);
    if (!dev) return -1;

    // For devices, return the device index as fd
    return (int)(dev - devices);
}

size_t dvcfs_read(vfs_filesystem_t *fs, void *fd, void *buf, size_t size, size_t *offset) {
    (void)fs;
    int idx = (int)(uintptr_t)fd;
    if (idx < 0 || idx >= DVCFS_MAX_DEVICES || !devices[idx].used) return 0;

    dvcfs_device_t *dev = &devices[idx];
    if (!dev->ops->read) return 0;

    // Delegate to device read, but pass device's data
    return dev->ops->read(fs, dev->data, buf, size, offset);
}

size_t dvcfs_write(vfs_filesystem_t *fs, void *fd, const void *buf, size_t size, size_t *offset) {
    (void)fs;
    int idx = (int)(uintptr_t)fd;
    if (idx < 0 || idx >= DVCFS_MAX_DEVICES || !devices[idx].used) return 0;

    dvcfs_device_t *dev = &devices[idx];
    if (!dev->ops->write) return 0;

    return dev->ops->write(fs, dev->data, buf, size, offset);
}

int dvcfs_close(vfs_filesystem_t *fs, void *fd) {
    (void)fs;
    int idx = (int)(uintptr_t)fd;
    if (idx < 0 || idx >= DVCFS_MAX_DEVICES || !devices[idx].used) return -1;

    dvcfs_device_t *dev = &devices[idx];
    if (!dev->ops->close) return 0;

    return dev->ops->close(fs, dev->data);
}

int dvcfs_unlink(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    (void)path;
    // Device files are not unlinked
    return -1;
}

int dvcfs_mkdir(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    (void)path;
    // No directories in DVCFS
    return -1;
}

int dvcfs_rmdir(vfs_filesystem_t *fs, const char *path) {
    (void)fs;
    (void)path;
    // No directories
    return -1;
}

int dvcfs_stat(vfs_filesystem_t *fs, const char *path, vfs_stat_t *st) {
    (void)fs;
    if (!path || !st) return -1;

    if (path[0] == '/') path++;

    dvcfs_device_t *dev = find_device(path);
    if (!dev) return -1;

    st->type = VFS_TYPE_FILE; // devices are files
    st->size = 0; // devices don't have size
    return 0;
}