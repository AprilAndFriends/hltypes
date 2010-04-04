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
		
		void split(const char splitter) const;
		void split(const char* splitter) const;
		void split(const string& splitter) const;
		
		bool startswith(const char* s) const;
		bool startswith(const string& s) const;
		bool endswith(const char* s) const;
		bool endswith(const string& s) const;

		string replace(const char* what,const char* with_what) const;
		string replace(const string& what,const char* with_what) const;
		string replace(const char* what,const string& with_what) const;
		string replace(const string& what,const string& with_what) const;
		
		operator float() const;
		operator int() const;
		
		void operator=(const float f);
		void operator=(const int i);
		void operator=(const bool b);
		void operator=(const std::string& s);
		void operator=(const char* s);

		bool operator==(const float f) const;
		bool operator==(const int i) const;
		bool operator==(const bool b) const;
		bool operator==(const char* s) const;
		bool operator==(const std::string& s) const;

	};
}

typedef hltypes::string hstr;
typedef const hltypes::string& hcstr;
