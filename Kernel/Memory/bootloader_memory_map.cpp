#include "bootloader_memory_map.h"

namespace Kernel::Memory {

const char* memory_map_type_as_string(uint32_t type)
{
	switch(type) {
		case 1:
			return "USABLE";
		case 2:
			return "RESERVED";
		case 3:
			return "ACPI RECLAIMABLE";
		case 4:
			return "ACPI NVS";
		case 5:
			return "BAD MEMORY";
		case 0x1000:
			return "BOOTLOADER RECLAIMABLE";
		case 0x1001:
			return "KERNEL AND MODULES";
		case 0x1002:
			return "FRAMEBUFFER";
		default:
			return "UNKNOWN";
	}
}

}
