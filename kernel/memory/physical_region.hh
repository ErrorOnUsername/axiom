#pragma once

#include <ax_util/helpers.hh>
#include <kernel/memory/physical_address.hh>

namespace Kernel::Memory {

struct PhysicalRegion {
	PhysicalAddress m_base;

private:
	MAKE_NONCOPYABLE(PhysicalRegion);
	MAKE_NONMOVEABLE(PhysicalRegion);
};

}
