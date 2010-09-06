/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "harray.h"
#include "hstring.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <algorithm>
#include <stdarg.h>


#define HL_E_TOLERANCE (0.01f)

typedef std::basic_string<char> stdstr;

namespace hltypes
{
	string::string() : stdstr() {}
	string::string(const char c) : stdstr(1, c) {}
	string::string(const char c, const int len) : stdstr(len, c) {}
	string::string(const char* s) : stdstr(s) {}
	string::string(const string& s) : stdstr(s) {}
	string::string(const std::string& s) : stdstr(s) {}
	string::string(const char* s, const int len) : stdstr(s, len) {}
	string::string(const string& s, const int len) : stdstr(s, len) {}
	string::string(const int i) { this->operator=(i); }
	string::string(const unsigned int i) { this->operator=(i); }
	string::string(const float f) { this->operator=(f); }
	string::string(const float f,int precision)
	{
		char fmt[16],s[64];
		sprintf(fmt,"%%.%df",precision);
		sprintf(s, fmt, f);
		stdstr::operator=(s);
	}

	harray_hstr string::split(const char* splitter, unsigned int n) const
	{
		harray_hstr out;
		const char *s = this->c_str(), *p;
		int splitter_len = strlen(splitter);
		while ((p = strstr(s, splitter)) != 0 && n > 0)
		{
			out.push_back(string(s, p - s));
			s = p + splitter_len;
			n--;
		}
		out.push_back(string(s));
		return out;
	}
	
	harray_hstr string::rsplit(const char* splitter, unsigned int n) const
	{
		harray_hstr out;
		const char *s = this->c_str(), *p;
		int splitter_len = strlen(splitter);
		for (p = s + strlen(s) - 1; p != s && n > 0; p--)
		{
			if (strncmp(p, splitter, splitter_len) == 0)
			{
				n--;
			}
		}
		if (s != p)
		{
			out.push_back(string(s, p - s + 1));
			s = p + 1 + splitter_len;
		}
		while ((p = strstr(s, splitter)) != 0)
		{
			out.push_back(string(s, p - s));
			s = p + splitter_len;
		}
		out.push_back(string(s));
		return out;
	}

	bool string::split(const char* splitter, string& out_left, string& out_right) const
	{
		int index = this->find(splitter);
		if (index < 0)
		{
			return false;
		}
		out_left = stdstr::substr(0, index);
		out_right = stdstr::substr(index + strlen(splitter), 1000);
		return true;
	}
	
	bool string::rsplit(const char* splitter, string& out_left, string& out_right) const
	{
		int index = this->rfind(splitter);
		if (index < 0)
		{
			return false;
		}
		out_left = stdstr::substr(0, index);
		out_right = stdstr::substr(index + strlen(splitter), 1000);
		return true;
	}

	bool string::starts_with(const char* s) const
	{
		return (strncmp(this->c_str(), s, strlen(s)) == 0);
	}

	bool string::ends_with(const char* s) const
	{
		const char* thiss = this->c_str();
		int thislen = this->size(), slen = strlen(s);
		if (slen > thislen)
		{
			return false;
		}
		return (strcmp(thiss + thislen - slen, s) == 0);
	}
	
	string string::lower() const
	{
		hstr s(*this);
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}
	
	string string::upper() const
	{
		hstr s(*this);
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	string string::reverse() const
	{
		hstr s(*this);
		std::reverse(s.begin(), s.end());
		return s;
	}

	string string::replace(const char* what, const char* with_what) const
	{
		const char *s = this->c_str(), *p;
		string out;
		int what_len = strlen(what);
		if (what_len == 0)
		{
			return *this;	
		}
		while ((p = strstr(s, what)) != 0)
		{
			out.append(s, p - s);
			out.append(with_what);
			s = p + what_len;
		}
		out.append(s);
		return out;
	}
	
	bool string::contains(const char* s) const
	{
		return stdstr::find(s) != stdstr::npos;
	}
	
	int string::size() const
	{
		return stdstr::size();
	}
	
/******* TYPE EXTENSION FUNCTIONS **************************************/
	harray_hstr string::split(const char splitter, unsigned int n) const
	{
		const char sp[2] = {splitter, 0};
		return this->split(sp, n);
	}

	harray_hstr string::split(const string& splitter, unsigned int n) const
	{
		return this->split(splitter.c_str(), n);
	}
	
	harray_hstr string::rsplit(const char splitter, unsigned int n) const
	{
		const char sp[2] = {splitter, 0};
		return this->rsplit(sp, n);
	}

	harray_hstr string::rsplit(const string& splitter,unsigned int n) const
	{
		return this->rsplit(splitter.c_str(), n);
	}
	
	bool string::split(const char splitter,string& out_left,string& out_right) const
	{
		const char sp[2] = {splitter, 0};
		return this->split(sp, out_left, out_right);
	}
		
	bool string::split(const string& splitter,string& out_left, string& out_right) const
	{
		return this->split(splitter.c_str(), out_left, out_right);
	}
	
	bool string::rsplit(const char splitter, string& out_left, string& out_right) const
	{
		const char sp[2] = {splitter, 0};
		return this->rsplit(sp, out_left, out_right);
	}
	
	bool string::rsplit(const string& splitter, string& out_left, string& out_right) const
	{
		return this->rsplit(splitter.c_str(), out_left, out_right);
	}
    
    int string::count(const char substr)
    {
        char c[2] = {substr, 0};
        return count(c);
    }
    
    int string::count(const char* substr)
    {
        int c = 0;
        hstr tmp(this->c_str());
        for(int i = 0; i < size(); ++i)
        {
            if(tmp(i, -1).starts_with(substr))
            {
                c++;
                i += (hstr(substr).size() - 1);
            }
        }
        return c;
    }
    
    int string::count(const string& substr)
    {
        return count(substr.c_str());
    }

	bool string::starts_with(const string& s) const
	{
		return this->starts_with(s.c_str());
	}

	bool string::ends_with(const string& s) const
	{
		return this->ends_with(s.c_str());
	}

	string string::replace(const string& what, const char* with_what) const
	{
		return this->replace(what.c_str(), with_what);
	}
	
	string string::replace(const char* what, const string& with_what) const
	{
		return this->replace(what, with_what.c_str());
	}
	
	string string::replace(const string& what, const string& with_what) const
	{
		return this->replace(what.c_str(), with_what.c_str());
	}
	
	string string::replace(size_t pos1, size_t n1, const string& str)
	{
		return stdstr::replace(pos1, n1, str);
	}
	
	string string::replace(size_t pos1, size_t n1, const string& str, size_t pos2, size_t n2)
	{
		return stdstr::replace(pos1, n1, str, pos2, n2);
	}
	
	string string::replace(size_t pos1, size_t n1, const char* s, size_t n2)
	{
		return stdstr::replace(pos1, n1, s, n2);
	}
	
	string string::replace(size_t pos1, size_t n1, const char* s)
	{
		return stdstr::replace(pos1, n1, s);
	}
	
	string string::replace(size_t pos1, size_t n1, size_t n2, char c)
	{
		return stdstr::replace(pos1, n1, n2, c);
	}
	
	bool string::contains(const char c) const
	{
		char st[2]={c,0};
		return this->contains(st);
	}
	
	bool string::contains(const string& s) const
	{
		return this->contains(s.c_str());
	}
/******* SUBSTR OPERATORS **********************************************/
	string string::operator()(int start, int count) const
	{
		return stdstr::substr(start, count);
	}
	
	string string::operator()(int start, int count, int step) const
	{
		if (count < 0)
		{
			count = stdstr::npos;
		}
		if (step == 1)
		{
			return stdstr::substr(start, count);
		}
		hstr result;
		for (int i = start; i < start + count; i += step)
		{
			result += stdstr::at(i);
		}
		return result;
	}
	
	string string::operator()(int index) const
	{
		return stdstr::at(index);
	}
	
	char& string::operator[](int index)
	{
		return stdstr::at(index);
	}
/******* CAST OPERATORS ************************************************/
	string::operator float() const
	{
		float f;
		sscanf(this->c_str(), "%f", &f);
		return f;
	}
	
	string::operator int() const
	{
		int i;
		sscanf(this->c_str(), "%d", &i);
		return i;
	}
	
	string::operator unsigned int() const
	{
		unsigned int i;
		sscanf(this->c_str(), "%u", &i);
		return i;
	}
	
	string::operator bool() const
	{
		return (*this != "" && *this != "0" && *this != "false") ? true : false;
	}
	
/******* ASSIGNMENT OPERATORS ******************************************/
	void string::operator=(const float f)
	{
		char s[64];
		sprintf(s, "%f", f);
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
		sprintf(s, "%d", i);
		stdstr::operator=(s);
	}
	
	void string::operator=(const unsigned int i)
	{
		char s[64];
		sprintf(s, "%u", i);
		stdstr::operator=(s);
	}
	
	void string::operator=(const bool b)
	{
		stdstr::operator=(b ? "true" : "false");
	}
/******* CONCATENATION OPERATORS ***************************************/
	void string::operator+=(const float f)
	{
		string s = f;
		stdstr::append(s);
	}
	
	void string::operator+=(const std::string& s)
	{
		stdstr::append(s);
	}
	
	void string::operator+=(const char* s)
	{
		stdstr::append(s);
	}
	
	void string::operator+=(const int i)
	{
		string s = i;
		stdstr::append(s);
	}
	
	void string::operator+=(const unsigned int i)
	{
		string s = i;
		stdstr::append(s);
	}
	
	void string::operator+=(const bool b)
	{
		string s = (b ? "true" : "false");
		stdstr::append(s);
	}
	
	void string::operator+=(const char c)
	{
		char chstr[2]={c,0};
		stdstr::append(chstr);
	}
/******* COMPARISON OPERATORS ******************************************/
	bool string::operator==(const float f) const
	{
		return (fabs(((float)*this) - f) < HL_E_TOLERANCE);
	}
	
	bool string::operator==(const int i) const
	{
		return (((int)*this) == i);
	}
	
	bool string::operator==(const unsigned int i) const
	{
		return (((unsigned int)*this) == i);
	}
	
	bool string::operator==(const bool b) const
	{
		const char* cstr = this->c_str();
		return (strcmp(cstr, "1") == 0 && b ||
				strcmp(cstr, "0") == 0 && !b ||
				strcmp(cstr, "true") == 0 && b ||
				strcmp(cstr, "false") == 0 && !b);
	}

	bool string::operator==(const char* s) const
	{
		return (strcmp(this->c_str(), s) == 0);
	}
	
	bool string::operator==(const std::string& s) const
	{
		return (strcmp(this->c_str(), s.c_str()) == 0);
	}
/******* ADDITION OPERATORS *********************************************/
	string string::operator+(const char* s1) const
	{
		string s(*this);
		s.append(s1);
		return s;
	}
	
	string string::operator+(char* s1) const
	{ 
		return this->operator+((const char*) s1);
	}
	
	string string::operator+(const string& s1) const
	{
		string s(*this);
		s.append(s1);
		return s;
	}
	
	string string::operator+(const std::string& s1) const
	{
		string s(*this);
		s.append(s1);
		return s;
	}
	
	string string::operator+(const char c) const
	{
		char chstr[2]={c,0};
		string s(*this);
		s.append(chstr);
		return s;
	}
}
/******* GLOBAL ADDITION OPERATORS *******************************************/
hstr operator+(const char* s1, chstr s2)
{
	hstr s(s1);
	return (s + s2);
}

hstr operator+(char* s1, chstr s2)
{
	hstr s(s1);
	return (s + s2);
}

hstr hsprintf(const char* format, ...)
{
	char c[1024] = {0};
	va_list args;
	va_start(args, format);
	vsnprintf(c, 1023, format, args);
	va_end(args);
	return hstr(c);
}

hstr hsprintf_s(const char* format, ...) //2DO - possible implementation with dynamic buffer
{
	char c[65536] = {0};
	va_list args;
	va_start(args, format);
	vsnprintf(c, 65535, format, args);
	va_end(args);
	return hstr(c);
}

