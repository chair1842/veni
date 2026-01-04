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
    int fd = vfs_create("/home/hello.txt");
    printf("Created /home/hello.txt with fd %d\n", fd);
    vfs_write(fd, "hello world", 11);
    printf("Wrote hello world to /home/hello.txt.\n\n");

    vfs_lseek(fd, 0);

    char buf[12] = {0};
    vfs_read(fd, buf, 11);
    printf("Read from /home/hello.txt successful.\n");

    printf(buf);

    printf("\n\nNext test\n");
    int fd2 = vfs_create("/home/num/numbers.txt");
    printf("Created /home/num/numbers.txt with fd %d\n", fd2);
    vfs_write(fd2, "1234567890", 10);
    printf("Wrote numbers to /home/num/numbers.txt.\n\n");

    vfs_lseek(fd2, 0);

    char numbuf[11] = {0};
    vfs_read(fd2, numbuf, 10);
    printf("Read from /home/num/numbers.txt successful.\n");

    printf(numbuf);

    vfs_lseek(fd, 0);

    printf("\n\nRe-reading /home/hello.txt:\n");
    char buf2[12] = {0};
    vfs_read(fd, buf2, 11);
    printf(buf2);

    vfs_mkdir("/home/docs");
    printf("\n\nCreated directory /home/docs\n");
    vfs_rmdir("/home/docs");
    printf("Removed directory /home/docs\n");

    vfs_stat("/home/hello.txt", NULL);
    vfs_stat("/home/num/numbers.txt", NULL);
    printf("\n\nStat called on both files.\n");

    vfs_unlink("/home/hello.txt");
    vfs_unlink("/home/num/numbers.txt");
    printf("\n\nDeleted /home/hello.txt and /home/num/numbers.txt\n");

    printf("\n\nIt is done\n");

    while (1) {} // never return
}