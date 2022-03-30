#pragma once

#include <ax_util/helpers.hh>
#include <ax_util/types.hh>
#include <kernel/arch/amd64/descriptor_table_pointer.hh>

namespace Kernel::GDT {

enum class GDTEntryOffset : u16 {
	NullSegment          = 0x0000,
	KernelCodeSegment    = 0x0008,
	KernelDataSegment    = 0x0010,
	UserspaceCodeSegment = 0x0018,
	UserspaceDataSegment = 0x0020,
};

struct GDTEntry {
	u16 limit_0;
	u16 base_0;
	u8  base_1;
	u8  access_byte;
	u8  limit_and_flags;
	u8  base_2;
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
