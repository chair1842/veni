/* Kernel main entry point and initialization */

#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/kb_keys.h>
#include <kernel/vfs.h>
#include <string.h>

// Main kernel entry point
void kernel_main() {
    terminal_clear();

    int random = vfs_open("/veni/dvcf/random");
    if (random >= 0) {

    printf("\n\nIt is done\n");

    while (1) {} // never return
}