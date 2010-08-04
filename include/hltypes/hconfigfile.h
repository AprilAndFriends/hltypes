/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef _HLTYPES_HCONFIGFILE
#define _HLTYPES_HCONFIGFILE

#include <map>

#include "hltypesExport.h"
#include "hstring.h"

namespace hltypes
{
	class hltypesExport config_file
	{
		hstr filename;
		std::map<hstr, hstr> entries;
	public:
		config_file(chstr filename);
		config_file();
		~config_file();
		
		void read(chstr filename);
		
		chstr operator[](const char* var);
		chstr operator[](chstr var);
		void set(chstr key, chstr value);
		
		std::map<hstr, hstr>& _getInternalMap() { return this->entries; }
	};
}

typedef hltypes::config_file hcfgfile;

#endif

