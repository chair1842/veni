#ifndef _KERNEL_EXCEPTION_HANDLERS_H
#define _KERNEL_EXCEPTION_HANDLERS_H

#include <stdint.h>

void page_fault_handler(uint32_t error_code);

#endif