/* Kernel main entry point and initialization */

#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/kb_keys.h>
#include <kernel/vfs.h>
#include <string.h>

// Main kernel entry point
void kernel_main() {
    terminal_clear();

    int kbd = vfs_open("/veni/dvcf/kbd");
    if (kbd >= 0) {
        printf("Keyboard device opened with fd %d\n", kbd);

        KeyPacket_t pkt = {0};
        printf("Waiting for a key press...\n");

        vfs_read(kbd, &pkt, 0);
        printf("KeyPacket received: code=%d, pressed=%d, shift=%d, ctrl=%d, alt=%d, capslock=%d, scrolllock=%d, numlock=%d\n",
               pkt.keycode, pkt.pressed, pkt.shift, pkt.ctrl, pkt.alt,
               pkt.capslock, pkt.scrolllock, pkt.numlock);

        char ch = key_to_ascii(pkt);
        if (ch != 0) {
            printf("Key pressed: %c (code %d)\n", ch, pkt.keycode);
        } else {
            printf("Non-printable key pressed (code %d)\n", pkt.keycode);
        }
    } else {
        printf("Failed to open keyboard device\n");
    }

    printf("\n\nIt is done\n");

    while (1) {} // never return
}