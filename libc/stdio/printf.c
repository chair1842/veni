#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			int num = va_arg(parameters, int);
			char buffer[32];
			char* p = &buffer[31];
			*p = '\0';
			
			// Handle negative numbers
			int is_negative = 0;
			if (num < 0) {
				is_negative = 1;
				num = -num;
			}
			
			// Convert number to string (backwards)
			do {
				*--p = '0' + (num % 10);
				num /= 10;
			} while (num > 0);
			
			// Add minus sign if negative
			if (is_negative) {
				*--p = '-';
			}
			
			size_t len = &buffer[31] - p;
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(p, len))
				return -1;
			written += len;
		} else if (*format == 'p') {
			format++;
			void* ptr = va_arg(parameters, void*);
			char buffer[10];  // 8 hex digits + "0x" + null terminator
			buffer[0] = '0';
			buffer[1] = 'x';
			for (int i = 9; i >= 2; i--) {
				unsigned int nibble = ((unsigned int)ptr >> ((9-i)*4)) & 0xF;
				buffer[i] = nibble < 10 ? '0' + nibble : 'a' + (nibble - 10);
			}
			if (!print(buffer, 10))
				return -1;
			written += 10;
		} else if (*format == 'z' && format[1] == 'u') {
			format += 2;
			size_t num = va_arg(parameters, size_t);
			char buffer[21];  // Enough for max size_t value
			char* p = &buffer[20];
			*p = '\0';
			
			do {
				*--p = '0' + (num % 10);
				num /= 10;
			} while (num > 0);
			
			size_t len = &buffer[20] - p;
			if (!print(p, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
