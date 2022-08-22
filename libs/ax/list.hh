#pragma once

#include "malloc.hh"
#include "assert.hh"
#include "types.hh"

namespace AX {
template<typename T>
struct List {
	T*     storage;
	size_t capacity;
	size_t count;

	List(size_t initial_capacity)
		: storage(nullptr)
		, capacity(0)
		, count(initial_capacity)
	{
		ensure_capacity(initial_capacity);
	}

	List()
		: storage(nullptr)
		, capacity(0)
		, count(0)
	{
		ensure_capacity(capacity);
	}

	List(List const& other)
		: storage(other.storage)
		, capacity(other.capacity)
		, count(other.count)
	{ }

	~List()
	{
		free(storage);
	}

	T& at(size_t index)
	{
		ASSERT(index < count);

		return storage[index];
	}

	T const& at(size_t index) const
	{
		ASSERT(index < count);

		return storage[index];
	}

	T& last()
	{
		ASSERT(count > 0);

		return storage[count - 1];
	}

	T const& last() const
	{
		ASSERT(count > 0);

		return storage[count - 1];
	}

	T& operator[](size_t index) { return at(index); }
	T const& operator[](size_t index) const { return at(index); }

	void set(size_t index, T const& element)
	{
		ASSERT(index < count);

		storage[index] = element;
	}

	void append(T const& element)
	{
		if(count == capacity)
			ensure_capacity((capacity == 0) ? 1 : capacity * 2);

		storage[count] = element;
		count++;
	}

	void insert(size_t index, T const& element)
	{
		bool result = try_insert(index, element);
		ASSERT_MSG(result, "Could not insert element at index: %us!!!", index);
	}

	bool try_insert(size_t index, T const& element)
	{
		if(index > count)
			return false;

		if(count == capacity) {
			if(!try_ensure_capacity((capacity == 0) ? 1 : capacity * 2))
				return false;
		}

		T current = element;
		T temp;
		for(size_t i = index; i <= count; i++) {
			temp = storage[i];
			storage[i] = current;
			current = temp;
		}

		count++;

		return true;
	}

	void remove(size_t index)
	{
		bool result = try_remove(index);
		ASSERT_MSG(result, "Could not remove element at index: %us!!!\n", index);
	}

	bool try_remove(size_t index)
	{
		// FIXME: IMPLEMENT THIS RIGHT!!!!
		if(index > count - 1)
			return false;

		if(storage[index])
			storage[index].~T();

		count--;
		for(size_t i = 0; i < count; i++) {
			if(i > index)
				storage[i - 1] = storage[i];
		}
		return true;
	}

	bool is_empty() const
	{
		return count == 0;
	}

	void ensure_capacity(size_t new_capacity)
	{
		bool result = try_ensure_capacity(new_capacity);
		ASSERT_MSG(result, "Could not go from length %us to %us!!!\n", capacity, new_capacity);
	}

	bool try_ensure_capacity(size_t new_capacity)
	{
		// Take care of cheap cases
		if(new_capacity == capacity)
			return true;

		if(storage && new_capacity == 0) {
			free(storage);

			storage = nullptr;
			capacity = 0;

			return true;
		}

		T* new_storage = (T*)malloc(new_capacity * sizeof(T));
		if(!new_storage)
			return false;

		for(size_t i = 0; i < new_capacity; i++) {
			if(storage && i < count) {
				new_storage[i] = storage[i];
				storage[i].~T();
			} else {
				new_storage[i] = T();
			}
		}

		if(storage)
			free(storage);

		capacity = new_capacity;
		storage  = new_storage;
		return true;
	}
};

}
