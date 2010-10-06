/************************************************************************************\
* This source dir is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef HLTYPES_HDIR_H
#define HLTYPES_HDIR_H

#include "hltypesExport.h"

#include "hstring.h"

namespace hltypes
{
	class hltypesExport dir
	{
	public:
		dir();
		~dir();
		
		// static
		
		static void makedirs(chstr path);
		
	};
}

typedef hltypes::dir hdir;

#endif

