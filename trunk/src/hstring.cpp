/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <algorithm>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "harray.h"
#include "hstring.h"
#include "util.h"

typedef std::basic_string<char> stdstr;

namespace hltypes
{
	string::string() : stdstr() { }
	string::string(const char c) : stdstr(1, c) { }
	string::string(const char c, const int len) : stdstr(len, c) { }
	string::string(const char* s) : stdstr(s) { }
	string::string(const string& s) : stdstr(s) { }
	string::string(const std::string& s) : stdstr(s) { }
	string::string(const char* s, const int len) : stdstr(s, len) { }
	string::string(const string& s, const int len) : stdstr(s, len) { }
	string::string(const int i) { this->operator=(i); }
	string::string(const unsigned int i) { this->operator=(i); }
	string::string(const float f) { this->operator=(f); }
	string::string(const float f, int precision)
	{
		char fmt[16], s[64];
		sprintf(fmt, "%%.%df", precision);
		sprintf(s, fmt, f);
		stdstr::operator=(s);
	}

	harray_hstr string::split(const char* delimiter, unsigned int n) const
	{
		harray_hstr out;
		const char *s = stdstr::c_str(), *p;
		int delimiter_len = strlen(delimiter);
		while ((p = strstr(s, delimiter)) != 0 && n > 0)
		{
			out += string(s, p - s);
			s = p + delimiter_len;
			n--;
		}
		out += string(s);
		return out;
	}
	
	harray_hstr string::rsplit(const char* delimiter, unsigned int n) const
	{
		harray_hstr out;
		const char *s = stdstr::c_str(), *p;
		int delimiter_len = strlen(delimiter);
		for (p = s + strlen(s) - 1; p != s && n > 0; p--)
		{
			if (strncmp(p, delimiter, delimiter_len) == 0)
			{
				n--;
			}
		}
		if (s != p)
		{
			out += string(s, p - s + 1);
			s = p + 1 + delimiter_len;
		}
		while ((p = strstr(s, delimiter)) != 0)
		{
			out += string(s, p - s);
			s = p + delimiter_len;
		}
		out += string(s);
		return out;
	}

	bool string::split(const char* delimiter, string& out_left, string& out_right) const
	{
		int index = this->find(delimiter);
		if (index < 0)
		{
			return false;
		}
		out_left = stdstr::substr(0, index);
		out_right = stdstr::substr(index + strlen(delimiter), 1000);
		return true;
	}
	
	bool string::rsplit(const char* delimiter, string& out_left, string& out_right) const
	{
		int index = this->rfind(delimiter);
		if (index < 0)
		{
			return false;
		}
		out_left = stdstr::substr(0, index);
		out_right = stdstr::substr(index + strlen(delimiter), 1000);
		return true;
	}

	bool string::starts_with(const char* s) const
	{
		return (strncmp(stdstr::c_str(), s, strlen(s)) == 0);
	}

	bool string::ends_with(const char* s) const
	{
		const char* thiss = stdstr::c_str();
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

	string string::trim(char c) const
	{
		return this->ltrim(c).rtrim(c);
	}

	string string::ltrim(char c) const
	{
		int index = stdstr::find_first_not_of(c);
		if (index < 0)
		{
			return *this;
		}
		return stdstr::substr(index, this->size() - index);
	}

	string string::rtrim(char c) const
	{
		int index = stdstr::find_last_not_of(c);
		if (index < 0)
		{
			return *this;
		}
		return stdstr::substr(0, index + 1);
	}

	string string::replace(const char* what, const char* with_what) const
	{
		const char *s = stdstr::c_str(), *p;
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
	harray_hstr string::split(const char delimiter, unsigned int n) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->split(sp, n);
	}

	harray_hstr string::split(const string& delimiter, unsigned int n) const
	{
		return this->split(delimiter.c_str(), n);
	}
	
	harray_hstr string::rsplit(const char delimiter, unsigned int n) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->rsplit(sp, n);
	}

	harray_hstr string::rsplit(const string& delimiter, unsigned int n) const
	{
		return this->rsplit(delimiter.c_str(), n);
	}
	
	bool string::split(const char delimiter, string& out_left,string& out_right) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->split(sp, out_left, out_right);
	}
		
	bool string::split(const string& delimiter, string& out_left, string& out_right) const
	{
		return this->split(delimiter.c_str(), out_left, out_right);
	}
	
	bool string::rsplit(const char delimiter, string& out_left, string& out_right) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->rsplit(sp, out_left, out_right);
	}
	
	bool string::rsplit(const string& delimiter, string& out_left, string& out_right) const
	{
		return this->rsplit(delimiter.c_str(), out_left, out_right);
	}
    
    int string::count(const char substr) const
    {
        char c[2] = {substr, 0};
        return count(c);
    }
    
    int string::count(const char* substr) const
    {
        int c = 0;
        hstr tmp(stdstr::c_str());
        for (int i = 0; i < size(); ++i)
        {
            if (tmp(i, -1).starts_with(substr))
            {
                c++;
                i += strlen(substr) - 1;
            }
        }
        return c;
    }
    
    int string::count(const string& substr) const
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
		char st[2] = {c, '\0'};
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
		string result;
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
	
	const char& string::operator[](int index) const
	{
		return stdstr::at(index);
	}
/******* CAST OPERATORS ************************************************/
	string::operator float() const
	{
		float f;
		sscanf(stdstr::c_str(), "%f", &f);
		return f;
	}
	
	string::operator int() const
	{
		int i;
		sscanf(stdstr::c_str(), "%d", &i);
		return i;
	}
	
	string::operator unsigned int() const
	{
		unsigned int i;
		sscanf(stdstr::c_str(), "%u", &i);
		return i;
	}
	
	string::operator bool() const
	{
		return (*this != "" && *this != "0" && *this != "false");
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
		string s = b;
		stdstr::append(s);
	}
	
	void string::operator+=(const char c)
	{
		char chstr[2] = {c, '\0'};
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
		const char* cstr = stdstr::c_str();
		return (strcmp(cstr, "1") == 0 && b ||
				strcmp(cstr, "0") == 0 && !b ||
				strcmp(cstr, "true") == 0 && b ||
				strcmp(cstr, "false") == 0 && !b);
	}

	bool string::operator==(const char* s) const
	{
		return (strcmp(stdstr::c_str(), s) == 0);
	}
	
	bool string::operator==(const std::string& s) const
	{
		return (strcmp(stdstr::c_str(), s.c_str()) == 0);
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
		string s(*this);
		s.append(1, c);
		return s;
	}
}
/******* GLOBAL ADDITION OPERATORS *******************************************/
hstr operator+(const char* s1, chstr s2)
{
	return (hstr(s1) + s2);
}

hstr operator+(char* s1, chstr s2)
{
	return (hstr(s1) + s2);
}

hstr operator+(char s1, chstr s2)
{
	return (hstr(s1) + s2);
}

hstr hvsprintf(const char* format, va_list args)
{
	int size = 2;
	char* c = new char[size + 1];
	int count = 0;
	for (int i = 0; i < 8; i++)
	{
		count = vsnprintf(c, size + 1, format, args);
		if (count == 0)
		{
			break;
		}
		if (count > 0)
		{
			delete c;
			c = new char[count + 1];
			vsnprintf(c, count + 1, format, args);
			break;
		}
		size *= 2;
		delete c;
		c = new char[size + 1];
	}
	hstr result(c);
	delete c;
	return result;
}


hstr hsprintf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	hstr result(hvsprintf(format, args));
	va_end(args);
	return result;
}
