#pragma once

#include <AXUtil/Types.h>

namespace Kernel {

struct TextConsole {
	uint32_t* framebuffer;
	uint16_t  framebuffer_width;
	uint16_t  framebuffer_height;

	uint16_t row;
	uint16_t column;

	TextConsole(uint32_t* framebuffer_raw, uint16_t width, uint16_t height);

	void put_char(char c);
	void draw_char(char c);
	
	static constexpr uint8_t CHARACTER_WIDTH  = 8;
	static constexpr uint8_t CHARACTER_HEIGHT = 16;
	static constexpr uint8_t TAB_WIDTH        = 4;
};

}
