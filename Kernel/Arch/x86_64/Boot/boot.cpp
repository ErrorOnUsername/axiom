#include <AXUtil/Types.h>
#include <Kernel/Arch/x86_64/Boot/stivale2.h>

namespace Kernel {

static uint8_t k_stack[4096];

static stivale2_header_tag_framebuffer framebuffer_header_tag {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
		.next = 0,
	},
	.framebuffer_width = 1280,
	.framebuffer_height = 1024,
	.framebuffer_bpp = 32,
};

SECTION(.stivale2hdr)
static stivale2_header stivale2_header {
	.entry_point = 0,
	.stack = (uintptr_t)k_stack + sizeof(k_stack),
	.flags = (1 << 1),
	.tags = (uintptr_t)&framebuffer_header_tag
};

void* get_stivale2_tag(stivale2_struct* stivale2_struct, uint64_t id)
{

	auto* current_tag = (stivale2_tag*)stivale2_struct->tags;
	for(;;) {
		if(current_tag == nullptr) {
			return nullptr;
		}
		if(current_tag->identifier == id) {
			return current_tag;
		}

		current_tag = (stivale2_tag*)current_tag->next;
	}
}

const char* memmap_type_as_string(uint32_t type)
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
			return "UNDEFINED";
	}
}

}
