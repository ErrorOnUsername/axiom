#include "k_malloc.h"

#include <AXUtil/Helpers.h>
#include <Kernel/std_lib.h>

#define K_MALLOC_POOL_SIZE (4 * MiB)
#define BITMAP_CHUNK_SIZE 32

struct AllocationHeader {
	size_t size;
};

SECTION(".heap") uint8_t k_malloc_pool[K_MALLOC_POOL_SIZE];

void k_malloc_init()
{
	memset(k_malloc_pool, 0, sizeof(k_malloc_pool));
}

//
// k_malloc:
//
// [DISCLAIMER]
// I've never written one of these before, so I'm sure that this isn't very
// good. So PLEASE future me come back and fix all this garbage so we don't
// get made fun of for writing a bad malloc implementation. :)
//
// The basic idea is to just use a bitmap to keep track of what's free in
// the heap (albeit in 32 byte chunks). So, when we find a space that will
// fit our desired size, we mark that space in the bitmap as taken.
// Additionally, we have another data stucture, tentaively named
// `AllocationHeader`, that will store the size of the data at that section
// in the heap. That way, we don't need to take a size in k_free. This is a
// huge benifit because, if we freed memory according to a user's input,
// then they could literally free the entire heap, which would obviously be
// very bad.
//
void* k_malloc(size_t size)
{
	AllocationHeader header { size };

	// FIXME: We should probably panic when we get an OOM in the kernel, but
	//        for now we're gonna make the responsible decision and just,
	//        return nullptr
	return nullptr;
}

void k_free(void*)
{ }
