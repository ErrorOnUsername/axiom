#pragma once

#include <AXUtil/types.h>

class VGATextModeDevice {
public:
	VGATextModeDevice();
	void clear_screen();
	void set_character(uint8_t column, uint8_t row, char c, uint8_t attribute = 7);
	void write_line(const char* msg, uint8_t attribute = 7);

	static constexpr int32_t columns() { return s_columns; }
	static constexpr int32_t rows() { return s_rows; }

private:
	uint8_t* m_text_buffer = (uint8_t*)0xb8000;

	uint8_t m_current_column;
	uint8_t m_current_row;

	static constexpr int32_t s_columns = 80;
	static constexpr int32_t s_rows = 25;
};
