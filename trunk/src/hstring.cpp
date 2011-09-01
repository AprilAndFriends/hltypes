/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @author  Domagoj Cerjan
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <algorithm>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "exception.h"
#include "harray.h"
#include "hstring.h"
#include "util.h"

typedef std::basic_string<char> stdstr;

namespace hltypes
{
	String::String() : stdstr() { }
	String::String(const char c) : stdstr(1, c) { }
	String::String(const char c, const int times) : stdstr(times, c) { }
	String::String(const char* s) : stdstr(s) { }
	String::String(const String& s) : stdstr(s) { }
	String::String(const std::string& s) : stdstr(s) { }
	String::String(const char* s, const int length) : stdstr(s, length) { }
	String::String(const String& s, const int length) : stdstr(s, length) { }
	String::String(const int i) { this->operator=(i); }
	String::String(const unsigned int i) { this->operator=(i); }
	String::String(const float f) { this->operator=(f); }
	String::String(const float f, int precision)
	{
		char fmt[16], s[64];
		sprintf(fmt, "%%.%df", precision);
		sprintf(s, fmt, f);
		stdstr::operator=(s);
	}
	String::~String() { }

	Array<String> String::split(const char* delimiter, unsigned int n) const
	{
		Array<String> out;
		const char *s = stdstr::c_str(), *p;
		int delimiter_len = strlen(delimiter);
		while ((p = strstr(s, delimiter)) != 0 && n > 0)
		{
			out += String(s, p - s);
			s = p + delimiter_len;
			n--;
		}
		out += String(s);
		return out;
	}
	
	Array<String> String::split(const char delimiter, unsigned int n) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->split(sp, n);
	}

	Array<String> String::split(const String& delimiter, unsigned int n) const
	{
		return this->split(delimiter.c_str(), n);
	}
	
	Array<String> String::rsplit(const char* delimiter, unsigned int n) const
	{
		Array<String> out;
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
			out += String(s, p - s + 1);
			s = p + 1 + delimiter_len;
		}
		while ((p = strstr(s, delimiter)) != 0)
		{
			out += String(s, p - s);
			s = p + delimiter_len;
		}
		out += String(s);
		return out;
	}

	Array<String> String::rsplit(const char delimiter, unsigned int n) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->rsplit(sp, n);
	}

	Array<String> String::rsplit(const String& delimiter, unsigned int n) const
	{
		return this->rsplit(delimiter.c_str(), n);
	}
	
	bool String::split(const char* delimiter, String& out_left, String& out_right) const
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
	
	bool String::rsplit(const char* delimiter, String& out_left, String& out_right) const
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

	bool String::starts_with(const char* s) const
	{
		return (strncmp(stdstr::c_str(), s, strlen(s)) == 0);
	}

	bool String::ends_with(const char* s) const
	{
		const char* thiss = stdstr::c_str();
		int thislen = this->size(), slen = strlen(s);
		if (slen > thislen)
		{
			return false;
		}
		return (strcmp(thiss + thislen - slen, s) == 0);
	}
	
	String String::lower() const
	{
		hstr s(*this);
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}
	
	String String::upper() const
	{
		hstr s(*this);
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	String String::reverse() const
	{
		hstr s(*this);
		std::reverse(s.begin(), s.end());
		return s;
	}
	
	bool String::is_digit() const
	{
        return (this->size() == 1 && isdigit(stdstr::c_str()[0]));
	}
    
	bool String::is_number() const
	{
		const char* s = stdstr::c_str();
		bool foundDot = false;
		int i = 0;
        if (s[i] == '-')
		{
			i++;
		}
		for (; s[i] != '\0'; i++)
		{
            if (s[i] == '.')
            {
                if (foundDot)
				{
					return false;
				}
				foundDot = true;
            }
			else if (!isdigit(s[i]))
			{
				return false;
			}
		}
		return true;
	}

	String String::trim(char c) const
	{
		return this->ltrim(c).rtrim(c);
	}

	String String::ltrim(char c) const
	{
		int index = stdstr::find_first_not_of(c);
		if (index < 0)
		{
			return *this;
		}
		return stdstr::substr(index, this->size() - index);
	}

	String String::rtrim(char c) const
	{
		int index = stdstr::find_last_not_of(c);
		if (index < 0)
		{
			return *this;
		}
		return stdstr::substr(0, index + 1);
	}

	String String::replace(const char* what, const char* with_what) const
	{
		const char *s = stdstr::c_str(), *p;
		String out;
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
	
	int String::size() const
	{
		return stdstr::size();
	}
	
	int String::length() const
	{
		return stdstr::size();
	}

	wchar_t* String::w_str()
	{
		wchar_t* result = new wchar_t[stdstr::size() * 4];
		memset(result, 0, stdstr::size() * 4);
		mbstowcs(result, stdstr::c_str(), stdstr::size());
		return result;
	}

	
/******* TYPE EXTENSION FUNCTIONS **************************************/
	bool String::split(const char delimiter, String& out_left,String& out_right) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->split(sp, out_left, out_right);
	}
		
	bool String::split(const String& delimiter, String& out_left, String& out_right) const
	{
		return this->split(delimiter.c_str(), out_left, out_right);
	}
	
	bool String::rsplit(const char delimiter, String& out_left, String& out_right) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->rsplit(sp, out_left, out_right);
	}
	
	bool String::rsplit(const String& delimiter, String& out_left, String& out_right) const
	{
		return this->rsplit(delimiter.c_str(), out_left, out_right);
	}
    
    int String::count(const char substr) const
    {
        char c[2] = {substr, 0};
        return count(c);
    }
    
    int String::count(const char* substr) const
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
    
    int String::count(const String& substr) const
    {
        return count(substr.c_str());
    }

	bool String::starts_with(const String& s) const
	{
		return this->starts_with(s.c_str());
	}

	bool String::ends_with(const String& s) const
	{
		return this->ends_with(s.c_str());
	}

	String String::replace(const String& what, const char* with_what) const
	{
		return this->replace(what.c_str(), with_what);
	}
	
	String String::replace(const char* what, const String& with_what) const
	{
		return this->replace(what, with_what.c_str());
	}
	
	String String::replace(const String& what, const String& with_what) const
	{
		return this->replace(what.c_str(), with_what.c_str());
	}
	
	String String::replace(int pos1, int n1, const String& str)
	{
		return stdstr::replace(pos1, n1, str);
	}
	
	String String::replace(int pos1, int n1, const String& str, int pos2, int n2)
	{
		return stdstr::replace(pos1, n1, str, pos2, n2);
	}
	
	String String::replace(int pos1, int n1, const char* s)
	{
		return stdstr::replace(pos1, n1, s);
	}
	
	String String::replace(int pos1, int n1, const char* s, int n2)
	{
		return stdstr::replace(pos1, n1, s, n2);
	}
	
	String String::replace(int pos1, int n1, char c, int n2)
	{
		return stdstr::replace(pos1, n1, n2, c);
	}
	
	bool String::contains(const char c) const
	{
		char st[2] = {c, '\0'};
		return this->contains(st);
	}
	
	bool String::contains(const char* s) const
	{
		return (stdstr::find(s) != stdstr::npos);
	}
	
	bool String::contains(const String& s) const
	{
		return this->contains(s.c_str());
	}
/******* SUBSTR OPERATORS **********************************************/
	String String::operator()(int start, int count) const
	{
		return stdstr::substr(start, count);
	}
	
	String String::operator()(int start, int count, int step) const
	{
		if (count < 0)
		{
			count = stdstr::npos;
		}
		if (step == 1)
		{
			return stdstr::substr(start, count);
		}
		String result;
		for (int i = start; i < start + count; i += step)
		{
			result += stdstr::at(i);
		}
		return result;
	}
	
	String String::operator()(int index) const
	{
		return stdstr::at(index);
	}
	
	char& String::operator[](int index)
	{
		return stdstr::at(index);
	}
	
	const char& String::operator[](int index) const
	{
		return stdstr::at(index);
	}
/******* CAST OPERATORS ************************************************/
	String::operator float() const
	{
		float f;
		sscanf(stdstr::c_str(), "%f", &f);
		return f;
	}
	
	String::operator int() const
	{
		int i;
		sscanf(stdstr::c_str(), "%d", &i);
		return i;
	}
	
	String::operator unsigned int() const
	{
		unsigned int i;
		sscanf(stdstr::c_str(), "%u", &i);
		return i;
	}
	
	String::operator bool() const
	{
		return (*this != "" && *this != "0" && *this != "false");
	}
	
/******* ASSIGNMENT OPERATORS ******************************************/
	void String::operator=(const float f)
	{
		char s[64];
		sprintf(s, "%f", f);
		stdstr::operator=(s);
	}
	
	void String::operator=(const std::string& s)
	{
		stdstr::operator=(s);
	}
	
	void String::operator=(const char* s)
	{
		stdstr::operator=(s);
	}
	
	void String::operator=(const int i)
	{
		char s[64];
		sprintf(s, "%d", i);
		stdstr::operator=(s);
	}
	
	void String::operator=(const unsigned int i)
	{
		char s[64];
		sprintf(s, "%u", i);
		stdstr::operator=(s);
	}
	
	void String::operator=(const bool b)
	{
		stdstr::operator=(b ? "true" : "false");
	}
/******* CONCATENATION OPERATORS ***************************************/
	void String::operator+=(const float f)
	{
		String s = f;
		stdstr::append(s);
	}
	
	void String::operator+=(const std::string& s)
	{
		stdstr::append(s);
	}
	
	void String::operator+=(const char* s)
	{
		stdstr::append(s);
	}
	
	void String::operator+=(const int i)
	{
		String s = i;
		stdstr::append(s);
	}
	
	void String::operator+=(const unsigned int i)
	{
		String s = i;
		stdstr::append(s);
	}
	
	void String::operator+=(const bool b)
	{
		String s = b;
		stdstr::append(s);
	}
	
	void String::operator+=(const char c)
	{
		char chstr[2] = {c, '\0'};
		stdstr::append(chstr);
	}
	
/******* COMPARISON OPERATORS ******************************************/
	bool String::operator==(const float f) const
	{
		return (fabs(((float)*this) - f) < HL_E_TOLERANCE);
	}
	
	bool String::operator==(const int i) const
	{
		return (((int)*this) == i);
	}
	
	bool String::operator==(const unsigned int i) const
	{
		return (((unsigned int)*this) == i);
	}
	
	bool String::operator==(const bool b) const
	{
		const char* cstr = stdstr::c_str();
		return ((strcmp(cstr, "1")     == 0 &&  b) ||
				(strcmp(cstr, "0")     == 0 && !b) ||
				(strcmp(cstr, "true")  == 0 &&  b) ||
				(strcmp(cstr, "false") == 0 && !b));
	}

	bool String::operator==(const char* s) const
	{
		return (strcmp(stdstr::c_str(), s) == 0);
	}
	
	bool String::operator==(const std::string& s) const
	{
		return (strcmp(stdstr::c_str(), s.c_str()) == 0);
	}
/******* ADDITION OPERATORS ********************************************/
	String String::operator+(const char* s) const
	{
		String result(*this);
		result.append(s);
		return result;
	}
	
	String String::operator+(char* s) const
	{ 
		return this->operator+((const char*)s);
	}
	
	String String::operator+(const String& s) const
	{
		String result(*this);
		result.append(s);
		return result;
	}
	
	String String::operator+(const std::string& s) const
	{
		String result(*this);
		result.append(s);
		return result;
	}
	
	String String::operator+(const char c) const
	{
		String result(*this);
		result.append(1, c);
		return result;
	}
}
/******* GLOBAL ADDITION OPERATORS **************************************/
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
	int size = 256; // safe assumption that strings will be usually under 256 characters
	char* c = new char[size + 1];
	int count = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		count = vsnprintf(c, size + 1, format, args);
		if (count == 0)
		{
			c[0] = '\0'; // empty string
			break;
		}
		if (count > 0)
		{
			delete [] c;
			c = new char[count + 1];
			vsnprintf(c, count + 1, format, args);
			c[count] = '\0'; // terminate string with \0, safe is safe
			break;
		}
		size *= 2; // not enough characters, double current buffer
		delete [] c;
		c = new char[size + 1];
	}
#ifdef _DEBUG
	if (i == 8)
	{
		delete [] c;
		throw hl_exception("ERROR! Resulting string for hsprintf is longer than 2^16 (65536) characters!");
	}
#endif
	hstr result(c);
	delete [] c;
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
