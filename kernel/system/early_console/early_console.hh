#pragma once

#include <libs/ax/types.hh>

namespace Kernel::EarlyConsole {

void init(u32* framebuffer_addr, u16 framebuffer_width, u16 framebuffer_height);

void put_char(char c);
void draw_char(char c);

}
