#include "paging.hh"

#include <ax_util/helpers.hh>
#include <kernel/memory/memory_manager.hh>
#include <kernel/k_debug.hh>
#include <kernel/panic.hh>

extern "C" void load_cr3(void*);
extern "C" void invalidate_tlb();

namespace Kernel::Memory {

ALIGN(PAGE_SIZE) PML4Table kernel_pml4t   = { };
ALIGN(PAGE_SIZE) PDPTable  kernel_pdpt    = { };
ALIGN(PAGE_SIZE) PDTable   kernel_pdt     = { };
ALIGN(PAGE_SIZE) PT        kernel_pt[512] = { };

PML4Table* kernel_address_space()
{
	return &kernel_pml4t;
}

void switch_address_space(PML4Table* pml4)
{
	load_cr3(pml4);
}

void init_virtual_memory()
{
	auto* pml4t_entry = &kernel_pml4t.entries[0];

	pml4t_entry->user          = false;
	pml4t_entry->read_write    = true;
	pml4t_entry->present       = true;
	pml4t_entry->physical_addr = (addr_t)&kernel_pdpt / PAGE_SIZE;

	auto* pdpt_entry = &kernel_pdpt.entries[0];

	pdpt_entry->user          = false;
	pdpt_entry->read_write    = true;
	pdpt_entry->present       = true;
	pdpt_entry->physical_addr = (addr_t)&kernel_pdt / PAGE_SIZE;

	for(size_t i = 0; i < 512; i++) {
		auto* pdt_entry = &kernel_pdt.entries[i];

		pdt_entry->user          = false;
		pdt_entry->read_write    = true;
		pdt_entry->present       = true;
		pdt_entry->physical_addr = (addr_t)&kernel_pt[i] / PAGE_SIZE;
	}
}

void enable_virtual_memory()
{
	switch_address_space(kernel_address_space());
}

PML4Table* create_address_space()
{
	auto* pml4t = (PML4Table*)memory_allocate_page_identity(kernel_address_space(), CLEAR_MEMORY).start;
	auto* pdpt  = (PDPTable*)memory_allocate_page_identity(kernel_address_space(), CLEAR_MEMORY).start;

	auto* pml4t_entry = &pml4t->entries[0];
	pml4t_entry->user          = true;
	pml4t_entry->read_write    = true;
	pml4t_entry->present       = true;
	pml4t_entry->physical_addr = (addr_t)pdpt / PAGE_SIZE;

	auto* pdt  = (PDTable*)memory_allocate_page_identity(kernel_address_space(), CLEAR_MEMORY).start;

	auto* pdpt_entry = &pdpt->entries[0];
	pdpt_entry->user          = true;
	pdpt_entry->read_write    = true;
	pdpt_entry->present       = true;
	pdpt_entry->physical_addr = (addr_t)pdt / PAGE_SIZE;

	for(size_t i = 0; i < 512; i++) {
		auto* pdt_entry = &pdt->entries[i];

		pdt_entry->user          = true;
		pdt_entry->read_write    = true;
		pdt_entry->present       = true;
		pdt_entry->physical_addr = (addr_t)&kernel_pt[i] / PAGE_SIZE;
	}

	return pml4t;
}

bool virtual_is_present(PML4Table* pml4t, uintptr_t vaddr)
{
	auto* pml4t_entry = &pml4t->entries[pml4t_index(vaddr)];
	if(!pml4t_entry->present) return false;

	auto* pdpt       = (PDPTable*)(pml4t_entry->physical_addr * PAGE_SIZE);
	auto* pdpt_entry = &pdpt->entries[pdpt_index(vaddr)];
	if(!pdpt_entry->present) return false;

	auto* pdt       = (PDTable*)(pdpt_entry->physical_addr * PAGE_SIZE);
	auto* pdt_entry = &pdt->entries[pdt_index(vaddr)];
	if(!pdt_entry->present) return false;

	auto* pt       = (PT*)(pdt_entry->physical_addr * PAGE_SIZE);
	auto* pt_entry = &pt->entries[pt_index(vaddr)];
	return pt_entry->present;
}

uintptr_t virtual_to_physical(PML4Table* pml4t, uintptr_t vaddr)
{
	auto* pml4t_entry = &pml4t->entries[pml4t_index(vaddr)];
	if(!pml4t_entry->present) return 0;

	auto* pdpt       = (PDPTable*)(pml4t_entry->physical_addr * PAGE_SIZE);
	auto* pdpt_entry = &pdpt->entries[pdpt_index(vaddr)];
	if(!pdpt_entry->present) return 0;

	auto* pdt       = (PDTable*)(pdpt_entry->physical_addr * PAGE_SIZE);
	auto* pdt_entry = &pdt->entries[pdt_index(vaddr)];
	if(!pdt_entry->present) return 0;

	auto* pt       = (PT*)(pdt_entry->physical_addr * PAGE_SIZE);
	auto* pt_entry = &pt->entries[pt_index(vaddr)];
	if(!pt_entry->present) return 0;

	return (pt_entry->physical_addr * PAGE_SIZE) + (vaddr & 0xfff);
}

AX::Result virtual_map_range(PML4Table* pml4t, MemoryRange const& range, uintptr_t vaddr, AllocationFlags flags)
{
	for(size_t i = 0; i < range.page_count(); i++) {
		addr_t addr = vaddr + i * PAGE_SIZE;

		auto* pml4t_entry = &pml4t->entries[pml4t_index(addr)];

		auto* pdpt = (PDPTable*)(pml4t_entry->physical_addr * PAGE_SIZE);
		if(!pml4t_entry->present) {
			pdpt = (PDPTable*)memory_allocate_page_identity(pml4t, CLEAR_MEMORY).start;

			pml4t_entry->present       = true;
			pml4t_entry->read_write    = true;
			pml4t_entry->user          = true;
			pml4t_entry->physical_addr = (addr_t)pdpt / PAGE_SIZE;
		}

		auto* pdpt_entry = &pdpt->entries[pdpt_index(addr)];

		auto* pdt = (PDTable*)(pdpt_entry->physical_addr * PAGE_SIZE);
		if(!pdpt_entry->present) {
			pdt = (PDTable*)memory_allocate_page_identity(pml4t, CLEAR_MEMORY).start;

			pdpt_entry->present       = true;
			pdpt_entry->read_write    = true;
			pdpt_entry->user          = true;
			pdpt_entry->physical_addr = (addr_t)pdt / PAGE_SIZE;
		}

		auto* pdt_entry = &pdt->entries[pdt_index(addr)];

		auto* pt = (PT*)(pdt_entry->physical_addr * PAGE_SIZE);
		if(pdt_entry->present) {
			pt = (PT*)memory_allocate_page_identity(pml4t, CLEAR_MEMORY).start;

			pdt_entry->present       = true;
			pdt_entry->read_write    = true;
			pdt_entry->user          = true;
			pdt_entry->physical_addr = (addr_t)pt / PAGE_SIZE;
		}

		auto* pt_entry = &pt->entries[pt_index(addr)];

		pt_entry->present       = true;
		pt_entry->read_write    = true;
		pt_entry->user          = flags & USER_MEMORY;
		pt_entry->physical_addr = (range.start + i * PAGE_SIZE) / PAGE_SIZE;
	}

	invalidate_tlb();

	return AX::Result::Success;
}

MemoryRange virtual_allocate(PML4Table* pml4t, MemoryRange const& physical_range, AllocationFlags flags)
{
	uintptr_t vaddr          = 0;
	size_t    current_size   = 0;
	bool      is_user_memory = flags & USER_MEMORY;

	for(size_t i = ((is_user_memory) ? 1 : (KERNEL_VIRTUAL_START / PAGE_SIZE));
	           i < ((is_user_memory) ? KERNEL_VIRTUAL_START : 0xffffffffffffffff);
	           i++)
	{
		uintptr_t current_addr = i * PAGE_SIZE;

		if(!virtual_is_present(pml4t, vaddr)) {
			if(current_size == 0) {
				vaddr = current_addr;
			}

			current_size += PAGE_SIZE;

			if(current_size == physical_range.size) {
				ASSERT(AX::Result::Success == virtual_map_range(pml4t, physical_range, vaddr, flags));
				return MemoryRange {
					.start = vaddr,
					.size  = current_size
				};
			}
		}
	}

	panic("OOM! Out of virtual memory! How??");
	return MemoryRange { 0, 0 };
}

void virtual_free(PML4Table* pml4t, MemoryRange& virtual_range)
{
	for(size_t i = 0; i < virtual_range.page_count(); i++) {
		addr_t addr = virtual_range.start + i * PAGE_SIZE;

		auto* pml4t_entry = &pml4t->entries[pml4t_index(addr)];
		if(!pml4t_entry->present) continue;

		auto* pdpt       = (PDPTable*)(pml4t_entry->physical_addr * PAGE_SIZE);
		auto* pdpt_entry = &pdpt->entries[pdpt_index(addr)];
		if(!pdpt_entry->present) continue;

		auto* pdt       = (PDTable*)(pdpt_entry->physical_addr * PAGE_SIZE);
		auto* pdt_entry = &pdt->entries[pdt_index(addr)];
		if(!pdt_entry->present) continue;

		auto* pt       = (PT*)(pdt_entry->physical_addr * PAGE_SIZE);
		auto* pt_entry = &pt->entries[pt_index(addr)];

		*pt_entry = { };
	}

	invalidate_tlb();
}

}
