#pragma once
#include <stdarg.h>

#include <AXUtil/Types.h>

#define klogf(mode, msg, ...)                                                               \
	Kernel::klog_impl(mode                                                                  \
	                , Kernel::DebugFileLocation { __FILE__, __FUNCTION__, __LINE__ } \
					, msg                                                                   \
					, __VA_ARGS__)

#define klog(mode, msg)                                                                     \
	Kernel::klog_impl(mode                                                                  \
	                , Kernel::DebugFileLocation { __FILE__, __FUNCTION__, __LINE__ } \
					, msg)

namespace Kernel {

enum LogMode : uint16_t {
	LogModeInfo          = 1 << 0,
	LogModeWarning       = 1 << 1,
	LogModeError         = 1 << 2,
	LogModeBoot          = 1 << 3,
	LogModeKMalloc       = 1 << 4,
	LogModeMemoryManager = 1 << 5,
	LogModeScheduler     = 1 << 6,
};

struct DebugFileLocation {
	char const* filepath;
	char const* function_name;
	uint32_t line;

	char const* get_root_filename();
};

extern "C" int k_printf(char const* fmt, ...);
extern "C" int k_vprintf(char const* fmt, va_list args);

void klog_impl(uint16_t log_mode, DebugFileLocation calling_file, char const* fmt, ...);

}
