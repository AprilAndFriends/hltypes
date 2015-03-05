/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef _WIN32
#include <pthread.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "hlog.h"
#include "hltypesUtil.h"
#include "hmutex.h"
#include "hplatform.h"
#include "hthread.h"

namespace hltypes
{
	Mutex::ScopeLock::ScopeLock(Mutex* mutex, bool logUnhandledUnlocks) : mutex(NULL)
	{
		this->logUnhandledUnlocks = logUnhandledUnlocks;
		this->acquire(mutex);
	}

	Mutex::ScopeLock::~ScopeLock()
	{
		Mutex* mutex = this->mutex;
		if (this->release() && this->logUnhandledUnlocks && mutex != NULL)
		{
#ifdef _WIN32
			String address = hsprintf("<0x%p>", this);
#else
			String address = hsprintf("<%p>", this); // on Unix %p adds the 0x
#endif
			Log::warnf("hmutex", "'%s' has been scope-unlocked automatically!", (mutex->name != "" ? mutex->name : address).cStr());
		}
	}

	bool Mutex::ScopeLock::acquire(Mutex* mutex)
	{
		if (this->mutex == NULL && mutex != NULL)
		{
			this->mutex = mutex;
			this->mutex->lock();
			return true;
		}
		return false;
	}

	bool Mutex::ScopeLock::release()
	{
		if (this->mutex != NULL)
		{
			this->mutex->unlock();
			this->mutex = NULL;
			return true;
		}
		return false;
	}

	Mutex::Mutex(const String& name) : handle(NULL), locked(false)
	{
		this->name = name;
#ifdef _WIN32
		this->handle = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));
#ifndef _WINRT // WinXP does not have InitializeCriticalSectionEx()
		InitializeCriticalSection((CRITICAL_SECTION*)this->handle);
#elif !defined(_DEBUG)
		InitializeCriticalSectionEx((CRITICAL_SECTION*)this->handle, 0, 0);
#else
		InitializeCriticalSectionEx((CRITICAL_SECTION*)this->handle, 0, CRITICAL_SECTION_NO_DEBUG_INFO);
#endif
#else
		this->handle = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((pthread_mutex_t*)this->handle, 0);
#endif
	}

	Mutex::~Mutex()
	{
#ifdef _WIN32
		DeleteCriticalSection((CRITICAL_SECTION*)this->handle);
		free((CRITICAL_SECTION*)this->handle);
#else
		pthread_mutex_destroy((pthread_mutex_t*)this->handle);
		free((pthread_mutex_t*)this->handle);
#endif
	}

	void Mutex::lock()
	{
#ifdef _WIN32
		EnterCriticalSection((CRITICAL_SECTION*)this->handle);
		if (this->locked)
		{
			hltypes::_platformPrint("hmutex", hsprintf("'%s' is deadlocked!", (this->name != "" ? this->name : hsprintf("<0x%p>", this)).cStr()), 1000);
			while (true) // simulating a deadlock
			{
				Thread::sleep(1.0f);
			}
		}
		this->locked = true;
#else
		pthread_mutex_lock((pthread_mutex_t*)this->handle);
#endif
	}

	void Mutex::unlock()
	{
#ifdef _WIN32
		this->locked = false;
		LeaveCriticalSection((CRITICAL_SECTION*)this->handle);
#else
		pthread_mutex_unlock((pthread_mutex_t*)this->handle);
#endif
	}
	
	Mutex::Mutex(const Mutex& other)
	{
		throw ObjectCannotCopyException("hltypes::Mutex");
	}

	Mutex& Mutex::operator=(Mutex& other)
	{
		throw ObjectCannotAssignException("hltypes::Mutex");
		return (*this);
	}

}
