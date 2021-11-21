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

enum class LogLevel {
	Info,
	Warning,
	Error,
};

struct DebugFileLocation {
	char const* filepath;
	char const* function_name;
	uint32_t line;

	char const* get_root_filename();
};

extern "C" int k_printf(char const* fmt, ...);
extern "C" int k_vprintf(char const* fmt, va_list args);

void klog_impl(LogLevel log_level, DebugFileLocation calling_file, char const* fmt, ...);

}
