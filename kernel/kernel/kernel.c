#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <kernel/kb_keys.h>
#include <kernel/vfs.h>
#include <string.h>

void kernel_main(void) {
    terminal_clear();

    int fd = vfs_create("hello");
    vfs_write(fd, "hello world", 11);

    char buf[12] = {0};
    vfs_read(fd, buf, 11);

    printf(buf);

    while (1) {} // never return
}
