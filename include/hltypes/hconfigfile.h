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

#include "hltypesExport.h"
#include "hstring.h"
#include <map>

namespace hltypes
{
	class hltypesExport config_file
	{
		string filename;
		std::map<string,string> entries;
	public:
		config_file(string filename);
		config_file();
		~config_file();
		
		void read(string filename);
		
		chstr operator[](const char* var);
		chstr operator[](chstr var);
		void set(chstr key,chstr value);
		
		std::map<string,string>& _getInternalMap() { return this->entries; }
	};
}

typedef hltypes::config_file hcfgfile;


#endif

