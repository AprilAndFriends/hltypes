/// @file
/// @version 2.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides functionality of a semaphore for multithreading.

#ifndef HLTYPES_SEMAPHORE_H
#define HLTYPES_SEMAPHORE_H

#include "hltypesExport.h"
#include "hstring.h"

namespace hltypes
{
	/// @brief Provides functionality of a Semaphore for multithreading.
	class hltypesExport Semaphore
	{
	public:
		/// @brief Basic constructor.
		/// @param[in] max_count The max lock count.
		/// @param[in] name The internal name.
		Semaphore(int max_count, const String& name = "");
		/// @brief Destructor.
		~Semaphore();
		/// @brief Returns the semaphore name.
		/// @return The semaphore name.
		inline int getMaxCount() { return this->max_count; }
		/// @brief Returns the semaphore name.
		/// @return The semaphore name.
		inline String getName() { return this->name; }
		/// @brief Decreases the lock counter of the Semaphore.
		/// @note If the lock counter is zero, the caller thread will wait until a previous thread increases it.
		void lock();
		/// @brief Increases the lock counter of the Semaphore.
		/// @note Use this when you're done with thread-safe sections of your code.
		void unlock();
		
	protected:
		/// @brief Semaphore OS handle.
		void* handle;
		/// @brief Semaphore max count.
		int max_count;
		/// @brief Semaphore name.
		String name;

	};
}

/// @brief Alias for simpler code.
typedef hltypes::Semaphore hsemaphore;

#endif
