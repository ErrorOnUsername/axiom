#pragma once

#include "helpers.hh"

namespace AX {

class Lock {
	MAKE_NONMOVEABLE(Lock);
	MAKE_NONCOPYABLE(Lock);

	enum LockState : bool {
		LockLocked   = true,
		LockUnlocked = false,
	};
public:
	Lock()
		: m_raw(LockUnlocked)
	{ }

	void lock()
	{
		while (!__sync_bool_compare_and_swap(&m_raw, LockUnlocked, LockLocked)) {
			asm volatile("pause"); // try to not burn the cpu
		}
		__sync_synchronize();
	}

	void unlock()
	{
		__sync_synchronize();
		__atomic_store_n(&m_raw, LockUnlocked, __ATOMIC_SEQ_CST);

		m_raw = LockUnlocked;
	}

	void force_unlock()
	{
		m_raw = LockUnlocked;
		__sync_synchronize();
	}

	bool is_locked() const
	{
		bool result = false;
		__atomic_load(&m_raw, &result, __ATOMIC_SEQ_CST);
		return result;
	}
private:
	bool m_raw;
};

class ScopeLocker {
public:
	ScopeLocker(Lock& lock)
		: m_lock(lock)
	{
		m_lock.lock();
	}

	~ScopeLocker()
	{
		m_lock.unlock();
	}
private:
	Lock& m_lock;
};

}
