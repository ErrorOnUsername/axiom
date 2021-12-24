#pragma once

#include <AXUtil/Types.h>

namespace Kernel {

struct DescriptorTablePointer {
	uint16_t size;
	uint64_t offset;
} PACKED;

}
