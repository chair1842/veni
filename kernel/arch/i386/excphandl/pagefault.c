#include "excphandl.h"
#include <kernel/io.h>
#include <stdbool.h>
#include <kernel/kpanic.h>

// handle page fault exception
void page_fault_handler(uint32_t error_code) {
	uintptr_t fault_addr;
	asm volatile ("mov %%cr2, %0" : "=r"(fault_addr));

	bool present = error_code & 0x1;
	bool write   = error_code & 0x2;
	bool user    = error_code & 0x4;
	bool fetch   = error_code & 0x10;

	kpanic("Page Fault Exception");
}