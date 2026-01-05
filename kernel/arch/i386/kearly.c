#include "kearly.h"
#include <kernel/vfs.h>
#include "../kernel/filesystem/ramfs.h"
#include "../kernel/filesystem/dvcfs.h"
#include <stdlib.h>
#include <string.h>

static vfs_filesystem_t ramfs_fs;
static vfs_filesystem_t dvcfs_fs;

void kearly() {
    terminal_initialize();
    printf("Kernel early initialization started.\n");
    serial_init();
    printf("Serial port initialized.\n");

    // Initialize PIC
    pic_remap(0x20, 0x28);
    printf("PIC remapped.\n");
    pic_disable();
    printf("PIC disabled.\n");

    // Initialize IDT
    idt_init();
    printf("IDT initialized.\n");

    pit_init(100); // Set PIT to 100Hz
    printf("PIT initialized to 100Hz.\n");

    // Unmask IRQs
    //irq_clear_mask(0); // PIT
    //printf("PIT IRQ unmasked.\n");
    irq_clear_mask(1); // Keyboard
    printf("Keyboard IRQ unmasked.\n");
    printf("IRQs unmasked.\n");
    
    // Initialize physical memory manager
    // Start after kernel end, use first 16MB for now
    pmm_init((uint32_t)&kernel_end, 16 * 1024 * 1024);
    printf("Physical Memory Manager initialized.\n");
    
    // Initialize virtual memory manager
    vmm_init();
    printf("Virtual Memory Manager initialized.\n");
    
    // Initialize kernel heap
    kheap_init();
    printf("Heap initialized.\n");

    vfs_init();
    printf("Virtual Filesystem initialized.\n");

    ramfs_init();

    ramfs_fs = (vfs_filesystem_t){
        .name = "ramfs",
        .ops = {
            .create = ramfs_create,
            .open = ramfs_open,
            .read = ramfs_read,
            .write = ramfs_write,
            .close = ramfs_close,
            .unlink = ramfs_unlink,
            .mkdir = ramfs_mkdir,
            .rmdir = ramfs_rmdir,
            .stat = ramfs_stat
        },
        .data = NULL
    };

    vfs_mount(&ramfs_fs, "/");
    printf("RAMFS mounted as root filesystem.\n");

    dvcfs_init();

    dvcfs_fs = (vfs_filesystem_t){
        .name = "dvcfs",
        .ops = {
            .create = dvcfs_create,
            .open = dvcfs_open,
            .read = dvcfs_read,
            .write = dvcfs_write,
            .close = dvcfs_close,
            .unlink = dvcfs_unlink,
            .mkdir = dvcfs_mkdir,
            .rmdir = dvcfs_rmdir,
            .stat = dvcfs_stat
        },
        .data = NULL
    };

    vfs_mount(&dvcfs_fs, "/dev");
    printf("DVCFS mounted at /dev.\n");

    // Create device nodes
    vfs_node_t *dev_root = vfs_resolve("/dev");
    if (dev_root) {
        // null device
        vfs_node_t *null_node = malloc(sizeof(vfs_node_t));
        if (null_node) {
            strncpy(null_node->name, "null", sizeof(null_node->name) - 1);
            null_node->name[sizeof(null_node->name) - 1] = '\0';
            null_node->type = VFS_TYPE_FILE;
            null_node->fs = &dvcfs_fs;
            null_node->fs_data = NULL;
            null_node->fs_fd = (void*)0; // device index
            null_node->parent = dev_root;
            null_node->children = NULL;
            null_node->next = NULL;
            dev_root->children = null_node;
        }

        // zero device
        vfs_node_t *zero_node = malloc(sizeof(vfs_node_t));
        if (zero_node) {
            strncpy(zero_node->name, "zero", sizeof(zero_node->name) - 1);
            zero_node->name[sizeof(zero_node->name) - 1] = '\0';
            zero_node->type = VFS_TYPE_FILE;
            zero_node->fs = &dvcfs_fs;
            zero_node->fs_data = NULL;
            zero_node->fs_fd = (void*)1; // device index
            zero_node->parent = dev_root;
            zero_node->children = NULL;
            zero_node->next = NULL;
            if (null_node) null_node->next = zero_node;
        }
    }

    io_wait();

    __asm__ volatile ("sti"); // Enable interrupts
    printf("Interrupts enabled.\n");

    printf("Kernel early initialization complete.\n");
    return;
}