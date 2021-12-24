#pragma once

#include <AXUtil/Helpers.h>
#include <kernel/memory/physical_address.h>

namespace Kernel::Memory {

struct PhysicalRegion {
	PhysicalAddress m_base;

private:
	MAKE_NONCOPYABLE(PhysicalRegion);
	MAKE_NONMOVEABLE(PhysicalRegion);
};

}
