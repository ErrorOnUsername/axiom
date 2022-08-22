#include "paging.hh"

#include <libs/ax/helpers.hh>
#include <kernel/memory/memory.hh>
#include <kernel/memory/memory_manager.hh>
#include <kernel/k_debug.hh>
#include <kernel/panic.hh>

extern "C" void load_cr3(void*);
extern "C" void invalidate_tlb();

namespace Kernel::Memory {

ALIGN(PAGE_SIZE) PML4T kernel_pml4t   = { };
ALIGN(PAGE_SIZE) PDPT  kernel_pdpt    = { };
ALIGN(PAGE_SIZE) PDT   kernel_pdt     = { };
ALIGN(PAGE_SIZE) PT    kernel_pt[512] = { };

PML4T* kernel_address_space()
{
	return &kernel_pml4t;
}

void set_address_space(addr_t addr_space_physical_address)
{
	asm volatile("mov %%rax, %%cr3" :: "a"(addr_space_physical_address) : "memory");
}

void init_virtual_memory()
{
	auto* pml4t_entry = &kernel_pml4t.entries[0];

	pml4t_entry->user          = false;
	pml4t_entry->read_write    = true;
	pml4t_entry->present       = true;
	pml4t_entry->physical_addr = ((addr_t)&kernel_pdpt - KERNEL_VIRTUAL_START) >> 12;

	auto* pdpt_entry = &kernel_pdpt.entries[0];

	pdpt_entry->user          = false;
	pdpt_entry->read_write    = true;
	pdpt_entry->present       = true;
	pdpt_entry->physical_addr = ((addr_t)&kernel_pdt - KERNEL_VIRTUAL_START) >> 12;

	for(size_t i = 0; i < 512; i++) {
		auto* pdt_entry = &kernel_pdt.entries[i];

		pdt_entry->user          = false;
		pdt_entry->read_write    = true;
		pdt_entry->present       = true;
		pdt_entry->physical_addr = ((addr_t)&kernel_pt[i] - KERNEL_VIRTUAL_START) >> 12;
	}
}

void enable_virtual_memory()
{
	set_address_space((addr_t)kernel_address_space() - KERNEL_VIRTUAL_START);
}

PML4T* create_address_space()
{
	auto* pml4t = (PML4T*)memory_allocate_page_identity(kernel_address_space(), CLEAR_MEMORY).start;
	auto* pdpt  = (PDPT*)memory_allocate_page_identity(kernel_address_space(), CLEAR_MEMORY).start;

	auto* pml4t_entry = &pml4t->entries[0];
	pml4t_entry->user          = true;
	pml4t_entry->read_write    = true;
	pml4t_entry->present       = true;
	pml4t_entry->physical_addr = (addr_t)pdpt / PAGE_SIZE;

	auto* pdt  = (PDT*)memory_allocate_page_identity(kernel_address_space(), CLEAR_MEMORY).start;

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

bool virtual_is_present(PML4T* pml4t, uintptr_t vaddr)
{
	auto* pml4t_entry = &pml4t->entries[pml4t_index(vaddr)];
	if(!pml4t_entry->present) return false;

	//////////////////////////////////////////////////
	////////// Page Directory Pointer Table //////////
	//////////////////////////////////////////////////

	addr_t pdpt_addr = pml4t_entry->physical_addr << 12;
	if(pdpt_addr == kernel_to_physical((addr_t)&kernel_pdpt))
		pdpt_addr += KERNEL_VIRTUAL_START;

	auto* pdpt = (PDPT*)pdpt_addr;
	auto* pdpt_entry = &pdpt->entries[pdpt_index(vaddr)];
	if(!pdpt_entry->present) return false;

	//////////////////////////////////////////////////
	////////////// Page Directory Table //////////////
	//////////////////////////////////////////////////

	addr_t pdt_addr = pdpt_entry->physical_addr << 12;
	if(pdt_addr == kernel_to_physical((addr_t)&kernel_pdt))
		pdt_addr += KERNEL_VIRTUAL_START;

	auto* pdt       = (PDT*)pdt_addr;
	auto* pdt_entry = &pdt->entries[pdt_index(vaddr)];
	if(!pdt_entry->present) return false;

	//////////////////////////////////////////////////
	/////////////////// Page Table ///////////////////
	//////////////////////////////////////////////////

	addr_t pt_addr = pdt_entry->physical_addr << 12;
	if((pt_addr >= kernel_to_physical((addr_t)&kernel_pt[0])) && (pt_addr <= kernel_to_physical((addr_t)&kernel_pt[511])))
		pt_addr += KERNEL_VIRTUAL_START;

	auto* pt       = (PT*)pt_addr;
	auto* pt_entry = &pt->entries[pt_index(vaddr)];

	return pt_entry->present;
}

uintptr_t virtual_to_physical(PML4T* pml4t, uintptr_t vaddr)
{
	auto* pml4t_entry = &pml4t->entries[pml4t_index(vaddr)];
	if(!pml4t_entry->present) return 0;

	//////////////////////////////////////////////////
	////////// Page Directory Pointer Table //////////
	//////////////////////////////////////////////////

	addr_t pdpt_addr = pml4t_entry->physical_addr << 12;
	if(pdpt_addr == kernel_to_physical((addr_t)&kernel_pdpt))
		pdpt_addr += KERNEL_VIRTUAL_START;

	auto* pdpt = (PDPT*)pdpt_addr;
	auto* pdpt_entry = &pdpt->entries[pdpt_index(vaddr)];
	if(!pdpt_entry->present) return 0;

	//////////////////////////////////////////////////
	////////////// Page Directory Table //////////////
	//////////////////////////////////////////////////

	addr_t pdt_addr = pdpt_entry->physical_addr << 12;
	if(pdt_addr == kernel_to_physical((addr_t)&kernel_pdt))
		pdt_addr += KERNEL_VIRTUAL_START;

	auto* pdt       = (PDT*)pdt_addr;
	auto* pdt_entry = &pdt->entries[pdt_index(vaddr)];
	if(!pdt_entry->present) return 0;

	//////////////////////////////////////////////////
	/////////////////// Page Table ///////////////////
	//////////////////////////////////////////////////

	addr_t pt_addr = pdt_entry->physical_addr << 12;
	if((pt_addr >= kernel_to_physical((addr_t)&kernel_pt[0])) && (pt_addr <= kernel_to_physical((addr_t)&kernel_pt[511])))
		pt_addr += KERNEL_VIRTUAL_START;

	auto* pt       = (PT*)pt_addr;
	auto* pt_entry = &pt->entries[pt_index(vaddr)];
	if(!pt_entry->present) return 0;

	// We add vaddr & 0xfff so we maintain the offset into the page
	return (pt_entry->physical_addr << 12) + (vaddr & 0xfff);
}

AX::Result virtual_map_range(PML4T* pml4t, MemoryRange const& range, uintptr_t vaddr, AllocationFlags flags)
{
	for(size_t i = 0; i < range.page_count(); i++) {
		addr_t addr = vaddr + i * PAGE_SIZE;

		auto* pml4t_entry = &pml4t->entries[pml4t_index(addr)];

		//////////////////////////////////////////////////
		////////// Page Directory Pointer Table //////////
		//////////////////////////////////////////////////
		addr_t pdpt_addr = pml4t_entry->physical_addr << 12;
		if(pdpt_addr == kernel_to_physical((addr_t)&kernel_pdpt))
			pdpt_addr += KERNEL_VIRTUAL_START;

		auto* pdpt = (PDPT*)pdpt_addr;
		if(!pml4t_entry->present) {
			pdpt = (PDPT*)memory_allocate_page_identity(pml4t, CLEAR_MEMORY).start;

			pml4t_entry->present       = true;
			pml4t_entry->read_write    = true;
			pml4t_entry->user          = true;
			pml4t_entry->physical_addr = (addr_t)pdpt >> 12;
		}

		auto* pdpt_entry = &pdpt->entries[pdpt_index(addr)];

		//////////////////////////////////////////////////
		////////////// Page Directory Table //////////////
		//////////////////////////////////////////////////

		addr_t pdt_addr = pdpt_entry->physical_addr << 12;
		if(pdt_addr == kernel_to_physical((addr_t)&kernel_pdt))
			pdt_addr += KERNEL_VIRTUAL_START;

		auto* pdt = (PDT*)pdt_addr;
		if(!pdpt_entry->present) {
			pdt = (PDT*)memory_allocate_page_identity(pml4t, CLEAR_MEMORY).start;

			pdpt_entry->present       = true;
			pdpt_entry->read_write    = true;
			pdpt_entry->user          = true;
			pdpt_entry->physical_addr = (addr_t)pdt >> 12;
		}

		auto* pdt_entry = &pdt->entries[pdt_index(addr)];

		//////////////////////////////////////////////////
		/////////////////// Page Table ///////////////////
		//////////////////////////////////////////////////

		addr_t pt_addr = pdt_entry->physical_addr << 12;
		if((pt_addr >= kernel_to_physical((addr_t)&kernel_pt[0])) && (pt_addr <= kernel_to_physical((addr_t)&kernel_pt[511])))
			pt_addr += KERNEL_VIRTUAL_START;

		auto* pt = (PT*)pt_addr;
		if(!pdt_entry->present) {
			pt = (PT*)memory_allocate_page_identity(pml4t, CLEAR_MEMORY).start;

			pdt_entry->present       = true;
			pdt_entry->read_write    = true;
			pdt_entry->user          = true;
			pdt_entry->physical_addr = (addr_t)pt >> 12;
		}

		auto* pt_entry = &pt->entries[pt_index(addr)];

		pt_entry->present       = true;
		pt_entry->read_write    = true;
		pt_entry->user          = flags & USER_MEMORY;
		pt_entry->physical_addr = (range.start + i * PAGE_SIZE) >> 12;

	}

	invalidate_tlb();

	return AX::Result::Success;
}

MemoryRange virtual_allocate(PML4T* pml4t, MemoryRange const& physical_range, AllocationFlags flags)
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

void virtual_free(PML4T* pml4t, MemoryRange& virtual_range)
{
	bool is_kernel_addr_space = (addr_t)pml4t >= KERNEL_VIRTUAL_START;

	for(size_t i = 0; i < virtual_range.page_count(); i++) {
		addr_t addr = virtual_range.start + i * PAGE_SIZE;

		auto* pml4t_entry = &pml4t->entries[pml4t_index(addr)];
		if(!pml4t_entry->present) continue;

		auto* pdpt       = (PDPT*)((pml4t_entry->physical_addr << 12) + ((is_kernel_addr_space) ? 0 : KERNEL_VIRTUAL_START));
		auto* pdpt_entry = &pdpt->entries[pdpt_index(addr)];
		if(!pdpt_entry->present) continue;

		auto* pdt       = (PDT*)((pdpt_entry->physical_addr << 12) + ((is_kernel_addr_space) ? 0 : KERNEL_VIRTUAL_START));
		auto* pdt_entry = &pdt->entries[pdt_index(addr)];
		if(!pdt_entry->present) continue;

		auto* pt       = (PT*)((pdpt_entry->physical_addr << 12) + ((is_kernel_addr_space) ? 0 : KERNEL_VIRTUAL_START));
		auto* pt_entry = &pt->entries[pt_index(addr)];

		*pt_entry = { };
	}

	invalidate_tlb();
}

}
