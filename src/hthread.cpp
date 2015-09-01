/// @file
/// @version 3.0
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

#ifdef _WINRT
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
		Thread* t = (Thread*)param;
#ifdef _MSC_VER
		if (t->getName() != "")
		{
			SetThreadName(GetCurrentThreadId(), t->getName());
		}
#endif
		t->execute();
		return 0;
	}
#else
	static void* _asyncCall(void* param)
	{
		Thread* t = (Thread*)param;
#ifdef __APPLE__
		if (t->getName() != "")
		{
			pthread_setname_np(t->getName().cStr());
		}
#endif
		t->execute();
		pthread_exit(NULL);
		return NULL;
	}
#endif

#ifdef _WINRT
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

	Thread::Thread(void (*function)(Thread*), const String& name) : id(0), running(false)
	{
		this->function = function;
		this->name = name;
#ifndef _WIN32
		this->id = (pthread_t*)malloc(sizeof(pthread_t));
#endif
	}

	Thread::~Thread()
	{
		if (this->running)
		{
			Log::warn(logTag, "Thread still running in destructor! Attempting 'stop', but this may be unsafe. The thread should be joined before deleting it.");
			this->stop();
		}
		if (this->id != NULL)
		{
#ifdef _WIN32
#ifndef _WINRT
			CloseHandle(this->id);
#else
			delete this->id;
#endif
#else
			free((pthread_t*)this->id);
#endif
		}
	}

	void Thread::start()
	{
		this->running = true;
#ifdef _WIN32
#ifndef _WINRT
		this->id = CreateThread(0, 0, &_asyncCall, this, 0, 0);
#else
		this->id = new AsyncActionWrapper(ThreadPool::RunAsync(ref new WorkItemHandler([&](IAsyncAction^ workItem)
		{
			if (this->name != "")
			{
				SetThreadName(GetCurrentThreadId(), this->name);
			}
			this->execute();
		}), WorkItemPriority::Normal, WorkItemOptions::TimeSliced));
#endif
#else
		pthread_t* thread = (pthread_t*)this->id;
		pthread_create(thread, NULL, &_asyncCall, this);
#ifndef __APPLE__
		if (this->name != "")
		{
			pthread_setname_np(*thread, this->name.cStr());
		}
#endif
#endif
	}

	void Thread::execute()
	{
		if (this->function != NULL)
		{
			this->running = true;
			(*this->function)(this);
		}
		this->running = false;
	}

	void Thread::join()
	{
		this->running = false;
#ifdef _WIN32
#ifndef _WINRT
		WaitForSingleObject(this->id, INFINITE);
		if (this->id != NULL)
		{
			CloseHandle(this->id);
			this->id = NULL;
		}
#else
		if (this->id == NULL) // means that it wasn't started yet
		{
			return;
		}
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

	void Thread::resume()
	{
#ifdef _WIN32
#ifndef _WINRT
		ResumeThread(this->id);
#else
		// not available in WinRT
		Log::warn(logTag, "Thread::resume() is not available on WinRT.");
#endif
#endif
	}

	void Thread::pause()
	{
#ifdef _WIN32
#ifndef _WINRT
		SuspendThread(this->id);
#else
		// not available in WinRT
		Log::warn(logTag, "Thread::pause() is not available on WinRT.");
#endif
#endif
	}

	void Thread::stop()
	{
		if (this->running)
		{
			this->running = false;
#ifdef _WIN32
#ifndef _WINRT
			TerminateThread(this->id, 0);
#else
			((AsyncActionWrapper*)this->id)->asyncAction->Cancel();
#endif
#elif defined(_ANDROID)
			pthread_kill(*((pthread_t*)this->id), 0);
#else
			pthread_cancel(*((pthread_t*)this->id));
#endif
		}
	}

	void Thread::sleep(float milliseconds)
	{
#ifdef _WIN32
#ifndef _WINRT
		Sleep((int)milliseconds);
#else
		WaitForSingleObjectEx(GetCurrentThread(), (int)milliseconds, 0);
#endif
#else
		usleep(milliseconds * 1000);
#endif
	}

	Thread::Thread(const Thread& other)
	{
		throw ObjectCannotCopyException("hltypes::Thread");
	}

	Thread& Thread::operator=(Thread& other)
	{
		throw ObjectCannotAssignException("hltypes::Thread");
		return (*this);
	}

}
