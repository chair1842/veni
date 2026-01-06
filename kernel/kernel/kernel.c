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
        printf("Random device opened with fd %d\n", random);

        uint8_t buffer[16];

        vfs_read(random, buffer, sizeof(buffer));

        for (int i = 0; i < sizeof(buffer); i++) {
            printf("%d ", buffer[i]);
        }

        printf("\n\n");
    } else {
        printf("Failed to open random device\n");
    }

    int kbd = vfs_open("/veni/dvcf/kbd");
    if (kbd >= 0) {
        printf("Keyboard device opened with fd %d\n", kbd);
        printf("Waiting for keyboard input... Press 'q' to quit.\n");

        while (1) {
            KeyPacket_t pkt = {0};
            vfs_read(kbd, &pkt, sizeof(KeyPacket_t));
            printf("KeyPacket received: code=%d, pressed=%d, shift=%d, ctrl=%d, alt=%d, capslock=%d, scrolllock=%d, numlock=%d\n",
                   pkt.keycode, pkt.pressed, pkt.shift, pkt.ctrl, pkt.alt,
                   pkt.capslock, pkt.scrolllock, pkt.numlock);

            char ch = key_to_ascii(pkt);
            if (ch != 0) {
                printf("Key pressed: %c (code %d)\n", ch, pkt.keycode);
            } else {
                printf("Non-printable key pressed (code %d)\n", pkt.keycode);
            }

            if (pkt.keycode == KEY_Q && pkt.pressed) {
                break; // Quit on 'q' press
            }
        }

        vfs_close(kbd);
    } else {
        printf("Failed to open keyboard device\n");
    }

    int timer = vfs_open("/veni/dvcf/timer");
    if (timer >= 0) {
        printf("Timer device opened with fd %d\n", timer);

        uint32_t ticks = 0;

        while (1) {
            vfs_read(timer, &ticks, sizeof(ticks));
            printf("Timer ticks: %d\n", ticks);
            if (ticks >= 1000) { // Exit after 10 seconds
                break;
            }
        }
    } else {
        printf("Failed to open timer device\n");
    }

    printf("\n\nIt is done\n");

    while (1) {} // never return
}