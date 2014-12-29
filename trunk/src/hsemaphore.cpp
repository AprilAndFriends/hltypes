/// @file
/// @version 2.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef _WIN32
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "hlog.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hsemaphore.h"

namespace hltypes
{
	Semaphore::Semaphore(int max_count, const String& name) : handle(NULL)
	{
		this->max_count = max_count;
		this->name = name;
#ifdef _WIN32
#ifndef _WINRT // WinXP does not have CreateSemaphoreEx()
		this->handle = CreateSemaphoreW(NULL, this->max_count, this->max_count, L""); // this->name is NOT a system name!
#else
		this->handle = CreateSemaphoreExW(NULL, this->max_count, this->max_count, L"", 0, 0); // this->name is NOT a system name!
#endif
#else
		this->handle = sem_open(this->name.c_str(), O_CREAT|O_EXCL, this->max_count);
		if (this->handle == SEM_FAILED)
		{
			Log::writef("semaphore", "Error creating POSIX semaphore '%s'! errno = %s", this->name.c_str(), String(strerror(errno)).c_str());
		}
#endif
	}

	Semaphore::~Semaphore()
	{
#ifdef _WIN32
		CloseHandle(this->handle);
#else
		sem_close((sem_t*)this->handle);
#endif
	}

	void Semaphore::lock()
	{
#ifdef _WIN32
		WaitForSingleObjectEx(this->handle, INFINITE, FALSE);
#else
		sem_wait((sem_t*)this->handle);
#endif
	}

	void Semaphore::unlock()
	{
#ifdef _WIN32
		ReleaseSemaphore(this->handle, 1, NULL);
#else
		sem_post((sem_t*)this->handle);
#endif
	}
	
}

