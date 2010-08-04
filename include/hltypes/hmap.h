/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef HLTYPES_MAP_H
#define HLTYPES_MAP_H

#include <map>
#include "hstring.h"

namespace hltypes
{
#define stdmap std::map<K, T>
	
	template <class K, class T> class Map : public stdmap
	{
	public:
		// constructors
		Map() : stdmap()
		{
		}
		
/******* OPERATORS *****************************************************/
		T& operator[](K key)
		{
			return stdmap::operator[](key);
		}
	
	};
	
}

#define hmap hltypes::Map

#endif
