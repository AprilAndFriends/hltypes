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

#include "hmutex.h"

namespace hltypes
{
	Mutex::Mutex()
	{
#ifdef _WIN32
		this->handle = 0;
#else
		pthread_mutex_init(&this->handle, 0);
#endif
	}

	Mutex::~Mutex()
	{
#ifdef _WIN32
		if (this->handle)
		{
			CloseHandle(this->handle);
		}
#else
		pthread_mutex_destroy(&this->handle);
#endif
	}

	void Mutex::lock()
	{
#ifdef _WIN32
		if (!this->handle)
		{
			this->handle = CreateMutex(0, 0, 0);
		}
		WaitForSingleObject(this->handle, INFINITE);
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