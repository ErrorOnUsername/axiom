#include <stdint.h>
#include <stddef.h>

#include <AXUtil/types.h>

#include <Kernel/Boot/x86_64/stivale2.h>

static uint8_t k_stack[4096];

static stivale2_header_tag_terminal terminal_header_tag {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
		.next = 0
	},
	.flags = 0
};

static stivale2_header_tag_framebuffer framebuffer_header_tag {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
		.next = (uint64_t)&terminal_header_tag
	},
	.framebuffer_width = 0,
	.framebuffer_height = 0,
	.framebuffer_bpp = 0
};

#ifdef __APPLE__
__attribute__((section("DATA,.stivale2hdr"), used))
#else
__attribute__((section(".stivale2hdr"), used))
#endif
static stivale2_header stivale2_header {
	.entry_point = 0,
	.stack = (uintptr_t)k_stack + sizeof(k_stack),
	.flags = (1 << 1),
	.tags = (uintptr_t)&framebuffer_header_tag
};

void* get_stivale2_tag(stivale2_struct* stivale2_struct, uint64_t id) {
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

void k_start(stivale2_struct* stivale2_struct) {
	auto* terminal_tag = (stivale2_struct_tag_terminal*)get_stivale2_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

	if(terminal_tag == nullptr)
		for(;;);

	void* term_write_ptr = (void*)terminal_tag->term_write;

	void (*term_write) (const char* str, size_t length) = (void (*) (const char*, size_t))term_write_ptr;

	term_write("Hello, World", 12);

	for(;;) {}
}
