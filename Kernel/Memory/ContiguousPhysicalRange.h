#pragma once

#include <AXUtil/Types.h>

namespace Kernel::Memory {

struct ContiguousPhysicalRange {
	uint64_t base_address;
	uint64_t upper_address;
};

}
