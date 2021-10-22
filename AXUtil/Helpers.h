#pragma once

#include <AXUtil/Types.h>

#define KiB (1024)
#define MiB (KiB * 1024)
#define GiB (MiB * 1024)

#define __STRINGIFY_HELPER(x) #x
#define __STRINGIFY__(x) __STRINGIFY_HELPER(x)

#define ERROR(msg) __attribute__((error(msg)))

#define NAKED __attribute__((naked))
#define PACKED __attribute__((packed))
#define USED __attribute__((used))
#define ALIGN(x) __attribute__((aligned(x)))
#define SECTION(x) __attribute__((__section__(#x), used))

#define PAGE_SIZE 0x1000
#define ADDR_PAGE_ALIGN_UP(addr) addr + (PAGE_SIZE - (addr % PAGE_SIZE))
#define ADDR_PAGE_ALIGN_DOWN(addr) addr - (addr % PAGE_SIZE)

#define MAKE_NONCOPYABLE(Class)             \
	Class(Class const&) = delete;           \
	Class& operator=(Class const&) = delete

#define MAKE_NONMOVEABLE(Class)        \
	Class(Class&&) = delete;           \
	Class& operator=(Class&&) = delete

static constexpr uint64_t exlode_byte(uint8_t b)
{
	uint64_t val = b;
	return ((val << 56) | (val << 48) | (val << 40) | (val << 32) | (val << 24) | (val << 16) | (val << 8) | val);
}
