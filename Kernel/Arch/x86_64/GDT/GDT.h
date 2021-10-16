#pragma once

#include <AXUtil/Helpers.h>
#include <AXUtil/Types.h>
#include <Kernel/Arch/x86_64/DescriptorTablePointer.h>

namespace Kernel::GDT {

enum class GDTEntryOffset : uint16_t {
	NullSegment          = 0x0000,
	KernelCodeSegment    = 0x0008,
	KernelDataSegment    = 0x0010,
	UserspaceCodeSegment = 0x0018,
	UserspaceDataSegment = 0x0020,
};

struct GDTEntry {
	uint16_t limit_0;
	uint16_t base_0;
	uint8_t base_1;
	uint8_t access_byte;
	uint8_t limit_and_flags;
	uint8_t base_2;
} PACKED;

struct GDT {
	GDTEntry null_segment;
	GDTEntry kernel_code_segment;
	GDTEntry kernel_data_segment;
	GDTEntry userspace_code_segment;
	GDTEntry userspace_data_segment;
} PACKED
ALIGN(0x1000);

extern GDT custom_gdt;
extern "C" void load_gdt(DescriptorTablePointer* descriptor);

void init_gdt();

}
