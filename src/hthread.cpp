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
#ifndef NO_THREADING
		Thread* t = (Thread*)param;
		t->execute();
#ifdef _WIN32
		return 0;
#else
		pthread_exit(NULL);
#endif
#endif
#ifdef NO_THREADING
		return NULL;
#endif
	}
	
	Thread::Thread(void (*function)()) : running(false), id(0)
	{
#ifndef NO_THREADING
		this->function = function;
#endif
	}

	Thread::~Thread()
	{
#ifndef NO_THREADING
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
#endif
	}

	void Thread::start()
	{
#ifndef NO_THREADING
		this->running = true;
#ifdef _WIN32
		this->id = CreateThread(0, 0, &asyncCall, this, 0, 0);
#else
		pthread_create(&this->id, NULL, &asyncCall, this);
#endif
#endif
	}
	
	void Thread::execute()
	{
#ifndef NO_THREADING
		if (this->function != NULL)
		{
			this->running = true;
			(*function)();
			this->running = false;
		}
#endif
	}

	void Thread::join()
	{
#ifndef NO_THREADING
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
#endif
	}
	
	void Thread::resume()
	{
#ifndef NO_THREADING
#ifdef _WIN32
		ResumeThread(this->id);
#endif
#endif
	}
	
	void Thread::pause()
	{
#ifndef NO_THREADING
#ifdef _WIN32
		SuspendThread(this->id);
#endif
#endif
	}
	
	void Thread::stop()
	{
#ifndef NO_THREADING
		if (this->running)
		{
#ifdef _WIN32
			TerminateThread(this->id, 0);
#else
			pthread_cancel(this->id);
#endif
		}
#endif
	}
	
	void Thread::enterCritical()
	{
#ifndef NO_THREADING
#ifdef _WIN32
#endif
#endif
	}
	
	void Thread::leaveCritical()
	{
#ifndef NO_THREADING
#ifdef _WIN32
#endif
#endif
	}
	
	void Thread::sleep(float miliseconds)
	{
#ifndef NO_THREADING
#ifdef _WIN32
		Sleep((int)miliseconds);
#else
		usleep(miliseconds * 1000);
#endif
#endif
	}
}
