/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#include "hmutex.h"
#include "hltypesUtil.h"
#include "hthread.h"

#if _HLWINRT
#using <Windows.winmd>
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
#endif

namespace hltypes
{
#ifdef _WIN32
	unsigned long WINAPI async_call(void* param)
	{
#else
	void *async_call(void* param)
	{
#endif
		Thread* t = (Thread*)param;
		t->execute();
#ifdef _WIN32
		return 0;
#else
		pthread_exit(NULL);
#endif
	}

#if _HLWINRT
	struct AsyncActionWrapper
	{
	public:
		IAsyncAction^ async_action;
		AsyncActionWrapper(IAsyncAction^ async_action)
		{
			this->async_action = async_action;
		}
	};
	
	WorkItemHandler^ work_item_handler = ref new WorkItemHandler([&](IAsyncAction^ work_item)
    {
		// TODO - WinRT
	});
#endif
	
	Thread::Thread(void (*function)()) : running(false), id(NULL)
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
#if !_HLWINRT
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
#if !_HLWINRT
		this->id = CreateThread(0, 0, &async_call, this, 0, 0);
#else
		this->id = new AsyncActionWrapper(ThreadPool::RunAsync(work_item_handler,
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
			(*this->function)();
			this->running = false;
		}
	}

	void Thread::join()
	{
		this->running = false;
#ifdef _WIN32
#if !_HLWINRT
		WaitForSingleObject(this->id, INFINITE);
		if (this->id)
		{
			CloseHandle(this->id);
			this->id = 0;
		}
#else
		((AsyncActionWrapper*)this->id)->async_action->Close();
#endif
#else
		pthread_join(this->id, 0);
#endif
	}
	
	void Thread::resume()
	{
#ifdef _WIN32
#if !_HLWINRT
		ResumeThread(this->id);
#else
		// not available in WinRT
#endif
#endif
	}
	
	void Thread::pause()
	{
#ifdef _WIN32
#if !_HLWINRT
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
#if !_HLWINRT
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
#if !_HLWINRT
		Sleep((int)miliseconds);
#else
		WaitForSingleObjectEx(GetCurrentThread(), (int)miliseconds, 0);
#endif
#else
		usleep(miliseconds * 1000);
#endif
	}
}
