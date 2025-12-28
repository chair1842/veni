#include <stdint.h>
#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/heap.h>
#include <kernel/serial.h>
#include "interrupts/idt.h"
#include "interrupts/pic.h"
#include "drivers/pit/pit.h"
#include <kernel/vfs.h>

extern uint32_t kernel_end;

void kearly();