#pragma once

#include <AXUtil/Bitmap.h>
#include <AXUtil/Helpers.h>
#include <AXUtil/Types.h>

namespace Kernel::Memory {

template<size_t CHUNK_SIZE, uint8_t MALLOC_CLEAR_BYTE, uint8_t FREE_CLEAR_BYTE>
class Heap {
	MAKE_NONCOPYABLE(Heap);

	struct AllocationHeader {
		size_t size;
	};
public:
	Heap(uint8_t* data, size_t size)
		: m_data(data)
		, m_size(size - (size % CHUNK_SIZE))
		, m_bitmap(data + 
	{ }

	void* allocate(size_t)
	{ }

	void free(void*)
	{ }

private:
	static 

	AX::Bitmap m_bitmap;

	uint8_t* m_data;
	size_t m_size;
};

}
