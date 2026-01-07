#include <stdio.h>
#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/kpanic.h>
#endif

__attribute__((__noreturn__))
void abort() {
#if defined(__is_libk)
	kpanic("abort() called");
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}
