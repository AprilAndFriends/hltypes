/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
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
#include "hltypesUtil.h"
#include "hmutex.h"

namespace hltypes
{
	Mutex::Mutex()
	{
#ifdef _WIN32
#if !_HLWINRT
		this->handle = CreateMutex(0, 0, 0);
		if (this->handle == 0)
		{
			throw hl_exception("Could not create mutex");
		}
#else
#endif
#else
		pthread_mutex_init(&this->handle, 0);
#endif
	}

	Mutex::~Mutex()
	{
#ifdef _WIN32
		CloseHandle(this->handle);
#else
		pthread_mutex_destroy(&this->handle);
#endif
	}

	void Mutex::lock()
	{
#ifdef _WIN32
		WaitForSingleObjectEx(this->handle, INFINITE, FALSE);
#else
		pthread_mutex_lock(&this->handle);
#endif
	}

	void Mutex::unlock()
	{
#ifdef _WIN32
		ReleaseMutex(this->handle);
#else
		pthread_mutex_unlock(&this->handle);
#endif
	}
	
}
