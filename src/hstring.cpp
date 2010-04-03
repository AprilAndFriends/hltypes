/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "hstring.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

typedef std::basic_string<char> stdstr;

namespace hltypes
{

	string::string() : stdstr() {}
	string::string(const char* s) : stdstr(s) {}
	string::string(const string& s) : stdstr(s) {}



	void string::split(const char splitter)
	{
	}

	void string::split(const char* splitter)
	{
	}

	void string::split(const string& splitter)
	{
	}
	
	string::operator float()
	{
		float f;
		sscanf(this->c_str(),"%f",&f);
		
		return f;
	}
	
	string::operator int()
	{
		
	}
	
	void string::operator=(const float f)
	{
		char s[64];
		sprintf(s,"%f",f);
		stdstr::operator=(s);
	}
	
	bool string::operator==(const float f)
	{
		return (fabs(float(*this)-f) < 0.01f);
	}

}
