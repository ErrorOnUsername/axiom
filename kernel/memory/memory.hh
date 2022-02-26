#pragma once

#include <ax_util/types.hh>

namespace Kernel::Memory {

enum AllocationFlags : uint8_t {
	CLEAR_MEMORY = 0b0001,
	USER_MEMORY  = 0b0010
};

}
