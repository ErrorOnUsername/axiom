#pragma once

#include <AXUtil/Assert.h>
#include <AXUtil/Bitmap.h>
#include <AXUtil/Types.h>

namespace Kernel::Memory {

// TODO: Maybe add some sort of param that can wipe whatever was on the
//       heap previously with some other value. It would be a performance
//       hit, but it would be good for security if we need it.
template<size_t DATA_SIZE, size_t CHUNK_SIZE>
class Heap {
	struct AllocationHeader {
		size_t allocation_size_in_bytes;
	};

	static size_t chunks_needed(size_t size)
	{
		// Round up the size to chunk boundaries so that we can fit all the data.
		if(size % CHUNK_SIZE != 0)
			return (size + (CHUNK_SIZE - (size % CHUNK_SIZE))) / CHUNK_SIZE;
		else
			return size / CHUNK_SIZE;
	}

	static AllocationHeader const* allocation_header_at_addr(void* addr)
	{
		return (AllocationHeader const*)(((uint8_t*)addr) - sizeof(AllocationHeader));
	}
public:
	Heap(uint8_t* heap_pool)
		: m_heap_pool(heap_pool)
		, m_pool_size(DATA_SIZE)
		, m_bitmap(&m_raw_bitmap[0], TOTAL_CHUNKS)
	{ }

	void* allocate(size_t bytes)
	{
		bytes += sizeof(AllocationHeader);
		ASSERT(bytes < m_pool_size);

		size_t chunks = chunks_needed(bytes);
		if(chunks > available_chunks())
			return nullptr;

		// FIXME: Don't always look for a perfectly-sized opening! We should
		//        have a threshold where we just look for a chunk slopily and
		//        find one that's at least big enough.
		int64_t chunk_index = m_bitmap.find_first_fit(chunks);
		if(chunk_index == -1)
			return nullptr;

		AllocationHeader* header = (AllocationHeader*)(m_heap_pool + chunk_index * CHUNK_SIZE);
		header->allocation_size_in_bytes = chunks * CHUNK_SIZE;

		m_bitmap.set_range(chunk_index, chunks, true);
		m_allocated_chunks += chunks;

		// TODO: Scrub out the buffer before returning the pointer?
		//       It could be good for security, but slow.
		return (void*)((uint64_t)header + sizeof(AllocationHeader));
	}

	void free(void* data)
	{
		if(!data)
			return;
		AllocationHeader const* alloc_header = allocation_header_at_addr(data);
		ASSERT((uint8_t*)alloc_header >= m_heap_pool && (uint8_t*)alloc_header < m_heap_pool + TOTAL_CHUNKS * CHUNK_SIZE);

		size_t total_chunks = alloc_header->allocation_size_in_bytes / CHUNK_SIZE;
		size_t start_chunk_index = ((uint64_t)alloc_header - (uint64_t)m_heap_pool) / CHUNK_SIZE;

		ASSERT(m_bitmap.verify(start_chunk_index));
		m_bitmap.set_range(start_chunk_index, total_chunks, false);

		// TODO: Scrub out the buffer after it's freed?
	}

	[[nodiscard]] inline size_t available_chunks() const { return TOTAL_CHUNKS - m_allocated_chunks; }
private:
	static constexpr size_t TOTAL_CHUNKS = (DATA_SIZE + (DATA_SIZE % CHUNK_SIZE)) / CHUNK_SIZE;

	uint8_t* m_heap_pool;
	size_t m_pool_size;

	AX::Bitmap m_bitmap;
	uint8_t m_raw_bitmap[TOTAL_CHUNKS];
	size_t m_allocated_chunks;
};

}
