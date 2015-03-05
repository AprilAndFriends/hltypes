/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Encapsulates all containers and adds high level methods.

#ifndef HLTYPES_CONTAINER_H
#define HLTYPES_CONTAINER_H

#include "hstring.h"

namespace hltypes
{
	/// @brief Encapsulates container functionality and adds high level methods.
	template <class T>
	class Container
	{
	public:
		/// @brief Empty constructor.
		inline Container()
		{
		}
		/// @brief Destructor.
		virtual inline ~Container()
		{
		}
		
	};
	
}

#endif
