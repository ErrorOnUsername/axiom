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
#include <kernel/std.hh>

namespace Kernel::Memory {

void init_memory_management(BootloaderMemoryMap& memory_map)
{
	init_physical_memory_manager(memory_map);
	init_virtual_memory();
}

AX::Result memory_map(PML4Table* address_space, MemoryRange& virtual_range, AllocationFlags flags)
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

AX::Result memory_map_indentity(PML4Table* address_space, MemoryRange& physical_range, AllocationFlags flags)
{
	ASSERT(physical_range.is_page_aligned());
	ScopeInterruptDisabler interrupt_disabler;

	physical_set_range_used(physical_range);
	ASSERT(AX::Result::Success == virtual_map_range(address_space, physical_range, physical_range.start, flags));

	if(flags & CLEAR_MEMORY)
		memset((void*)physical_range.start, 0, physical_range.size);

	return AX::Result::Success;
}

MemoryRange memory_allocate(PML4Table* address_space, size_t size, AllocationFlags flags)
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

MemoryRange memory_allocate_page_identity(PML4Table* address_space, AllocationFlags flags)
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

	// FIXME: Panic!
	ASSERT(false);
	return MemoryRange { };
}

void memory_free(PML4Table* address_space, MemoryRange& virtual_range)
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

}
