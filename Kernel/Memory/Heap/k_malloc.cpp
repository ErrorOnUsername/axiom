#include "k_malloc.h"

#include <AXUtil/Helpers.h>
#include <AXUtil/Lock.h>
#include <kernel/k_debug.h>
#include <kernel/std.h>
#include <kernel/arch/CPU.h>
#include <kernel/memory/heap/heap.h>

#define K_MALLOC_POOL_SIZE (4 * MiB)
#define BITMAP_CHUNK_SIZE 32

using KHeap = Kernel::Memory::Heap<K_MALLOC_POOL_SIZE, BITMAP_CHUNK_SIZE>;

static AX::Lock s_heap_lock;

static KHeap* k_malloc_heap_ptr;
SECTION(.kernel_heap) static uint8_t k_malloc_pool[K_MALLOC_POOL_SIZE];
alignas(KHeap) static uint8_t g_kmalloc_heap_raw[sizeof(KHeap)];

void k_malloc_init()
{
	memset(k_malloc_pool, 0, sizeof(k_malloc_pool));
	k_malloc_heap_ptr = new (g_kmalloc_heap_raw) KHeap(k_malloc_pool);
}

//
// k_malloc:
//
// DISCLAIMER:
// I've never written one of these before, so I'm sure that this isn't very
// good. So PLEASE future me come back and fix all this garbage, so we don't
// get made fun of for writing a bad malloc implementation. :)
//
// The basic idea is to just use a bitmap to keep track of what's free in
// the heap (albeit in 32 byte chunks). So, when we find a space that will
// fit our desired size, we mark that space in the bitmap as taken.
// Additionally, we have another data structure, tentatively named
// `AllocationHeader`, that will store the size of the data at that section
// in the heap. That way, we don't need to take a size in k_free. This is a
// huge benefit because, if we freed memory according to a user's input,
// then they could literally free the entire heap, which would obviously be
// very bad.
//
void* k_malloc(size_t bytes)
{
	AX::ScopeLocker locker(s_heap_lock);

	void* ptr = k_malloc_heap_ptr->allocate(bytes);

	return ptr;
}

void k_free(void* ptr)
{
	AX::ScopeLocker locker(s_heap_lock);

	k_malloc_heap_ptr->free(ptr);
}

void* operator new(size_t bytes)
{
	void* ptr = k_malloc(bytes);
	ASSERT(ptr);
	return ptr;
}
void* operator new(size_t bytes, std::nothrow_t const &) noexcept
{
	return k_malloc(bytes);
}
void* operator new(size_t, std::align_val_t);
void* operator new(size_t, std::align_val_t, std::nothrow_t const&) noexcept;
void* operator new[](size_t size)
{
	void* ptr = k_malloc(size);
	ASSERT_MSG(ptr, "OOM in k_malloc!!!!");
	return ptr;
}
void* operator new[](size_t size, std::nothrow_t const&) noexcept
{
	return k_malloc(size);
}

void operator delete(void* ptr, size_t) noexcept
{
	// We don't need the size parameter because we have allocation headers :)
	k_free(ptr);
}
void operator delete(void*, size_t, std::align_val_t) noexcept;
void operator delete[](void* ptr, size_t) noexcept
{
	k_free(ptr);
}
