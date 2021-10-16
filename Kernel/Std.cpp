#include "Std.h"

#include <AXUtil/Helpers.h>
#include <AXUtil/Types.h>

void* memset(void* destination_ptr, int value, size_t size)
{
	size_t dest = (size_t)destination_ptr;

	if(!(dest & 0x3)) {
		size_t size_count = size / sizeof(size_t);
		size_t exploded_value = exlode_byte((uint8_t)value);

		asm volatile(
			"rep stosq\n"
			: "=D"(dest)
			: "D"(dest), "c"(size_count), "a"(exploded_value)
			: "memory");

		size -= size_count * sizeof(size_t);
		if(size == 0)
			return destination_ptr;
	}

	asm volatile(
		"rep stosb"
		: "=D"(dest), "=c"(size)
		: "0"(dest), "1"(size), "a"(value)
		: "memory");

	return destination_ptr;
}
