#include "memory_manager.hh"

#include <ax_util/helpers.hh>
#include <ax_util/result.hh>
#include <kernel/arch/amd64/interrupts/interrupt_disabler.hh>
#include <kernel/arch/amd64/paging.hh>
#include <kernel/memory/bootloader_memory_map.hh>
#include <kernel/memory/memory.hh>
#include <kernel/memory/physical.hh>
#include <kernel/memory/region.hh>
#include <kernel/k_debug.hh>
#include <kernel/panic.hh>
#include <kernel/std.hh>

namespace Kernel::Memory {

static MemoryRange kernel_physical_range(BootloaderMemoryMap&);
static MemoryRange framebuffer_physical_range(BootloaderMemoryMap&);

void init_memory_management(BootloaderMemoryMap& memmap)
{
	init_physical_memory_manager(memmap);
	init_virtual_memory();

	MemoryRange kernel_range = kernel_physical_range(memmap);
	MemoryRange fbuff_range  = framebuffer_physical_range(memmap);
	virtual_map_range(kernel_address_space(), kernel_range, kernel_range.start + KERNEL_VIRTUAL_START, MEMORY_NONE);
	virtual_map_range(kernel_address_space(), fbuff_range,  fbuff_range.start  + IO_VIRTUAL_START,     MEMORY_NONE);

	MemoryRange zero_page = { 0, PAGE_SIZE };
	virtual_free(kernel_address_space(), zero_page);

	enable_virtual_memory();
}

AX::Result memory_map(PML4T* address_space, MemoryRange& virtual_range, AllocationFlags flags)
{
	ASSERT(virtual_range.is_page_aligned());
	ScopeInterruptDisabler interrupt_disabler;

	for(size_t i = 0; i < virtual_range.page_count(); i++) {
		uintptr_t vaddr = virtual_range.start + i * PAGE_SIZE;

		if(!virtual_is_present(address_space, vaddr)) {
			MemoryRange physical_range = physical_allocate_pages(1);
			AX::Result  map_result     = virtual_map_range(address_space, physical_range, vaddr, flags);

			if(map_result != AX::Result::Success) return map_result;
		}
	}

	if(flags & CLEAR_MEMORY)
		memset((void*)virtual_range.start, 0, virtual_range.size);

	return AX::Result::Success;
}

AX::Result memory_map_indentity(PML4T* address_space, MemoryRange& physical_range, AllocationFlags flags)
{
	ASSERT(physical_range.is_page_aligned());
	ScopeInterruptDisabler interrupt_disabler;

	physical_set_range_used(physical_range);
	ASSERT(AX::Result::Success == virtual_map_range(address_space, physical_range, physical_range.start, flags));

	if(flags & CLEAR_MEMORY)
		memset((void*)physical_range.start, 0, physical_range.size);

	return AX::Result::Success;
}

MemoryRange memory_allocate(PML4T* address_space, size_t size, AllocationFlags flags)
{
	ASSERT(size % PAGE_SIZE == 0);
	ScopeInterruptDisabler interrupt_disabler;

	if(size == 0) {
		klog(LogLevel::Warning, "Allocation of size: 0");
		return MemoryRange { };
	}

	// FIXME: Both of these just panic when they run out of memory. Maybe we
	//        should handle it more gracefilly?
	MemoryRange physical_range = physical_allocate_page_aligned_range_in_bytes(size);
	MemoryRange virtual_range  = virtual_allocate(address_space, physical_range, flags);

	if(flags & CLEAR_MEMORY)
		memset((void*)virtual_range.start, 0, size);

	return virtual_range;
}

MemoryRange memory_allocate_page_identity(PML4T* address_space, AllocationFlags flags)
{
	ScopeInterruptDisabler interrupt_disabler;

	// Just loop through all the available physical pages. Since we couldn't
	// allocate over that if we wanted to.
	MemoryRange identity_range = { 0, PAGE_SIZE };
	for(size_t i = 1; i <= physical_highest_page(); i++) {
		identity_range.start = i * PAGE_SIZE;

		if(!virtual_is_present(address_space, identity_range.start) && !physical_is_range_used(identity_range)) {
			physical_set_range_used(identity_range);
			ASSERT(AX::Result::Success == virtual_map_range(address_space, identity_range, identity_range.start, flags));

			if(flags & CLEAR_MEMORY)
				memset((void*)identity_range.start, 0, PAGE_SIZE);

			return identity_range;
		}
	}

	panic("memory_allocate_page_identity failed!!");
	return MemoryRange { };
}

void memory_free(PML4T* address_space, MemoryRange& virtual_range)
{
	ASSERT(virtual_range.is_page_aligned());
	ScopeInterruptDisabler interrupt_disabler;

	for(size_t i = 0; i < virtual_range.size / PAGE_SIZE; i++) {
		uintptr_t vaddr = virtual_range.start + i * PAGE_SIZE;

		if(virtual_is_present(address_space, vaddr)) {
			MemoryRange physical_page_range = { virtual_to_physical(address_space, vaddr), PAGE_SIZE };
			MemoryRange virtual_page_range  = { vaddr, PAGE_SIZE };

			physical_free_memory_range(physical_page_range);
			virtual_free(address_space, virtual_page_range);
		}
	}
}

static MemoryRange kernel_physical_range(BootloaderMemoryMap& memory_map)
{
	for(size_t i = 0; i < memory_map.length; i++) {
		BootloaderMemoryMapEntry const& entry = memory_map.entries[i];
		if(entry.type == MemoryRegionType::KernelAndModules) {
			return MemoryRange { entry.address, ALIGN_UP(entry.size, PAGE_SIZE) };
		}
	}
	
	return MemoryRange { 0, 0 };
}

static MemoryRange framebuffer_physical_range(BootloaderMemoryMap& memory_map)
{
	for(size_t i = 0; i < memory_map.length; i++) {
		BootloaderMemoryMapEntry const& entry = memory_map.entries[i];
		if(entry.type == MemoryRegionType::Framebuffer) {
			return MemoryRange { entry.address, ALIGN_UP(entry.size, PAGE_SIZE) };
		}
	}
	
	return MemoryRange { 0, 0 };
}

}
