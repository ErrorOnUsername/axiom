#include "paging.hh"

#include <ax_util/helpers.hh>
#include <kernel/arch/amd64/asm_utils.hh>
#include <kernel/k_debug.hh>

namespace Kernel::Memory {

ALIGN(PAGE_SIZE) PML4Table kernel_pml4t   = { };
ALIGN(PAGE_SIZE) PDPTable  kernel_pdpt    = { };
ALIGN(PAGE_SIZE) PDTable   kernel_pdt     = { };
ALIGN(PAGE_SIZE) PT        kernel_pt[512] = { };

PML4Table* kernel_pml4()
{
	return &kernel_pml4t;
}

void switch_address_space(PML4Table* pml4)
{
	load_cr3(pml4);
}

void init_virtual_memory()
{
	auto& pml4_entry = kernel_pml4t.entries[0];

	pml4_entry.user          = false;
	pml4_entry.read_write    = true;
	pml4_entry.present       = true;
	pml4_entry.physical_addr = (addr_t)&kernel_pdpt / PAGE_SIZE;

	auto& pdpt_entry = kernel_pdpt.entries[0];

	pdpt_entry.user          = false;
	pdpt_entry.read_write    = true;
	pdpt_entry.present       = true;
	pdpt_entry.physical_addr = (addr_t)&kernel_pdt / PAGE_SIZE;

	for(size_t i = 0; i < 512; i++) {
		auto& pdt_entry = kernel_pdt.entries[i];

		pdt_entry.user          = false;
		pdt_entry.read_write    = true;
		pdt_entry.present       = true;
		pdt_entry.physical_addr = (addr_t)&kernel_pt[i] / PAGE_SIZE;
	}
}

void enable_virtual_memory()
{
	switch_address_space(kernel_pml4());
}

bool virtual_is_present(PML4Table* pml4t, uintptr_t vaddr)
{
	auto pml4_entry = pml4t->entries[pml4t_index(vaddr)];
	if(!pml4_entry.present) return false;

	auto pdpt       = reinterpret_cast<PDPTable*>(pml4_entry.physical_addr * PAGE_SIZE);
	auto pdpt_entry = pdpt->entries[pdpt_index(vaddr)];
	if(!pdpt_entry.present) return false;

	auto pdt       = reinterpret_cast<PDTable*>(pdpt_entry.physical_addr * PAGE_SIZE);
	auto pdt_entry = pdt->entries[pdt_index(vaddr)];
	if(!pdt_entry.present) return false;

	auto pt       = reinterpret_cast<PT*>(pdt_entry.physical_addr * PAGE_SIZE);
	auto pt_entry = pt->entries[pt_index(vaddr)];
	return pt_entry.present;
}

uintptr_t virtual_to_physical(PML4Table* pml4t, uintptr_t vaddr)
{
	auto pml4_entry = pml4t->entries[pml4t_index(vaddr)];
	if(!pml4_entry.present) return 0;

	auto pdpt       = reinterpret_cast<PDPTable*>(pml4_entry.physical_addr * PAGE_SIZE);
	auto pdpt_entry = pdpt->entries[pdpt_index(vaddr)];
	if(!pdpt_entry.present) return 0;

	auto pdt       = reinterpret_cast<PDTable*>(pdpt_entry.physical_addr * PAGE_SIZE);
	auto pdt_entry = pdt->entries[pdt_index(vaddr)];
	if(!pdt_entry.present) return 0;

	auto pt       = reinterpret_cast<PT*>(pdt_entry.physical_addr * PAGE_SIZE);
	auto pt_entry = pt->entries[pt_index(vaddr)];
	if(!pt_entry.present) return 0;

	return (pt_entry.physical_addr * PAGE_SIZE) + (vaddr & 0xfff);
}

}
