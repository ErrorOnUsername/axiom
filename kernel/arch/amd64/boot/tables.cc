#include "tables.hh"

#include <libs/ax/types.hh>
#include <libs/ax/helpers.hh>

#include <kernel/k_debug.hh>

namespace Kernel {

struct DescriptorTablePointer {
	u16 size;
	u64 offset;
} PACKED;

// ####################### //
// Global Descriptor Table //
// ####################### //

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

extern "C" void load_gdt(DescriptorTablePointer* descriptor);

// ########################## //
// Interrupt Descriptor Table //
// ########################## //

#define IDT_ENTRY_COUNT 256

#define IDT_INTERRUPT_GATE_TA 0x8e
#define IDT_TRAP_GATE_TA      0x8f

struct IDTEntryDescriptor {
	u16 offset_0;
	u16 selector;
	u8  ist;
	u8  type_and_attributes;
	u16 offset_1;
	u32 offset_2;
	u32 reserved;
} PACKED;

static DescriptorTablePointer idtr;
static IDTEntryDescriptor idt[IDT_ENTRY_COUNT];

static void set_offset_of_idt_entry(IDTEntryDescriptor* descriptor, u64 offset)
{
	descriptor->offset_0 = (u16)((offset & 0x000000000000ffff) >> 0);
	descriptor->offset_1 = (u16)((offset & 0x00000000ffff0000) >> 16);
	descriptor->offset_2 = (u32)((offset & 0xffffffff00000000) >> 32);
}

static void set_idt_entry(u8 interrupt_vector, u8 type_and_attributes, void* handler)
{
	IDTEntryDescriptor* entry = &idt[interrupt_vector];

	set_offset_of_idt_entry(entry, (addr_t)handler);
	entry->selector = (u16)GDTEntryOffset::KernelCodeSegment;
	entry->type_and_attributes = type_and_attributes;
	entry->ist = 0; //FIXME: Actually use this like we're supposed to
	entry->reserved = 0;
}

// #################### //
// Initialization Logic //
// #################### //

static GDT custom_gdt = {
	{0, 0, 0, 0x00, 0x00, 0}, // NULL

	// Kernel Segments
	{0, 0, 0, 0x9a, 0xa0, 0}, // CODE
	{0, 0, 0, 0x92, 0xa0, 0}, // DATA

	// Userspace Segments
	{0, 0, 0, 0xfa, 0xa0, 0}, // CODE
	{0, 0, 0, 0xf2, 0xa0, 0}, // DATA
};

void init_descriptor_tables()
{
	// GDT
	DescriptorTablePointer descriptor { };
	descriptor.size = sizeof(custom_gdt) - 1;
	descriptor.offset = (addr_t)&custom_gdt;

	load_gdt(&descriptor);

	klogf(LogLevel::Info, "loaded GDT with size: %x offset: %xl", descriptor.size, descriptor.offset);

	// IDT
	idtr.offset = (addr_t)&idt[0];
	idtr.size = (IDT_ENTRY_COUNT * sizeof(IDTEntryDescriptor)) - 1;

	for(u8 vec = 0; vec < 48; vec++) {
		set_idt_entry(vec, IDT_INTERRUPT_GATE_TA, isr_table[vec]);
	}

	asm volatile("lidt %0" :: "memory"(idtr));
	asm volatile("sti");

	klogf(LogLevel::Info, "loaded IDT with size: %x offset: %xl", idtr.size, idtr.offset);
}

}
