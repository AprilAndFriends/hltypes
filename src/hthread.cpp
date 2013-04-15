/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 2.1
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

#if _HL_WINRT
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
#endif
	}

#if _HL_WINRT
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
	}

	Thread::~Thread()
	{
		if (this->running)
		{
			this->stop();
		}
#ifdef _WIN32
		if (this->id != NULL)
		{
#if !_HL_WINRT
			CloseHandle(this->id);
#else
			delete this->id;
#endif
		}
#endif
	}

	void Thread::start()
	{
		this->running = true;
#ifdef _WIN32
#if !_HL_WINRT
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
		pthread_create(&this->id, NULL, &async_call, this);
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
#if !_HL_WINRT
		WaitForSingleObject(this->id, INFINITE);
		if (this->id != 0)
		{
			CloseHandle(this->id);
			this->id = 0;
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
		pthread_join(this->id, 0);
#endif
	}
	
	void Thread::resume()
	{
#ifdef _WIN32
#if !_HL_WINRT
		ResumeThread(this->id);
#else
		// not available in WinRT
#endif
#endif
	}
	
	void Thread::pause()
	{
#ifdef _WIN32
#if !_HL_WINRT
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
#ifdef _WIN32
#if !_HL_WINRT
			TerminateThread(this->id, 0);
#else
			((AsyncActionWrapper*)this->id)->async_action->Cancel();
#endif
#elif defined(_ANDROID)
			pthread_kill(this->id, 0);
#else
			pthread_cancel(this->id);
#endif
		}
	}
	
	void Thread::enterCritical()
	{
		// TODO
	}
	
	void Thread::leaveCritical()
	{
		// TODO
	}
	
	void Thread::sleep(float miliseconds)
	{
#ifdef _WIN32
#if !_HL_WINRT
		Sleep((int)miliseconds);
#else
		WaitForSingleObjectEx(GetCurrentThread(), (int)miliseconds, 0);
#endif
#else
		usleep(miliseconds * 1000);
#endif
	}
}
