/// @file
/// @version 2.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides functionality of a mutex for multithreading.

#ifndef HLTYPES_MUTEX_H
#define HLTYPES_MUTEX_H

#include "hltypesExport.h"
#include "hstring.h"

namespace hltypes
{
	/// @brief Provides functionality of a Mutex for multithreading.
	/// @todo Finish the class and fix remaining problems.
	class hltypesExport Mutex
	{
	public:
		/// @brief Basic constructor.
		Mutex(const String& name = "");
		/// @brief Destructor.
		~Mutex();
		/// @brief Returns the mutex name.
		/// @return The mutex name.
		inline hstr getName() { return this->name; }
		/// @brief Locks the Mutex.
		/// @note If another thread has lock, the caller thread will wait until the previous thread unlocks it.
		void lock();
		/// @brief Unlocks the Mutex.
		/// @note Use this when you're done with thread-safe sections of your code.
		void unlock();
		
	protected:
		/// @brief Mutex OS handle.
		void* handle;
		/// @brief Mutex name.
		String name;
		/// @brief Used internallly on Win32 so the much faster critical sections can be used instead of mutices and semaphores.
		bool locked;

	};
}

/// @brief Alias for simpler code.
typedef hltypes::Mutex hmutex;

#endif
