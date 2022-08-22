#pragma once

#include "types.hh"
#include "malloc.hh"

namespace AX {

template<typename T, size_t DEFAULT_SIZE = 0>
struct Queue {
	T*     raw_data;
	size_t capacity;
	size_t count;

	Queue()
		: capacity(DEFAULT_SIZE)
		, count(0)
	{
		if constexpr (DEFAULT_SIZE)
			raw_data = (T*)malloc(DEFAULT_SIZE * sizeof(T));
	}

	void push(T const& element)
	{
		if(count + 1 > capacity) {
			// FIXME: Doubling the capacity might not be a good idea in the long term
			ensure_capacity(capacity * 2);
		}

		raw_data[count++] = element;
	}

	T pop()
	{
		T return_value = raw_data[0];

		T previous_value = raw_data[count - 1];
		for(size_t i = count - 2; i >= 0; i--) {
			if(i > 0) {
				// don't take the copy if were at the beginning
				previous_value = raw_data[i];
			}

			raw_data[i + 1].~T(); // make sure your destructor is public

			raw_data[i] = previous_value;
		}

		count--;

		return return_value;
	}

	bool is_empty() const
	{
		return count == 0;
	}

	void ensure_capacity(size_t desired_capacity)
	{
		auto* new_data = (T*)malloc(desired_capacity * sizeof(T));
		for(size_t i = 0; i < ((capacity > desired_capacity) ? capacity : desired_capacity); i++) {
			if(i < ((capacity < desired_capacity) ? capacity : desired_capacity))
				new_data[i] = raw_data[i];
			raw_data[i].~T(); // make sure your destructor is public
		}

		capacity = desired_capacity;
		count    = (capacity < count) ? capacity : count;
	}
};

}
