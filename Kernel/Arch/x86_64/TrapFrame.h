#pragma once

#include <AXUtil/Types.h>
#include <Kernel/Arch/x86_64/RegisterState.h>

struct TrapFrame {
	RegisterState register_state;
} PACKED;
