#pragma once

#include "malloc.hh"
#include "assert.hh"
#include "types.hh"

namespace AX {

struct Bitmap {
	u8*    raw_data;
	size_t chunk_count;
	bool   initialized;

	Bitmap(size_t num_chunks = 0)
		: raw_data(nullptr)
		, chunk_count(num_chunks)
		, initialized(false)
	{
		if(chunk_count != 0) {
			size_t bitmap_size_in_bytes = ALIGN_UP(chunk_count, 8) / 8;

			raw_data = (u8*)malloc(bitmap_size_in_bytes);
			initialized = true;
		}
	}

	Bitmap(u8* data_ptr, size_t num_chunks)
		: raw_data(data_ptr)
		, chunk_count(num_chunks)
		, initialized(true)
	{ }

	void init(size_t num_chunks)
	{
		if(!initialized) {
			size_t bitmap_size_in_bytes = ALIGN_UP(num_chunks, 8) / 8;
			chunk_count = num_chunks;
			raw_data = (u8*)malloc(bitmap_size_in_bytes);
			initialized = true;
		}
	}

	i64 find_first_fit(size_t num_chunks)
	{
		ASSERT(num_chunks <= chunk_count);

		size_t running_length       = 0;
		u8     current_chunk_sector = 0;
		u8     current_chunk_bit    = 0;

		for(size_t i = 0; i < chunk_count; i++) {
			current_chunk_sector = i / 8;
			current_chunk_bit    = 7 - (i % 8);

			if(!(raw_data[current_chunk_sector] & (1 << current_chunk_bit)))
				running_length++;
			else
				running_length = 0;

			if(running_length == num_chunks)
				return i - (running_length - 1);
		}

		// This should be fine. I'm sure that we wont need an entire u64 to hold
		// the index into this bitmap. I really just don't want to implement
		// `Optional` because template garbage is really ugly. Alternatively, I
		// could make a struct with a boolean and a union, so if we need a full
		// size_t, I'll do that.
		return -1;
	}

	void set_range(size_t index, size_t length, bool occupied)
	{
		ASSERT(index <= chunk_count);
		ASSERT((index + length) <= chunk_count);

		// FIXME: Optimize this so that it doesn't just write individual bits. We
		//        should write bytes at a time as well, if we're able to.
		u8 current_chunk_sector = 0;
		u8 current_chunk_bit    = 0;

		for(size_t i = index; i < (index + length); i++) {
			current_chunk_sector = i / 8;
			current_chunk_bit    = 7 - (i % 8);

			if(occupied) {
				raw_data[current_chunk_sector] |= 1 << current_chunk_bit;
			} else {
				raw_data[current_chunk_sector] -= 1 << current_chunk_bit;
			}
		}
	}

	void fill(u8 value)
	{
		__builtin_memset((void*)raw_data, value, chunk_count / 8);
	}

	bool verify_range_occupied(size_t chunk_index, size_t length)
	{
		// FIXME: Do this intelligently! Not this stupid way.
		for(size_t i = chunk_index; i < (chunk_index + length); i++) {
			if(!verify_occupied(i))
				return false;
		}
		return true;
	}

	inline bool verify_occupied(size_t chunk_index)
	{
		u8 chunk_sector        = raw_data[chunk_index / 8];
		u8 chunk_sector_offset = 7 - (chunk_index % 8);

		return chunk_sector & (1 << chunk_sector_offset);
	}
};

}
