#pragma once

#include <AXUtil/Types.h>

#define IDT_INTERRUPT_GATE_TA 0x8e
#define IDT_TRAP_GATE_TA      0x8f
#define IDT_CALL_GATE_TA      0x8b

struct IDTR {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed));

struct IDTDescriptor {
	uint16_t offset_0;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_and_attributes;
	uint16_t offset_1;
	uint32_t offset_2;
	uint32_t zero;

	uint64_t offset()
	{
		uint64_t ret = offset_0;
		ret |= (uint64_t)offset_1 << 16;
		ret |= (uint64_t)offset_2 << 32;
		return ret;
	}

	void set_offset(uint64_t offset)
	{
		offset_0 = (uint16_t)((offset & 0x000000000000ffff) >> 0);
		offset_1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
		offset_2 = (uint16_t)((offset & 0xffffffff00000000) >> 32);
	}
};
