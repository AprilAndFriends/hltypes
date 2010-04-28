/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef _HLTYPES_HFILE
#define _HLTYPES_HFILE

#include "hltypesExport.h"
#include "hstring.h"

namespace hltypes
{
	class file
	{
		file(string filename,const char* read_mode="r");
		file(const char* filename,const char* read_mode="r");
		~file();
		
		
	};
}

typedef hltypes::file hfile;


#endif

