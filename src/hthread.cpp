/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#include <pthread.h>
#endif

#define __HL_INCLUDE_PLATFORM_HEADERS
#include "harray.h"
#include "hlog.h"
#include "hltypesUtil.h"
#include "hmutex.h"
#include "hplatform.h"
#include "hthread.h"
#include "platform_internal.h"

#ifdef _UWP
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
#endif

namespace hltypes
{
#if defined(_WIN32) && defined(_MSC_VER)
#pragma pack(push, 8)
	typedef struct _THREADNAME_INFO
	{
		DWORD dwType;
		LPCSTR szName;
		DWORD dwThreadID;
		DWORD dwFlags;
	} THREADNAME_INFO;
#pragma pack(pop)

	static void SetThreadName(DWORD id, const String& name)
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = (char*)name.cStr();
		info.dwThreadID = id;
		info.dwFlags = 0;
		__try
		{
			RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}
#endif

#ifdef _WIN32
	static unsigned long WINAPI _asyncCall(void* param)
	{
		Thread::ThreadRunner* t = (Thread::ThreadRunner*)param;
#ifdef _MSC_VER
		hstr name = t->getThread()->getName();
		if (name != "")
		{
			SetThreadName(GetCurrentThreadId(), name);
		}
#endif
		t->execute();
		return 0;
	}
#else
	static void* _asyncCall(void* param)
	{
		Thread::ThreadRunner* t = (Thread::ThreadRunner*)param;
#ifdef __APPLE__
		hstr name = t->getThread()->getName();
		if (name != "")
		{
			pthread_setname_np(name.cStr());
		}
#endif
		t->execute();
		pthread_exit(NULL);
		return NULL;
	}
#endif

#ifdef _UWP
	struct AsyncActionWrapper
	{
	public:
		IAsyncAction^ asyncAction;
		AsyncActionWrapper(IAsyncAction^ asyncAction)
		{
			this->asyncAction = asyncAction;
		}
	};
#endif

	Thread::ThreadRunner::ThreadRunner(Thread* thread)
	{
		this->thread = thread;
	}

	Thread::ThreadRunner::~ThreadRunner()
	{
	}

	void Thread::ThreadRunner::execute()
	{
		this->thread->_execute();
	}

	Thread::Thread(void (*function)(Thread*), const String& name) :
		executing(false),
		runner(this),
		id(0),
		running(false)
	{
		this->function = function;
		this->name = name;
	}

	Thread::~Thread()
	{
		if (this->running)
		{
			Log::warn(logTag, "Thread still executing in destructor! Attempting 'stop', but this may be unsafe. The thread should be joined before deleting it.");
			this->stop();
		}
		this->_clear();
	}

	void Thread::start()
	{
		if (this->running)
		{
			Log::errorf(logTag, "Thread '%s' already running, cannot start!", this->name.cStr());
			return;
		}
		this->running = true;
		this->_clear(); // if thread exited on its own, but the data is still allocated
		this->_platformStart();
	}

	void Thread::join()
	{
		this->running = false;
		this->_platformJoin();
		this->_clear();
		// aborted execution can leave this variable in any state, reset it
		this->executing = false;
	}

	void Thread::resume()
	{
		this->_platformResume();
	}

	void Thread::pause()
	{
		this->_platformPause();
	}

	void Thread::stop()
	{
		if (this->running)
		{
			this->running = false;
			this->_platformStop();
			this->_clear();
			// aborted execution can leave this variable in any state, reset it
			this->executing = false;
		}
		else
		{
			this->_clear();
		}
	}

	void Thread::_execute()
	{
		if (this->running && this->function != NULL)
		{
			this->executing = true;
			try
			{
				(*this->function)(this);
			}
			catch (hltypes::_Exception& e)
			{
				hltypes::_platformPrint("FATAL", "Thread: " + this->name + "\n" + e.getMessage() + "\nStack:\n" + e.getStackTrace(), Log::LevelError);
				throw e;
			}
		}
		this->executing = false;
		this->running = false;
	}

	void Thread::_clear()
	{
		if (this->id != NULL)
		{
#ifdef _WIN32
#ifndef _UWP
			CloseHandle(this->id);
#else
			delete this->id;
#endif
#else
			free((pthread_t*)this->id);
#endif
			this->id = NULL;
		}
	}

	void Thread::_platformStart()
	{
#ifdef _WIN32
#ifndef _UWP
		this->id = CreateThread(0, 0, &_asyncCall, &this->runner, 0, 0);
#else
		this->id = new AsyncActionWrapper(ThreadPool::RunAsync(ref new WorkItemHandler([&](IAsyncAction^ workItem)
		{
			if (this->name != "")
			{
				SetThreadName(GetCurrentThreadId(), this->name);
			}
			this->_execute();
		}), WorkItemPriority::Normal, WorkItemOptions::TimeSliced));
#endif
#else
		pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t));
		this->id = thread;
		pthread_create(thread, NULL, &_asyncCall, &this->runner);
#ifndef __APPLE__
		if (this->name != "")
		{
			pthread_setname_np(*thread, this->name.cStr());
		}
#endif
#endif
	}

	void Thread::_platformJoin()
	{
		if (this->id == NULL)
		{
			return;
		}
#ifdef _WIN32
#ifndef _UWP
		WaitForSingleObject(this->id, INFINITE);
#else
		IAsyncAction^ action = ((AsyncActionWrapper*)this->id)->asyncAction;
		int i = 0;
		while (action->Status != AsyncStatus::Completed &&
			action->Status != AsyncStatus::Canceled &&
			action->Status != AsyncStatus::Error &&
			i < 100)
		{
			Thread::sleep(50);
			++i;
		}
		if (i >= 100)
		{
			i = 0;
			action->Cancel();
			while (action->Status != AsyncStatus::Completed &&
				action->Status != AsyncStatus::Canceled &&
				action->Status != AsyncStatus::Error &&
				i < 100)
			{
				Thread::sleep(50);
				++i;
			}
		}
#endif
#else
		pthread_join(*((pthread_t*)this->id), 0);
#endif
	}

	void Thread::_platformResume()
	{
#ifdef _WIN32
#ifndef _UWP
		ResumeThread(this->id);
#else
		// not available in WinRT
		Log::warn(logTag, "Thread::resume() is not available on WinRT.");
#endif
#endif
	}

	void Thread::_platformPause()
	{
#ifdef _WIN32
#ifndef _UWP
		SuspendThread(this->id);
#else
		// not available in WinRT
		Log::warn(logTag, "Thread::pause() is not available on WinRT.");
#endif
#endif
	}

	void Thread::_platformStop()
	{
		if (this->id == NULL)
		{
			return;
		}
#ifdef _WIN32
#ifndef _UWP
		TerminateThread(this->id, 0);
#else
		((AsyncActionWrapper*)this->id)->asyncAction->Cancel();
#endif
#elif defined(__ANDROID__)
		pthread_kill(*((pthread_t*)this->id), 0);
#else
		pthread_cancel(*((pthread_t*)this->id));
#endif
	}

	void Thread::sleep(float milliseconds)
	{
#ifdef _WIN32
#ifndef _UWP
		Sleep((int)milliseconds);
#else
		WaitForSingleObjectEx(GetCurrentThread(), (int)milliseconds, 0);
#endif
#else
		usleep(milliseconds * 1000);
#endif
	}

	Thread::Thread(const Thread& other) :
		runner(this)
	{
		throw ObjectCannotCopyException("hltypes::Thread");
	}

	Thread& Thread::operator=(Thread& other)
	{
		throw ObjectCannotAssignException("hltypes::Thread");
		return (*this);
	}

}
