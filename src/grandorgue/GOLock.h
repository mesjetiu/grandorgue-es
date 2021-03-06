/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2015 GrandOrgue contributors (see AUTHORS)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef GOLOCK_H
#define GOLOCK_H

#include "GrandOrgueDef.h"
#include <wx/log.h>
#include <wx/stackwalk.h>
#include <wx/string.h>
#include <wx/thread.h>
#include <vector>

#if 0  /* set to 1 to enable fallback code */
#undef HAVE_CSTDATOMIC
#undef HAVE_ATOMIC
#endif

#ifdef HAVE_ATOMIC
#include <atomic>
#endif
#ifdef HAVE_CSTDATOMIC
#include <cstdatomic>
#define HAVE_ATOMIC
#endif

#define GO_PRINTCONTENTION 0

#if wxUSE_STACKWALKER
class GOStackPrinter : public wxStackWalker
{
	void* m_ptr;
public:	
	GOStackPrinter(void* p)
	{
		m_ptr = p;
	}

	void OnStackFrame(const wxStackFrame& frame)
	{
		wxLogWarning(wxT("%p: [%2d] %s(%p)\t%s:%d"), m_ptr, frame.GetLevel(), frame.GetName().c_str(), frame.GetAddress(), frame.GetFileName().c_str(), frame.GetLine());
	}
};
#else
#undef GO_PRINTCONTENTION
#define GO_PRINTCONTENTION 0
#endif

#include <mutex>

#if 1
//#ifdef __WIN32__
class GOWaitQueue
{
private:
	wxSemaphore m_Wait;
public:
	GOWaitQueue()
	{
	}

	~GOWaitQueue()
	{
	}

	void Wait()
	{
		m_Wait.Wait();
	}

	void Wakeup()
	{
		m_Wait.Post();
	}
};
#else
#ifdef HAVE_MUTEX
class GOWaitQueue
{
private:
	std::mutex m_Wait;
public:
	GOWaitQueue()
	{
		m_Wait.lock();
	}

	~GOWaitQueue()
	{
		m_Wait.unlock();
	}

	void Wait()
	{
		m_Wait.lock();
	}

	void Wakeup()
	{
		m_Wait.unlock();
	}
};
#else
class GOWaitQueue
{
private:
	wxMutex m_Wait;
public:
	GOWaitQueue()
	{
		m_Wait.Lock();
	}

	~GOWaitQueue()
	{
		m_Wait.Unlock();
	}

	void Wait()
	{
		wxMutexError error = m_Wait.Lock();
		assert(error == wxMUTEX_NO_ERROR);
		(void)error;
	}

	void Wakeup()
	{
		m_Wait.Unlock();
	}
};
#endif
#endif

class GOMutex
{
private:
#ifndef HAVE_ATOMIC
	wxMutex m_Mutex;

	void Init()
	{
	}

	void Cleanup()
	{
	}

	void DoLock()
	{
		m_Mutex.Lock();
	}

	void DoUnlock()
	{
		m_Mutex.Unlock();
	}

	bool DoTryLock()
	{
		return m_Mutex.TryLock() == wxMUTEX_NO_ERROR;
	}
	friend class GOCondition;
#else
	GOWaitQueue m_Wait;
	std::atomic_int m_Lock;

	void Init()
	{
		m_Lock = 0;
	}

	void Cleanup()
	{
	}

	void DoLock()
	{
		int value = m_Lock.fetch_add(1);
		if (!value)
		{
			__sync_synchronize();
			return;
		}
#if GO_PRINTCONTENTION
		wxLogWarning(wxT("Mutex::wait %p"), this);
		GOStackPrinter print(this);
		print.Walk();
#endif

		m_Wait.Wait();
#if GO_PRINTCONTENTION
		wxLogWarning(wxT("Mutex::end_wait %p"), this);
#endif
	}

	void DoUnlock()
	{
		__sync_synchronize();
		int value = m_Lock.fetch_add(-1);
		if (value > 1)
			m_Wait.Wakeup();
	}

	bool DoTryLock()
	{
		int old = 0;
		if (m_Lock.compare_exchange_strong(old, 1))
		{
			__sync_synchronize();
			return true;
		}
		return false;
	}
#endif

	GOMutex(const GOMutex&);
	const GOMutex& operator=(const GOMutex&);

public:
	GOMutex()
	{
		Init();
	}

	~GOMutex()
	{
		Cleanup();
	}

	void Lock()
	{
		DoLock();
	}

	void Unlock()
	{
		DoUnlock();
	}

	bool TryLock()
	{
		return DoTryLock();
	}
};

class GOCondition
{
private:
#ifndef HAVE_ATOMIC
	wxCondition m_condition;

	void Init()
	{
	}

	void Cleanup()
	{
	}

	void DoWait()
	{
		m_condition.Wait();
	}

	void DoSignal()
	{
		m_condition.Signal();
	}

	void DoBroadcast()
	{
		m_condition.Broadcast();
	}
#else
	std::atomic_int m_Waiters;
	GOWaitQueue m_Wait;

	void Init()
	{
		m_Waiters = 0;
	}

	void Cleanup()
	{
		while(m_Waiters > 0)
			Signal();
	}

	void DoWait()
	{
		m_Waiters.fetch_add(1);
		m_Mutex.Unlock();
		m_Wait.Wait();
		m_Mutex.Lock();
	}

	void DoSignal()
	{
		int waiters = m_Waiters.fetch_add(-1);
		if (waiters <= 0)
			m_Waiters.fetch_add(+1);
		else
			m_Wait.Wakeup();
	}

	void DoBroadcast()
	{
		int waiters;
		do
		{
			waiters = m_Waiters.fetch_add(-1);
			if (waiters <= 0)
			{
				m_Waiters.fetch_add(+1);
				return;
			}
			else
				m_Wait.Wakeup();
		}
		while(waiters > 1);
	}

#endif
	GOMutex& m_Mutex;

	GOCondition(const GOCondition&);
	const GOCondition& operator=(const GOCondition&);
public:
	GOCondition(GOMutex& mutex) :
#ifndef HAVE_ATOMIC
		m_condition(mutex.m_Mutex),
#endif
		m_Mutex(mutex)
	{
		Init();
	}

	~GOCondition()
	{
		Cleanup();
	}

	void Wait()
	{
		DoWait();
	}

	void Signal()
	{
		DoSignal();
	}

	void Broadcast()
	{
		DoBroadcast();
	}
};

class GOMutexLocker
{
private:
	GOMutex& m_Mutex;
	bool m_Locked;
public:
	GOMutexLocker(GOMutex& mutex, bool try_lock = false) :
		m_Mutex(mutex),
		m_Locked(false)
	{
		if (try_lock)
			m_Locked = m_Mutex.TryLock();
		else
		{
			m_Mutex.Lock();
			m_Locked = true;
		}
	}

	~GOMutexLocker()
	{
		if (m_Locked)
			m_Mutex.Unlock();
	}

	bool TryLock()
	{
		if (!m_Locked)
			m_Locked = m_Mutex.TryLock();
		return m_Locked;
	}

	bool IsLocked() const
	{
		return m_Locked;
	}

	void Unlock()
	{
		if (m_Locked)
			m_Mutex.Unlock();
		m_Locked = false;
	}
};

class GOMultiMutexLocker
{
private:
	std::vector<GOMutex*> m_Mutex;

public:
	GOMultiMutexLocker() :
		m_Mutex(0)
	{
	}

	~GOMultiMutexLocker()
	{
		while(m_Mutex.size())
		{
			m_Mutex.back()->Unlock();
			m_Mutex.pop_back();
		}
	}

	void Add(GOMutex& mutex)
	{
		mutex.Lock();
		m_Mutex.push_back(&mutex);
	}
};

#ifndef HAVE_ATOMIC
namespace std {
	template<class T>
	class atomic
	{
	private:
		wxCriticalSection m_Lock;
		T m_Value;
	public:
		atomic()
		{
		}
	
		atomic(const T& val)
		{
			m_Value = val;
		}

		atomic(const atomic<T>& val)
		{
			m_Value = val.m_Value;
		}

		T operator=(const T& val)
		{
			wxCriticalSectionLocker m_Locker(m_Lock);
			m_Value = val;
			return val;
		}

		operator T() const
		{
			return m_Value;
		}

		T exchange(const T& val)
		{
			wxCriticalSectionLocker m_Locker(m_Lock);
			T current = m_Value;
			m_Value = val;
			return current;
		}

		bool compare_exchange_strong(T& expected, T new_value)
		{
			wxCriticalSectionLocker m_Locker(m_Lock);

			if (m_Value == expected)
			{
				m_Value = new_value;
				return true;
			}
			else
			{
				expected = m_Value;
				return false;
			}
		}

		T fetch_add(const T& value)
		{
			wxCriticalSectionLocker m_Locker(m_Lock);
			T current = m_Value;
			m_Value += value;
			return current;
		}

	};

	typedef atomic<unsigned> atomic_uint;
}
#endif

#endif
