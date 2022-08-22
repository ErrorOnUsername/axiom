#pragma once

#include <libs/ax/types.hh>

namespace IO {

static constexpr u16 QEMU_SERIAL_PORT = 0xE9;

inline u8 in8(i16 port)
{
	u8 ret;
	asm volatile(
		"inb %1, %0"
		: "=a"(ret)
		: "Nd"(port)
	);
	return ret;
}

inline u16 in16(i16 port)
{
	u16 ret;
	asm volatile(
		"inw %1, %0"
		: "=a"(ret)
		: "Nd"(port)
	);
	return ret;
}

inline u32 in32(i16 port)
{
	u32 ret;
	asm volatile(
		"inl %1, %0"
		: "=a"(ret)
		: "Nd"(port)
	);
	return ret;
}

inline void out8(u16 port, u8 data)
{
	asm volatile("outb %0, %1" :: "a"(data),  "Nd"(port));
}

inline void out16(u16 port, u8 data)
{
	asm volatile("outw %0, %1" :: "a"(data),  "Nd"(port));
}

inline void out32(u16 port, u8 data)
{
	asm volatile("outl %0, %1" :: "a"(data),  "Nd"(port));
}

}
