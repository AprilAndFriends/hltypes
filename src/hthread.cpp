/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 2.14
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#include <pthread.h>
#endif

#include "harray.h"
#include "hmutex.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hthread.h"

#ifdef _WINRT
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
#endif

namespace hltypes
{
#ifdef _WIN32
	unsigned long WINAPI async_call(void* param)
#else
	void* async_call(void* param)
#endif
	{
		Thread* t = (Thread*)param;
		t->execute();
#ifdef _WIN32
		return 0;
#else
		pthread_exit(NULL);
		return NULL;
#endif
	}

#ifdef _WINRT
	struct AsyncActionWrapper
	{
	public:
		IAsyncAction^ async_action;
		AsyncActionWrapper(IAsyncAction^ async_action)
		{
			this->async_action = async_action;
		}
	};
#endif
	
	Thread::Thread(void (*function)(Thread*)) : running(false), id(0)
	{
		this->function = function;
#ifndef _WIN32
		this->id = (pthread_t*)malloc(sizeof(pthread_t));
#endif
	}

	Thread::~Thread()
	{
		if (this->running)
		{
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
			this->id = NULL;
		}
	}

	void Thread::start()
	{
		this->running = true;
#ifdef _WIN32
#ifndef _WINRT
		this->id = CreateThread(0, 0, &async_call, this, 0, 0);
#else
		this->id = new AsyncActionWrapper(ThreadPool::RunAsync(
			ref new WorkItemHandler([&](IAsyncAction^ work_item)
			{
				this->execute();
			}),
			WorkItemPriority::Normal, WorkItemOptions::TimeSliced));
#endif
#else
		pthread_create((pthread_t*)this->id, NULL, &async_call, this);
#endif
	}
	
	void Thread::execute()
	{
		if (this->function != NULL)
		{
			this->running = true;
			(*this->function)(this);
			this->running = false;
		}
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
		IAsyncAction^ action = ((AsyncActionWrapper*)this->id)->async_action;
		int i = 0;
		while (action->Status != AsyncStatus::Completed &&
			action->Status != AsyncStatus::Canceled &&
			action->Status != AsyncStatus::Error &&
			i < 100)
		{
			hthread::sleep(50);
			i++;
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
				hthread::sleep(50);
				i++;
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
			((AsyncActionWrapper*)this->id)->async_action->Cancel();
#endif
#elif defined(_ANDROID)
			pthread_kill(*((pthread_t*)this->id), 0);
#else
			pthread_cancel(*((pthread_t*)this->id));
#endif
		}
	}
	
	void Thread::sleep(float miliseconds)
	{
#ifdef _WIN32
#ifndef _WINRT
		Sleep((int)miliseconds);
#else
		WaitForSingleObjectEx(GetCurrentThread(), (int)miliseconds, 0);
#endif
#else
		usleep(miliseconds * 1000);
#endif
	}
}
