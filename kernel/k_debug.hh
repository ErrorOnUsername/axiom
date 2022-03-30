#pragma once
#include <stdarg.h>

#include <ax_util/types.hh>

#define klogf(mode, msg, ...)                                                        \
	Kernel::klog_impl(mode                                                           \
	                , Kernel::DebugFileLocation { __FILE__, __FUNCTION__, __LINE__ } \
					, msg                                                            \
					, __VA_ARGS__)

#define klog(mode, msg)                                                              \
	Kernel::klog_impl(mode                                                           \
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
	u32         line;

	char const* get_root_filename() const;
};

extern "C" int k_printf(char const* fmt, ...);
extern "C" int k_vprintf(char const* fmt, va_list args);

void klog_impl(LogLevel, DebugFileLocation, char const* fmt, ...);

}
