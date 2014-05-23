/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>
#include <stdlib.h>

#include "exception.h"
#include "hltypesUtil.h"
#include "hmutex.h"
#include "hplatform.h"

namespace hltypes
{
	Mutex::Mutex()
	{
#ifdef _WIN32
#ifndef _WINRT // WinXP does not have CreateMutexEx()
		this->handle = CreateMutex(0, 0, 0);
#else
		this->handle = CreateMutexEx(NULL, NULL, 0, SYNCHRONIZE);
#endif
		if (this->handle == 0)
		{
			throw hl_exception("Could not create mutex.");
		}
#else
		this->handle = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((pthread_mutex_t*)this->handle, 0);
#endif
	}

	Mutex::~Mutex()
	{
#ifdef _WIN32
		CloseHandle(this->handle);
#else
		pthread_mutex_destroy((pthread_mutex_t*)this->handle);
		free((pthread_mutex_t*)this->handle);
		this->handle = NULL;
#endif
	}

	void Mutex::lock()
	{
#ifdef _WIN32
		WaitForSingleObjectEx(this->handle, INFINITE, FALSE);
#else
		pthread_mutex_lock((pthread_mutex_t*)this->handle);
#endif
	}

	void Mutex::unlock()
	{
#ifdef _WIN32
		ReleaseMutex(this->handle);
#else
		pthread_mutex_unlock((pthread_mutex_t*)this->handle);
#endif
	}
	
}

