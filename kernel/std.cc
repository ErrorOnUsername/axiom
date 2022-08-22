#include "std.hh"

#include <libs/ax/assert.hh>
#include <libs/ax/helpers.hh>
#include <libs/ax/types.hh>

void* memset(void* destination_ptr, int value, size_t size)
{
	size_t dest = (size_t)destination_ptr;

	if(!(dest & 0x3)) {
		size_t size_count = size / sizeof(size_t);
		size_t exploded_value = explode_byte((u8)value);

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

extern "C" int __cxa_atexit(void (*) (void*), void*, void*);

extern "C" int __cxa_atexit(void (*) (void*), void*, void*)
{
	ASSERT_NOT_REACHED();
	return 0;
}
