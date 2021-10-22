#pragma once

#include <AXUtil/malloc.h>
#include <AXUtil/Assert.h>
#include <AXUtil/Optional.h>
#include <AXUtil/Types.h>

namespace AX {

class Bitmap {
public:
	Bitmap(uint8_t* raw_data, size_t chunk_count)
		: m_raw_data(raw_data)
		, m_chunk_count(chunk_count)
	{ }

	Optional<size_t> find_first_fit(size_t chunk_count)
	{
		ASSERT(chunk_count <= m_chunk_count);

		size_t running_length = 0;

		uint8_t current_chunk_sector = 0;
		uint8_t current_chunk_bit= 0;
		for(size_t i = 0; i < m_chunk_count; i++) {
			current_chunk_sector = i / 8;
			current_chunk_bit = i % 8;

			if(m_raw_data[current_chunk_sector] & (1 << current_chunk_bit))
				running_length++;
			else
				running_length = 0;

			if(running_length == chunk_count)
				return i - (running_length - 1);

		}
		return { };
	}

	void set_range(size_t index, size_t length, bool value)
	{
		ASSERT(index <= m_chunk_count);
		ASSERT((index + length) <= m_chunk_count);
		// FIXME: Optimize this so that it doesn't just write individual bits. We
		//        should write bytes at a time as well, if we're able to.
		uint8_t current_chunk_sector = 0;
		uint8_t current_chunk_bit= 0;
		for(size_t i = index; i < index + length; i++) {
			current_chunk_sector = i / 8;
			current_chunk_bit = i % 8;

			if(value) {
				m_raw_data[current_chunk_sector] |= 1 << current_chunk_bit;
				return;
			} else {
				m_raw_data[current_chunk_sector] -= 1 << current_chunk_bit;
				return;
			}
		}
	}

	void fill(bool value)
	{
		__builtin_memset((void*)m_raw_data, (value) ? 0xff : 0x00, m_chunk_count / 8);
	}
private:
	uint8_t* m_raw_data;
	size_t m_chunk_count;
};

}
