/************************************************************************************\
This source file is part of the KS(X) audio library                                  *
For latest info, see http://code.google.com/p/libxal/                                *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef HLTYPES_THREAD_H
#define HLTYPES_THREAD_H

#ifndef _WIN32
#include <pthread.h>
#endif

#include "hltypesExport.h"

namespace hltypes
{
	class hltypesExport thread
	{
	public:
		thread(void (*function)());
		~thread();
		void setFunction(void (*value)()) { this->function = value; }
	
		void start();
		void stop();
		void resume();
		void pause();
		
		void enterCritical();
		void leaveCritical();
		virtual void execute();
		void join();
		static void sleep(float miliseconds);
		
		//static thread* getCurrentThread();

	protected:
		void (*function)();
#ifdef _WIN32
		void* id;
		static void* criticalSection;
#else
		pthread_t id;
#endif
		volatile bool running;
		
		
	};
}

typedef hltypes::thread hthread;

#endif
