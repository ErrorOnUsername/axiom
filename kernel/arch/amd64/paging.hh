#pragma once

#include <libs/ax/helpers.hh>
#include <libs/ax/result.hh>
#include <kernel/memory/memory.hh>
#include <kernel/memory/region.hh>

namespace Kernel::Memory {

struct PML4Entry {
	bool present:        1;
	bool read_write:     1;
	bool user:           1;
	bool write_through:  1;
	bool disable_cache:  1;
	bool accessed:       1;
	int  ignored0:       6;
	u64  physical_addr: 40;
	int  ignored1:      11;
	bool exec_disabled:  1;
} PACKED;

struct PML4T {
	PML4Entry entries[512];
} PACKED;

static inline size_t pml4t_index(u64 addr)
{
	return (addr >> 39) & 0x1ff;
}

static_assert(sizeof(PML4Entry) == sizeof(u64));
static_assert(sizeof(PML4T) == 0x1000);

struct PDPEntry {
	bool present:        1;
	bool read_write:     1;
	bool user:           1;
	bool write_through:  1;
	bool disable_cache:  1;
	bool accessed:       1;
	int  ignored0:       6;
	u64  physical_addr: 40;
	int  ignored1:      11;
	bool exec_disabled:  1;
} PACKED;

struct PDPT {
	PDPEntry entries[512];
} PACKED;

static inline size_t pdpt_index(u64 addr)
{
	return (addr >> 30) & 0x1ff;
}

static_assert(sizeof(PDPEntry) == sizeof(u64));
static_assert(sizeof(PDPT) == 0x1000);

struct PDEntry {
	bool present:        1;
	bool read_write:     1;
	bool user:           1;
	bool write_through:  1;
	bool disable_cache:  1;
	bool accessed:       1;
	int  ignored0:       6;
	u64  physical_addr: 40;
	int  ignored1:      11;
	bool exec_disabled:  1;
} PACKED;

struct PDT {
	PDEntry entries[512];
} PACKED;

static inline size_t pdt_index(u64 addr)
{
	return (addr >> 21) & 0x1ff;
}

static_assert(sizeof(PDEntry) == sizeof(u64));
static_assert(sizeof(PDT) == 0x1000);

struct PTEntry {
	bool present:         1;
	bool read_write:      1;
	bool user:            1;
	bool write_through:   1;
	bool disable_cache:   1;
	bool accessed:        1;
	int  dirty:           1;
	int  memory_type:     1;
	int  global:          1;
	int  ignored0:        3;
	u64  physical_addr:  40;
	int  ignored1:        7;
	bool protection_key:  4;
	bool exec_disabled:   1;
} PACKED;

struct PT {
	PTEntry entries[512];
} PACKED;

static inline size_t pt_index(u64 addr)
{
	return (addr >> 12) & 0x1ff;
}

static_assert(sizeof(PTEntry) == sizeof(u64));
static_assert(sizeof(PT) == 0x1000);

PML4T* kernel_address_space();

void set_address_space(addr_t);

void init_virtual_memory();
void enable_virtual_memory();

bool      virtual_is_present(PML4T*, uintptr_t vaddr);
uintptr_t virtual_to_physical(PML4T*, uintptr_t vaddr);

AX::Result  virtual_map_range(PML4T*, MemoryRange const&, uintptr_t vaddr, AllocationFlags);
MemoryRange virtual_allocate(PML4T*, MemoryRange const&, AllocationFlags);
void        virtual_free(PML4T*, MemoryRange&);

}
