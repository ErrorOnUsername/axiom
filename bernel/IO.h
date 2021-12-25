#pragma once

#include <AXUtil/Types.h>

namespace IO {

static constexpr uint16_t QEMU_SERIAL_PORT = 0xE9;

inline uint8_t in8(int16_t port)
{
	uint8_t ret;
	asm volatile(
		"inb %1, %0"
		: "=a"(ret)
		: "Nd"(port)
	);
	return ret;
}

inline uint16_t in16(int16_t port)
{
	uint16_t ret;
	asm volatile(
		"inw %1, %0"
		: "=a"(ret)
		: "Nd"(port)
	);
	return ret;
}

inline uint32_t in32(int16_t port)
{
	uint32_t ret;
	asm volatile(
		"inl %1, %0"
		: "=a"(ret)
		: "Nd"(port)
	);
	return ret;
}

inline void out8(uint16_t port, uint8_t data)
{
	asm volatile("outb %0, %1" :: "a"(data),  "Nd"(port));
}

inline void out16(uint16_t port, uint8_t data)
{
	asm volatile("outw %0, %1" :: "a"(data),  "Nd"(port));
}

inline void out32(uint16_t port, uint8_t data)
{
	asm volatile("outl %0, %1" :: "a"(data),  "Nd"(port));
}

}
