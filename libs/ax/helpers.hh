#pragma once

#include "types.hh"

#define KiB (1024)
#define MiB (KiB * 1024)
#define GiB (MiB * 1024)

#define __STRINGIFY_HELPER(x) #x
#define __STRINGIFY__(x) __STRINGIFY_HELPER(x)

#define ERROR(msg) __attribute__((error(msg)))

#define NAKED      __attribute__((naked))
#define PACKED     __attribute__((packed))
#define USED       __attribute__((used))
#define ALIGN(x)   __attribute__((aligned(x)))
#define SECTION(x) __attribute__((__section__(#x), used))

#define PAGE_SIZE 0x1000

#define ALIGN_UP(value, align_to)   ((value) + (((align_to) - ((value) % (align_to))) % (align_to)))
#define ALIGN_DOWN(value, align_to) ((value) - ((value) % (align_to)))

#define MAKE_NONCOPYABLE(Class)             \
	Class(Class const&) = delete;           \
	Class& operator=(Class const&) = delete

#define MAKE_NONMOVEABLE(Class)        \
	Class(Class&&) = delete;           \
	Class& operator=(Class&&) = delete

static constexpr u64 explode_byte(u8 byte)
{
	u64 big_byte = byte;
	return (big_byte << 56) | (big_byte << 48) |
	       (big_byte << 40) | (big_byte << 32) |
	       (big_byte << 24) | (big_byte << 16) |
		   (big_byte <<  8) | (big_byte <<  0);
}
