/* Kernel main entry point and initialization */

#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/kb_keys.h>
#include <kernel/vfs.h>
#include <string.h>

// Main kernel entry point
void kernel_main() {
    terminal_clear();

    printf("Testing VFS...\n");
    int fd = vfs_create("/hello.txt");
    printf("Created hello.txt with fd %d\n", fd);
    vfs_write(fd, "hello world", 11);
    printf("Wrote hello world to hello.txt.\n\n");

    vfs_lseek(fd, 0);

    char buf[12] = {0};
    vfs_read(fd, buf, 11);
    printf("Read from hello.txt successful.\n");

    printf(buf);

    printf("\n\nNext test\n");
    int fd2 = vfs_create("/num/numbers.txt");
    printf("Created num/numbers.txt with fd %d\n", fd2);
    vfs_write(fd2, "1234567890", 10);
    printf("Wrote numbers to numbers.txt.\n\n");

    vfs_lseek(fd2, 0);

    char numbuf[11] = {0};
    vfs_read(fd2, numbuf, 10);
    printf("Read from numbers.txt successful.\n");

    printf(numbuf);

    vfs_lseek(fd, 0);

    printf("\n\nRe-reading hello.txt:\n");
    char buf2[12] = {0};
    vfs_read(fd, buf2, 11);
    printf(buf2);

    vfs_unlink("/num/hello.txt");
    vfs_unlink("/numbers.txt");
    printf("\n\nDeleted hello.txt and numbers.txt\n");

    printf("\n\nIt is done\n");

    while (1) {} // never return
}