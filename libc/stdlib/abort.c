#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort() {
#if defined(__is_libk)
	printf("Kernel panic.\n");
	asm volatile("cli");
	while (1) {
		asm volatile("hlt");
	}
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}
