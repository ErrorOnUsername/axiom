#pragma once

#include <AXUtil/Types.hh>
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
__attribute__((section("__DATA,.stivale2hdr")))
#else
__attribute__((section(".stivale2hdr"), used))
#endif
static stivale2_header stivale2_header {
	.entry_point = 0,
	.stack = (uintptr_t)k_stack + sizeof(k_stack),
	.flags = (1 << 1),
	.tags = (uintptr_t)&framebuffer_header_tag
};

void* get_stivale2_tag(stivale2_struct* stivale2_struct, uint64_t id);
