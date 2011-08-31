/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "exception.h"
#include "hmutex.h"

namespace hltypes
{
	Mutex::Mutex()
	{
#ifndef NO_THREADING
#ifdef _WIN32
		this->handle = CreateMutex(0, 0, 0);
		if (this->handle == 0)
		{
			throw hl_exception("Could not create mutex");
		}
#else
		pthread_mutex_init(&this->handle, 0);
#endif
#endif
	}

	Mutex::~Mutex()
	{
#ifndef NO_THREADING
#ifdef _WIN32
		CloseHandle(this->handle);
#else
		pthread_mutex_destroy(&this->handle);
#endif
#endif
	}

	void Mutex::lock()
	{
#ifndef NO_THREADING
#ifdef _WIN32
		WaitForSingleObject(this->handle, INFINITE);
#else
		pthread_mutex_lock(&this->handle);
#endif
#endif
	}

	void Mutex::unlock()
	{
#ifndef NO_THREADING
#ifdef _WIN32
		ReleaseMutex(this->handle);
#else
		pthread_mutex_unlock(&this->handle);
#endif
#endif
	}
	
}
