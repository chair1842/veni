#include "kearly.h"

static vfs_filesystem_t ramfs_fs;
static vfs_filesystem_t dvcfs_fs;

static vfs_file_ops_t kbd_ops = {
    .read = kbd_read,
    .write = kbd_write,
    .close = kbd_close
};

static vfs_file_ops_t timer_ops = {
    .read = timer_read,
    .write = timer_write,
    .close = timer_close
};

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
    irq_clear_mask(0); // PIT
    printf("PIT IRQ unmasked.\n");
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

    vfs_mount(&dvcfs_fs, "/veni/dvcf");
    printf("DVCFS mounted at /veni/dvcf.\n");

    dvcfs_regdvc("kbd", &kbd_ops, NULL);
    printf("Keyboard device registered in DVCFS.\n");

    dvcfs_regdvc("timer", &timer_ops, NULL);
    printf("Timer device registered in DVCFS.\n");

    keyboard_flush(); // Clear any buffered keys

    // Create device nodes
    vfs_node_t *dev_root = vfs_resolve("/veni/dvcf");
    if (dev_root) {
        dvcfs_create_nodes(dev_root);
    }

    io_wait();

    __asm__ volatile ("sti"); // Enable interrupts
    printf("Interrupts enabled.\n");

    printf("Kernel early initialization complete.\n");
    return;
}