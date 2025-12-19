#include "pit.h"

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
uint32_t get_ticks(void) {
    return tick_count;
}
