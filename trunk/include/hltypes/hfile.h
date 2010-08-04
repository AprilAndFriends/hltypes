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

#include <stdio.h>

#include "hltypesExport.h"
#include "hstring.h"

namespace hltypes
{
	class hltypesExport file
	{
		FILE* cfile;
	public:
		file(chstr filename, const char* access_mode = "r");
		file(const char* filename, const char* access_mode = "r");
		file();
		~file();
		
		void open(const char* filename, const char* access_mode = "r");
		hstr read_line();
		bool eof();
		void close();
		
		static bool exists(chstr filename);
		
	};
}

typedef hltypes::file hfile;

#endif

