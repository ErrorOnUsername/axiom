#pragma once

#include <AXUtil/Bitmap.h>
#include <AXUtil/Types.h>

namespace Kernel::Memory {

template<size_t DATA_SIZE, size_t CHUNK_SIZE, uint8_t MALLOC_SCRUB_BYTE, uint8_t FREE_SCRUB_BYTE>
class Heap {
public:
	Heap(uint8_t* heap_pool, size_t pool_size)
		: m_heap_pool(heap_pool)
		, m_pool_size(pool_size)
	{ }
private:
	uint8_t* m_heap_pool;
	size_t m_pool_size;

	AX::Bitmap m_bitmap;
};

}
