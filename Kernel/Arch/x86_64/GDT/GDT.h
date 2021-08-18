#pragma once

#include <AXUtil/Types.h>

namespace Kernel {

struct GDTDescriptor {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed));

struct GDTEntry {
	uint16_t limit_0;
	uint16_t base_0;
	uint8_t base_1;
	uint8_t access_byte;
	uint8_t limit_and_flags;
	uint8_t base_2;
} __attribute__((packed));

struct GDT {
	GDTEntry null_segment;
	GDTEntry kernel_code_segment;
	GDTEntry kernel_data_segment;
	GDTEntry userspace_code_segment;
	GDTEntry userspace_data_segment;
} __attribute__((packed))
__attribute__((aligned(0x1000)));

extern GDT custom_gdt;
extern "C" void load_gdt(GDTDescriptor* descriptor);

void init_custom_gdt();

}
