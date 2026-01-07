#include <kernel/tty.h>
#include "../drivers/pit/pit.h"
#include "../drivers/keyboard/keyboard.h"
#include "pic.h"
#include <stdio.h>
#include <kernel/kpanic.h>
#include "../excphandl/excphandl.h"

__attribute__((noreturn))
void exception_handler(uint32_t int_no, uint32_t err);
void exception_handler(uint32_t int_no, uint32_t err) {
	switch (int_no) {
		case 0: 
			kpanic("Division By Zero Exception"); 
			break;
		case 6: 
			kpanic("Invalid Opcode Exception"); 
			break;
		case 13: 
			kpanic("General Protection Fault");
			break;
		case 14: 
			page_fault_handler(err);
			break;
		default:
			kpanic("Unknown Exception");
			break;
	}
}

void irq_handler(uint32_t vector) {
	uint8_t irq = vector - 32;

		// Minimal handling only
	switch (irq) {
		case 0:
			pit_handler();
			break;
		case 1:
			keyboard_handler(); // FUCK FUCK FUCK
			break;
		default:
			printf("Unhandled IRQ: %d\n", irq);
			break;
	}

	pic_send_eoi(irq);
}
