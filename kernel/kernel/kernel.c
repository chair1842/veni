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

    // Test DVCFS
    printf("\n\nTesting DVCFS...\n");
    int fd_null = vfs_open("/dev/null");
    printf("Opened /dev/null with fd %d\n", fd_null);
    if (fd_null >= 0) {
        char nullbuf[10] = {0};
        size_t read_null = vfs_read(fd_null, nullbuf, 10);
        printf("Read %d bytes from /dev/null\n", read_null);
        size_t write_null = vfs_write(fd_null, "test", 4);
        printf("Wrote %d bytes to /dev/null\n", write_null);
        vfs_close(fd_null);
    }

    int fd_zero = vfs_open("/dev/zero");
    printf("Opened /dev/zero with fd %d\n", fd_zero);
    if (fd_zero >= 0) {
        char zerobuf[10] = {0};
        size_t read_zero = vfs_read(fd_zero, zerobuf, 10);
        printf("Read %d bytes from /dev/zero: ", read_zero);
        for (int i = 0; i < 10; i++) {
            printf("%02x ", (unsigned char)zerobuf[i]);
        }
        printf("\n");
        vfs_close(fd_zero);
    }

    printf("\n\nIt is done\n");

    while (1) {} // never return
}