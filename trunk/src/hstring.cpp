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

	void string::split(const char splitter) const
	{
	}

	void string::split(const char* splitter) const
	{
	}

	void string::split(const string& splitter) const
	{
	}
	
	bool string::startswith(const char* s) const
	{
		return (strncmp(this->c_str(),s,strlen(s)) == 0);
	}
	
	bool string::startswith(const string& s) const
	{ return this->startswith(s.c_str()); }

	bool string::endswith(const char* s) const
	{
		const char* thiss=this->c_str();
		int thislen=this->size(),slen=strlen(s);
		if (slen > thislen) return 0;
		
		return (strcmp(thiss+thislen-slen,s) == 0);
	}
	
	bool string::endswith(const string& s) const
	{ return this->endswith(s.c_str()); }

	string string::replace(const char* what,const char* with_what) const
	{
		const char *s=this->c_str(),*p;
		string out;
		int what_len=strlen(what);
		if (what_len == 0) return *this;
		
		while ((p = strstr(s,what)) != 0)
		{
			out.append(s,p-s);
			out.append(with_what);
			s=p+what_len;
		}
		out.append(s);
		return out;
	}
	
	string string::replace(const string& what,const char* with_what) const
	{ return this->replace(what.c_str(),with_what); }
	string string::replace(const char* what,const string& with_what) const
	{ return this->replace(what,with_what.c_str()); }
	string string::replace(const string& what,const string& with_what) const
	{ return this->replace(what.c_str(),with_what.c_str()); }
/******* CAST OPERATORS ************************************************/
	string::operator float() const
	{
		float f;
		sscanf(this->c_str(),"%f",&f);
		return f;
	}
	
	string::operator int() const
	{
		int i;
		sscanf(this->c_str(),"%d",&i);
		return i;
	}
/******* ASSIGNMENT OPERATORS ******************************************/
	void string::operator=(const float f)
	{
		char s[64];
		sprintf(s,"%f",f);
		stdstr::operator=(s);
	}
	
	void string::operator=(const std::string& s)
	{
		stdstr::operator=(s);
	}

	void string::operator=(const char* s)
	{
		stdstr::operator=(s);
	}
	
	void string::operator=(const int i)
	{
		char s[64];
		sprintf(s,"%d",i);
		stdstr::operator=(s);
	}
	
	void string::operator=(const bool b)
	{
		if (b == true) stdstr::operator=("true");
		else           stdstr::operator=("false");
	}
/******* COMPARISON OPERATORS ******************************************/
	bool string::operator==(const float f) const
	{
		return (fabs(((float) *this)-f) < 0.01f);
	}
	
	bool string::operator==(const int i) const
	{
		return (((int) *this) == i);
	}
	
	bool string::operator==(const bool b) const
	{
		const char* cstr=this->c_str();
		if (strcmp(cstr,"1") == 0 && b == true    ||
		    strcmp(cstr,"0") == 0 && b == false   ||
		    strcmp(cstr,"true") == 0 && b == true ||
		    strcmp(cstr,"false") == 0 && b == false) return true;
		
		else return false;
	}

	bool string::operator==(const char* s) const
	{
		return (strcmp(this->c_str(),s) == 0);
	}
	
	bool string::operator==(const std::string& s) const
	{
		return (strcmp(this->c_str(),s.c_str()) == 0);
	}

}
