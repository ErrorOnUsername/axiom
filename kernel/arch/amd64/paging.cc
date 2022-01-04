#include "paging.hh"

#include <ax_util/Helpers.hh>
#include <kernel/arch/amd64/asm_util.hh>

namespace Kernel::Memory {

ALIGN(PAGE_SIZE) PML4Table kernel_pml4t    = { };
ALIGN(PAGE_SIZE) PDPTable  kernel_pdpt     = { };
ALIGN(PAGE_SIZE) PDTable   kernel_pdt      = { };
ALIGN(PAGE_SIZE) PTTable   kernel_ptt[512] = { };

void init_virtual_memory()
{
	auto& pml4_entry = kernel_pml4t.entries[0];
	pml4_entry.user          = false;
	pml4_entry.read_write    = true;
	pml4_entry.present       = true;
	pml4_entry.physical_addr = (uint64_t)&kernel_pdpt / PAGE_SIZE;

	auto& pdpt_entry = kernel_pdpt.entires[0];
	pdpt_entry.user          = false;
	pdpt_entry.read_write    = true;
	pdpt_entry.present       = true;
	pdpt_entry.physical_addr = (uint64_t)&kernel_pdt / PAGE_SIZE;

	for(size_t i = 0; i < 512; i++) {
		auto& pdt_entry = kernel_pdt.entries[i];
		pdt_entry.user = false;
		pdt_entry.read_write = true;
		pdt_entry.present = true;
		pdt_entry.physical_addr = (uint64_t)&kernel_ptt[i] / PAGE_SIZE;
	}
}

PML4Table* kernel_pml4()
{
	return &kernel_pml4t;
}

void switch_address_space(PML4Table* pml4)
{
	load_cr3((uint64_t)pml4);
}

};
