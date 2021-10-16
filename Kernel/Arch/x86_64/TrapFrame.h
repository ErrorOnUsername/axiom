#pragma once

#include <AXUtil/Helpers.h>
#include <Kernel/Arch/x86_64/RegisterState.h>

struct TrapFrame {
	RegisterState register_state;
} PACKED;
