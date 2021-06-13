#include "VGATextModeDevice.h"

#include <AXUtil/types.h>

VGATextModeDevice::VGATextModeDevice() { }

void VGATextModeDevice::clear_screen() {
	for(uint32_t i = 0; i < 2 * s_rows * s_columns; i++) {
		m_text_buffer[i] = 0;
	}
}

void VGATextModeDevice::write_line(const char* msg, uint8_t attribute) {
	char c = msg[0];
	int32_t idx = 0;

	while(c != '\0') {
		if(c == '\n') {
			m_current_column = 0;
			// TODO: Check bounds and implement scrolling!!
			m_current_row++;
			goto next;
		} else if (c == '\r') {
			goto next;
		} else if (c == '\t') {
			m_current_column += 2;
			goto next;
		}

		set_character(m_current_column, m_current_row, c, attribute);

next:
		c = msg[++idx];
	}

	m_current_column = 0;
	// TODO: Check bounds and implement scrolling!!
	m_current_row++;
}

void VGATextModeDevice::set_character(uint8_t column, uint8_t row, char c, uint8_t attribute) {
	m_text_buffer[(2*column) + (row * s_rows)] = c;
	m_text_buffer[(2*column+1) + (row * s_rows)] = attribute;
	m_current_column++;
}
