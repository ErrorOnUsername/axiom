#pragma once

#include <ax_util/types.hh>

namespace Kernel::EarlyConsole {

void initialize_early_console(uint32_t* framebuffer_addr, uint16_t framebuffer_width, uint16_t framebuffer_height);

void put_char(char c);
void draw_char(char c);

}
