#include "pit.h"
#include <string.h>
#include <stdio.h>

static volatile uint32_t tick_count = 0;

/* IRQ0 handler for the PIT */
void pit_handler() {
    tick_count++;
}

/* Initialize the PIT to a given frequency (Hz) */
void pit_init(uint32_t frequency) {
    if (frequency == 0) return;

    uint16_t divisor = 1193182 / frequency;

    // Set PIT to mode 2 (rate generator)
    outb(PIT_COMMAND, PIT_MODE);
    outb(PIT_CHANNEL0, divisor & 0xFF);      // low byte
    outb(PIT_CHANNEL0, divisor >> 8);        // high byte
}

/* Returns the current tick count */
uint32_t get_ticks() {
    return tick_count;
}

size_t timer_read(vfs_filesystem_t *fs, void *data, void *buf, size_t size, size_t *offset) {
    (void)fs;
    (void)data;
    (void)offset;

    if (size < sizeof(uint32_t)) {
        return 0;
    }

    uint32_t last = get_ticks();
    while (get_ticks() == last) {
        io_wait();
    }

    uint32_t ticks = get_ticks();
    memcpy(buf, &ticks, sizeof(ticks));
    return sizeof(ticks);
}

size_t timer_write(vfs_filesystem_t *fs, void *data, const void *buf, size_t size, size_t *offset) {
    (void)fs; (void)data; (void)buf; (void)size; (void)offset;
    return 0; // deny write
}

int timer_close(vfs_filesystem_t *fs, void *data) {
    (void)fs; (void)data;
    return 0;
}
