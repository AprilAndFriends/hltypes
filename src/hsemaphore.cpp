/// @file
/// @version 4.0
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

#define __HL_INCLUDE_PLATFORM_HEADERS
#include "hlog.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hsemaphore.h"

namespace hltypes
{
	Semaphore::Semaphore(int maxCount, const String& name) :
		handle(NULL)
	{
		this->maxCount = maxCount;
		this->name = name;
#ifdef _WIN32
#ifndef _UWP // WinXP does not have CreateSemaphoreEx()
		this->handle = CreateSemaphoreW(NULL, this->maxCount, this->maxCount, L""); // this->name is NOT a system name!
#else
		this->handle = CreateSemaphoreExW(NULL, this->maxCount, this->maxCount, L"", 0, 0); // this->name is NOT a system name!
#endif
#else
		this->handle = sem_open(this->name.cStr(), O_CREAT|O_EXCL, this->maxCount);
		if (this->handle == SEM_FAILED)
		{
			Log::writef("semaphore", "Error creating POSIX semaphore '%s'! errno = %s", this->name.cStr(), String(strerror(errno)).cStr());
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
	
	Semaphore::Semaphore(const Semaphore& other)
	{
		throw ObjectCannotCopyException("hltypes::Semaphore");
	}

	Semaphore& Semaphore::operator=(Semaphore& other)
	{
		throw ObjectCannotAssignException("hltypes::Semaphore");
		return (*this);
	}

}
