#pragma once

#include <AXUtil/Assert.h>
#include <AXUtil/Std.h>
#include <AXUtil/Types.h>

namespace AX {

template<typename T>
class Optional {
public:
	inline Optional() = default;

	Optional(Optional const&) requires(!Extra::is_copy_constructible<T>) = delete;
	Optional(Optional const&) = default;

	Optional(Optional&&) requires(!Extra::is_move_constructible<T>) = delete;

	Optional& operator=(Optional const&) requires(!Extra::is_copy_constructible<T> || !Extra::is_destructible<T>) = delete;
	Optional& operator=(Optional const&) = default;

	Optional& operator=(Optional&&) requires(!Extra::is_move_constructible<T> || !Extra::is_destructible<T>) = delete;

	~Optional() requires(!Extra::is_destructible<T>) = delete;
	~Optional() = default;

	Optional(Optional const& other)
		requires(!Extra::is_trivially_copy_constructible<T>)
		: m_has_value(other.m_has_value)
	{
		if(other.has_value()){
			new (&m_storage) T(other.value());
		}
	}

	inline Optional(Optional&& other)
		: m_has_value(other.m_has_value)
	{
		if(other.has_value()) {
			new (&m_storage) T(other.value());
		}
	}

	template<typename U = T>
	inline explicit(!Extra::is_convertable<U&&, T>) Optional(U&& value)
		requires(!Extra::is_same<Extra::remove_cv_reference<U>, Optional<T>> && Extra::is_constructible<T, U&&>)
		: m_has_value(true)
	{
		new (&m_storage) T(forward<U>(value));
	}

	inline Optional& operator=(Optional const& other)
		requires(!Extra::is_trivially_copy_constructible<T> || !Extra::is_trivially_destructible<T>)
	{
		if(this != other) {
			clear();
			m_has_value = other.m_has_value;
			if(other.has_value()) {
				new (&m_storage) T(other.value());
			}
		}
		return *this;
	}

	inline Optional& operator=(Optional&& other)
	{
		if(this != other) {
			clear();
			m_has_value = other.m_has_value;
			if(other.has_value()) {
				new (&m_storage) T(other.release_value());
			}
		}
		return *this;
	}

	template<typename O>
	inline bool operator==(Optional<O> const& other) const
	{
		return has_value() == other.has_value() && (!has_value() || value() == other.value());
	}

	template<typename O>
	inline bool operator==(O const& other) const
	{
		return has_value() && value() == other;
	}

	inline ~Optional()
		requires(!Extra::is_trivially_destructible<T>)
	{
		clear();
	}

	inline void clear()
	{
		if(m_has_value) {
			value().~T();
			m_has_value = false;
		}
	}

	template<typename... Params>
	inline void emplace(Params&&... params)
	{
		clear();
		m_has_value = true;
		new (&m_storage) T(forward<Params>(params)...);
	}

	inline bool has_value() const { return m_has_value; }

	inline T& value() &
	{
		ASSERT(m_has_value);
		return *__builtin_lauder(reinterpret_cast<T*>(&m_storage));
	}

	inline T const& value() const&
	{
		ASSERT(m_has_value);
		return *__builtin_lauder(reinterpret_cast<T const*>(&m_storage));
	}

	inline T value() &&
	{
		return release_value();
	}

	T release_value()
	{
		ASSERT(m_has_value);
		T released_value = move(value());
		value().~T();
		m_has_value = false;
		return released_value;
	}

	inline T value_or(T const& fallback) const&
	{
		if(m_has_value)
			return value();
		return fallback;
	}

	inline T value_or(T&& fallback) &&
	{
		if(m_has_value)
			return move(value());
		return move(fallback);
	}

	inline T& operator*() { return value(); }
	inline T const& operator*() const { return value(); }

	inline T* operator->() { return &value(); }
	inline T const* operator->() const { return &value(); }
private:
	bool m_has_value = false;
	alignas(T) uint8_t m_storage[sizeof(T)];
};

}
