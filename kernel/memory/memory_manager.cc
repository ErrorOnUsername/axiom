#include "memory_manager.hh"

#include <ax_util/helpers.hh>
#include <ax_util/result.hh>
#include <kernel/arch/amd64/interrupts/interrupt_disabler.hh>
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
	ASSERT(virtual_range.size % PAGE_SIZE == 0);
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
	ASSERT(physical_range.size % PAGE_SIZE == 0);
	ScopeInterruptDisabler interrupt_disabler;

	physical_set_range_used(physical_range);
	ASSERT(AX::Result::Success == virtual_map_range(address_space, physical_range, physical_range.start, flags));

	if(flags & CLEAR_MEMORY)
		memset((void*)physical_range.start, 0, physical_range.size);

	return AX::Result::Success;
}

AX::Result memory_allocate(uintptr_t* out_addr, PML4Table* address_space, size_t size, AllocationFlags flags)
{ }

AX::Result memory_allocate_identity(uintptr_t* out_addr, PML4Table* address_space, size_t size, AllocationFlags flags)
{ }

void memory_free(PML4Table* address_space, MemoryRange& virtual_range)
{ }

}
