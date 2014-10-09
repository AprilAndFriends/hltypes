/// @file
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef _WIN32
#include <semaphore.h>
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
		this->handle = (sem_t*)malloc(sizeof(sem_t));
		sem_init((sem_t*)this->handle, 0, this->max_count);
#endif
	}

	Semaphore::~Semaphore()
	{
#ifdef _WIN32
		CloseHandle(this->handle);
#else
		sem_destroy((sem_t*)this->handle);
		free((sem_t*)this->handle);
#endif
		this->handle = NULL;
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

