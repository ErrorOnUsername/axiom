#pragma once

#include <ax_util/types.hh>

namespace Kernel {

struct DescriptorTablePointer {
	u16 size;
	u64 offset;
} PACKED;

}
