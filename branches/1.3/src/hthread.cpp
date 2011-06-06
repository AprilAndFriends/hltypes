/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 1.0
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
#endif

#include "hmutex.h"
#include "hthread.h"

namespace hltypes
{
#ifdef _WIN32
	unsigned long WINAPI asyncCall(void* param)
	{
#else
	void *asyncCall(void* param)
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
	
	Thread::Thread(void (*function)()) : running(false), id(0)
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
		if (this->id != 0)
		{
			CloseHandle(this->id);
		}
#endif
	}

	void Thread::start()
	{
		this->running = true;
#ifdef _WIN32
		this->id = CreateThread(0, 0, &asyncCall, this, 0, 0);
#else
		pthread_create(&this->id, NULL, &asyncCall, this);
#endif
	}
	
	void Thread::execute()
	{
		if (this->function != NULL)
		{
			this->running = true;
			(*function)();
			this->running = false;
		}
	}

	void Thread::join()
	{
		this->running = false;
#ifdef _WIN32
		WaitForSingleObject(this->id, INFINITE);
		if (this->id)
		{
			CloseHandle(this->id);
			this->id = 0;
		}
#else
		pthread_join(this->id, 0);
#endif
	}
	
	void Thread::resume()
	{
#ifdef _WIN32
		ResumeThread(this->id);
#endif
	}
	
	void Thread::pause()
	{
#ifdef _WIN32
		SuspendThread(this->id);
#endif
	}
	
	void Thread::stop()
	{
		if (this->running)
		{
#ifdef _WIN32
			TerminateThread(this->id, 0);
#endif
			this->running = false;
			this->id = 0;
		}
	}
	
	void Thread::enterCritical()
	{
#ifdef _WIN32
#endif
	}
	
	void Thread::leaveCritical()
	{
#ifdef _WIN32
#endif
	}
	
	void Thread::sleep(float miliseconds)
	{
#ifdef _WIN32
		Sleep((int)miliseconds);
#else
		usleep(miliseconds * 1000);
#endif
	}
}
