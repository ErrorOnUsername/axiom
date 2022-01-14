#pragma once

#include <ax_util/helpers.hh>

namespace Kernel::Memory {

struct PML4Entry {
	bool     present:        1;
	bool     read_write:     1;
	bool     user:           1;
	bool     write_through:  1;
	bool     disable_cache:  1;
	bool     accessed:       1;
	int      ignored0:       6;
	uint64_t physical_addr: 36;
	int      ignored1:      15;
	bool     exec_disabled:  1;
} PACKED;

struct PML4Table {
	PML4Entry entries[512];
} PACKED;

static inline size_t pml4_index(uint64_t addr)
{
	return (addr >> 39) & 0x1ff;
}

static_assert(sizeof(PML4Entry) == sizeof(uint64_t));
static_assert(sizeof(PML4Table) == 0x1000);

struct PDPEntry {
	bool     present:        1;
	bool     read_write:     1;
	bool     user:           1;
	bool     write_through:  1;
	bool     disable_cache:  1;
	bool     accessed:       1;
	int      ignored0:       1;
	int      size:           1;
	int      ignored1:       4;
	uint64_t physical_addr: 36;
	int      ignored2:      15;
	bool     exec_disabled:  1;
} PACKED;

struct PDPTable {
	PDPEntry entires[512];
} PACKED;

static inline size_t pdp_index(uint64_t addr)
{
	return (addr >> 30) & 0x1ff;
}

static_assert(sizeof(PDPEntry) == sizeof(uint64_t));
static_assert(sizeof(PDPTable) == 0x1000);

struct PDEntry {
	bool     present:        1;
	bool     read_write:     1;
	bool     user:           1;
	bool     write_through:  1;
	bool     disable_cache:  1;
	bool     accessed:       1;
	int      ignored0:       1;
	int      size:           1;
	int      ignored1:       4;
	uint64_t physical_addr: 36;
	int      ignored2:      15;
	bool     exec_disabled:  1;
} PACKED;

struct PDTable {
	PDEntry entries[512];
} PACKED;

static inline size_t pd_index(uint64_t addr)
{
	return (addr >> 21) & 0x1ff;
}

static_assert(sizeof(PDEntry) == sizeof(uint64_t));
static_assert(sizeof(PDTable) == 0x1000);

struct PTEntry {
	bool     present:         1;
	bool     read_write:      1;
	bool     user:            1;
	bool     write_through:   1;
	bool     disable_cache:   1;
	bool     accessed:        1;
	int      dirty:           1;
	int      memory_type:     1;
	int      global:          1;
	int      ignored0:        3;
	uint64_t physical_addr:  36;
	int      ignored1:       10;
	bool     protection_key:  5;
	bool     exec_disabled:   1;
} PACKED;

struct PTTable {
	PTEntry entries[512];
} PACKED;

static inline size_t pt_index(uint64_t addr)
{
	return (addr >> 12) & 0x1ff;
}

static_assert(sizeof(PTEntry) == sizeof(uint64_t));
static_assert(sizeof(PTTable) == 0x1000);

PML4Table* kernel_pml4();

void init_virtual_memory();
void switch_address_space(PML4Table*);

}
