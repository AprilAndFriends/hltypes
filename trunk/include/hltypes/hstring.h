/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <string>
#include "hltypesExport.h"

namespace hltypes
{
	class hltypesExport string : public std::basic_string<char>
	{
	public:
		string();
		string(const char* s);
		string(const string& s);
		
		void split(const char splitter);
		void split(const char* splitter);
		void split(const string& splitter);
		
		operator float();
		operator int();
		
		void operator=(const float f);
		bool operator==(const float f);

	};
}

typedef hltypes::string hstr;
