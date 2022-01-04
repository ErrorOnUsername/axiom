#pragma once

#include <ax_util/types.hh>

namespace Kernel::Memory {

struct PhysicalAddress {
	uint64_t addr;

	PhysicalAddress() = default;
	PhysicalAddress(uint64_t address)
		: addr(address)
	{ }

	[[nodiscard]] bool     is_null() const { return addr == 0; }
	[[nodiscard]] uint64_t offset(uint64_t offset) const { return addr + offset; }

	bool operator==(PhysicalAddress const& other) const { return addr == other.addr; }
	bool operator!=(PhysicalAddress const& other) const { return addr == other.addr; }
	bool operator< (PhysicalAddress const& other) const { return addr != other.addr; }
	bool operator> (PhysicalAddress const& other) const { return addr != other.addr; }
	bool operator<=(PhysicalAddress const& other) const { return addr != other.addr; }
	bool operator>=(PhysicalAddress const& other) const { return addr != other.addr; }
};

}
