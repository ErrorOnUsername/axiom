#pragma once

#include <ax_util/types.hh>

namespace Kernel {

struct DescriptorTablePointer {
	uint16_t size;
	uint64_t offset;
} PACKED;

}
