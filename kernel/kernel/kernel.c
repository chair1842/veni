#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/kb_keys.h>
#include <kernel/vfs.h>
#include <string.h>

void kernel_main(void) {
    terminal_clear();

    printf("Testing VFS...\n");
    int fd = vfs_create("hello.txt");
    printf("Created hello.txt with fd %d\n", fd);
    vfs_write(fd, "hello world", 11);
    printf("Wrote hello world to hello.txt.\n");

    vfs_lseek(fd, 0);

    char buf[12] = {0};
    vfs_read(fd, buf, 11);
    printf("Read from hello.txt successful.\n");

    printf(buf);

    while (1) {} // never return
}
