#pragma once

#include <ax_util/malloc.hh>
#include <ax_util/assert.hh>
#include <ax_util/types.hh>

namespace AX {

class Bitmap {
public:
	Bitmap(uint8_t* raw_data, size_t chunk_count)
		: m_raw_data(raw_data)
		, m_chunk_count(chunk_count)
	{ }

	int64_t find_first_fit(size_t chunk_count)
	{
		ASSERT(chunk_count <= m_chunk_count);

		size_t running_length = 0;
		uint8_t current_chunk_sector = 0;
		uint8_t current_chunk_bit= 0;

		for(size_t i = 0; i < m_chunk_count; i++) {
			current_chunk_sector = i / 8;
			current_chunk_bit = 7 - (i % 8);

			if(!(m_raw_data[current_chunk_sector] & (1 << current_chunk_bit)))
				running_length++;
			else
				running_length = 0;

			if(running_length == chunk_count)
				return i - (running_length - 1);
		}

		// This should be fine. I'm sure that we wont need an entire u64 to hold
		// the index into this bitmap. I really just don't want to implement
		// `Optional` because template garbage is really ugly. Alternatively, I
		// could make a struct with a boolean and a union, so if we need a full
		// size_t, I'll do that.
		return -1;
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
			current_chunk_bit = 7 - (i % 8);

			if(value) {
				m_raw_data[current_chunk_sector] |= 1 << current_chunk_bit;
			} else {
				m_raw_data[current_chunk_sector] -= 1 << current_chunk_bit;
			}
		}
	}

	void fill(bool value)
	{
		__builtin_memset((void*)m_raw_data, (value) ? 0xff : 0x00, m_chunk_count / 8);
	}

	inline bool verify(size_t chunk_index)
	{
		uint8_t chunk_sector = m_raw_data[chunk_index / 8];
		uint8_t chunk_sector_offset = 7 - (chunk_index % 8);

		return chunk_sector & (1 << chunk_sector_offset);
	}
private:
	uint8_t* m_raw_data;
	size_t m_chunk_count;
};

}
