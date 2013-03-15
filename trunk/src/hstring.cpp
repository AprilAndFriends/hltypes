/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @author  Domagoj Cerjan
/// @version 2.1
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
#include "hlog.h"
#include "hltypesUtil.h"
#include "hstring.h"

/*
7	U+7F		0xxxxxxx
11	U+7FF		110xxxxx	10xxxxxx
16	U+FFFF		1110xxxx	10xxxxxx	10xxxxxx
21	U+1FFFFF	11110xxx	10xxxxxx	10xxxxxx	10xxxxxx
26	U+3FFFFFF	111110xx	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx
31	U+7FFFFFFF	1111110x	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx
*/
#define _FROM_UNICODE_FAST(string, value) \
	if ((value) < 0x80) \
	{ \
		string += (char)(value); \
	} \
	else if ((value) < 0x800) \
	{ \
		string += (char)(0xC0 | ((value) >> 6)); \
		string += (char)(0x80 | ((value) & 0x3F)); \
	} \
	else if ((value) < 0x10000) \
	{ \
		string += (char)(0xE0 | ((value) >> 12)); \
		string += (char)(0x80 | (((value) >> 6) & 0x3F)); \
		string += (char)(0x80 | ((value) & 0x3F)); \
	} \
	else if ((value) < 0x200000) \
	{ \
		string += (char)(0xF0 | ((value) >> 18)); \
		string += (char)(0x80 | (((value) >> 12) & 0x3F)); \
		string += (char)(0x80 | (((value) >> 6) & 0x3F)); \
		string += (char)(0x80 | ((value) & 0x3F)); \
	} \
	else if ((value) < 0x4000000) \
	{ \
		string += (char)(0xF8 | ((value) >> 24)); \
		string += (char)(0x80 | (((value) >> 18) & 0x3F)); \
		string += (char)(0x80 | (((value) >> 12) & 0x3F)); \
		string += (char)(0x80 | (((value) >> 6) & 0x3F)); \
		string += (char)(0x80 | ((value) & 0x3F)); \
	} \
	else if ((value) < 0x80000000) \
	{ \
		string += (char)(0xFC | ((value) >> 30)); \
		string += (char)(0x80 | (((value) >> 24) & 0x3F)); \
		string += (char)(0x80 | (((value) >> 18) & 0x3F)); \
		string += (char)(0x80 | (((value) >> 12) & 0x3F)); \
		string += (char)(0x80 | (((value) >> 6) & 0x3F)); \
		string += (char)(0x80 | ((value) & 0x3F)); \
	}
#define _TO_UNICODE_FAST(result, str, i, size) \
	if (str[i] < 0x80) \
	{ \
		result = str[i]; \
		size = 1; \
	} \
	else if ((str[i] & 0xE0) == 0xC0) \
	{ \
		result = ((str[i] & 0x1F) << 6) | (str[i + 1] & 0x3F); \
		size = 2; \
	} \
	else if ((str[i] & 0xF0) == 0xE0) \
	{ \
		result = ((((str[i] & 0xF) << 6) | (str[i + 1] & 0x3F) ) << 6) | (str[i + 2] & 0x3F); \
		size = 3; \
	} \
	else \
	{ \
		result = ((((((str[i] & 0x7) << 6) | (str[i + 1] & 0x3F)) << 6) | (str[i + 2] & 0x3F)) << 6) | (str[i + 3] & 0x3F); \
		size = 4; \
	}

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

	Array<String> String::split(const char* delimiter, unsigned int n, bool remove_empty) const
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
		if (remove_empty)
		{
			out.remove_all("");
		}
		return out;
	}
	
	Array<String> String::split(const char delimiter, unsigned int n, bool remove_empty) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->split(sp, n, remove_empty);
	}

	Array<String> String::split(const String& delimiter, unsigned int n, bool remove_empty) const
	{
		return this->split(delimiter.c_str(), n, remove_empty);
	}
	
	Array<String> String::rsplit(const char* delimiter, unsigned int n, bool remove_empty) const
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
		if (remove_empty)
		{
			out.remove_all("");
		}
		return out;
	}

	Array<String> String::rsplit(const char delimiter, unsigned int n, bool remove_empty) const
	{
		const char sp[2] = {delimiter, '\0'};
		return this->rsplit(sp, n, remove_empty);
	}

	Array<String> String::rsplit(const String& delimiter, unsigned int n, bool remove_empty) const
	{
		return this->rsplit(delimiter.c_str(), n, remove_empty);
	}
	
	bool String::split(const char* delimiter, String& out_left, String& out_right) const
	{
		int index = this->find(delimiter);
		if (index < 0)
		{
			return false;
		}
		out_left = stdstr::substr(0, index);
		out_right = stdstr::substr(index + strlen(delimiter));
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
		out_right = stdstr::substr(index + strlen(delimiter));
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
		String s(*this);
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}
	
	String String::upper() const
	{
		String s(*this);
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	String String::reverse() const
	{
		String s(*this);
		std::reverse(s.begin(), s.end());
		return s;
	}
	
	bool String::is_digit() const
	{
		return (this->size() == 1 && isdigit(stdstr::c_str()[0]));
	}
	
	bool String::is_int() const
	{
		const char* s = stdstr::c_str();
		if (s[0] == '\0')
		{
			return false;
		}
		int i = 0;
		if (s[i] == '-')
		{
			i++;
		}
		for (; s[i] != '\0'; i++)
		{
			if (!isdigit(s[i]))
			{
				return false;
			}
		}
		return true;
	}

	bool String::is_float(bool require_dot) const
	{
		const char* s = stdstr::c_str();
		if (s[0] == '\0')
		{
			return false;
		}
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
		return (!require_dot || foundDot);
	}

	bool String::is_number() const
	{
		return this->is_float(false);
	}

	bool String::is_hex() const
	{
		const char* s = stdstr::c_str();
		if (s[0] == '\0')
		{
			return false;
		}
		for (int i = 0; s[i] != '\0'; i++)
		{
			if (!isxdigit(s[i]))
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
		const char* cstr = stdstr::c_str();
		while (*cstr == c)
		{
			cstr++;
		}
		return cstr;
	}

	String String::rtrim(char c) const
	{
		int len = stdstr::size();
		if (len == 0)
		{
			return "";
		}
		const char* cstr = stdstr::c_str();
		int i = len - 1;
		while (i >= 0 && cstr[i] == c)
		{
			i--;
		}
		return stdstr::substr(0, i + 1);
	}

	String String::replace(const char* what, const char* with_what) const
	{
		const char* s = stdstr::c_str();
		const char* p;
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

	String String::substr(int start, int count) const
	{
		return stdstr::substr(start, count);
	}
	
	String String::utf8_substr(int start, int count) const
	{
		String result;
		const unsigned char* str = (const unsigned char*)stdstr::c_str();
		int start_index = 0;
		int i = 0;
		while (str[i] != 0 && start_index < start)
		{
			if (str[i] < 0x80)
			{
				i += 1;
			}
			else if ((str[i] & 0xE0) == 0xC0)
			{
				i += 2;
			}
			else if ((str[i] & 0xF0) == 0xE0)
			{
				i += 3;
			}
			else
			{
				i += 4;
			}
			start_index++;
		}
#ifdef _DEBUG
		if (str[i] == 0)
		{
			Log::warn(hltypes::logTag, "Parameter start in utf8_substr() is after end of string: " + *this);
		}
#endif
		int start_size = i;
		int size = 0;
		while (str[i] != 0 && size < count)
		{
			if (str[i] < 0x80)
			{
				i += 1;
			}
			else if ((str[i] & 0xE0) == 0xC0)
			{
				i += 2;
			}
			else if ((str[i] & 0xF0) == 0xE0)
			{
				i += 3;
			}
			else
			{
				i += 4;
			}
			size++;
		}
#ifdef _DEBUG
		if (size < count)
		{
			Log::warn(hltypes::logTag, "Parameter count in utf8_substr() is out of bounds in string: " + *this);
		}
#endif
		return String((const char*)&str[start_size], i - start_size);
	}
	
	int String::size() const
	{
		return (int)stdstr::size();
	}
	
	int String::length() const
	{
		return (int)stdstr::size();
	}

	int String::utf8_size() const
	{
		int result = 0;
		const unsigned char* str = (const unsigned char*)stdstr::c_str();
		int i = 0;
		while (str[i] != 0)
		{
			if (str[i] < 0x80)
			{
				i += 1;
			}
			else if ((str[i] & 0xE0) == 0xC0)
			{
				i += 2;
			}
			else if ((str[i] & 0xF0) == 0xE0)
			{
				i += 3;
			}
			else
			{
				i += 4;
			}
			result++;
		}
		return result;
	}
	
	int String::utf8_length() const
	{
		return this->utf8_size();
	}

	bool String::is_ascii() const
	{
		const unsigned char* str = (const unsigned char*)stdstr::c_str();
		int i = 0;
		while (str[i] != 0)
		{
			if (str[i] > 0x7F)
			{
				return false;
			}
		}
		return true;
	}

	String String::to_hex() const
	{
		String hex = "";
		for_iter (i, 0, this->size())
		{
			hex += hsprintf("%02X", stdstr::at(i));
		}
		return hex;
	}

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
		String temp = stdstr::c_str();
		for_iter (i, 0, this->size())
		{
			if (temp(i, -1).starts_with(substr))
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
		for_iter_step (i, start, start + count, step)
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
	
	String::operator float() const
	{
		float f = 0.0f;
		sscanf(stdstr::c_str(), "%f", &f);
		return f;
	}
	
	String::operator int() const
	{
		int i = 0;
		sscanf(stdstr::c_str(), "%d", &i);
		return i;
	}
	
	String::operator unsigned int() const
	{
		unsigned int i = 0;
		sscanf(stdstr::c_str(), "%u", &i);
		return i;
	}
	
	String::operator bool() const
	{
		return (*this != "" && *this != "0" && this->lower() != "false");
	}
	
	void String::operator=(const float f)
	{
		char s[64] = {'\0'};
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
		char s[64] = {'\0'};
		sprintf(s, "%d", i);
		stdstr::operator=(s);
	}
	
	void String::operator=(const unsigned int i)
	{
		char s[64] = {'\0'};
		sprintf(s, "%u", i);
		stdstr::operator=(s);
	}
	
	void String::operator=(const bool b)
	{
		stdstr::operator=(b ? "true" : "false");
	}

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
		char cstr[2] = {c, '\0'};
		stdstr::append(cstr);
	}
	
	bool String::operator==(const float f) const
	{
		return heqf((float)*this, f, (float)HL_E_TOLERANCE);
	}
	
	bool String::operator==(const int i) const
	{
		return ((int)*this == i);
	}
	
	bool String::operator==(const unsigned int i) const
	{
		return ((unsigned int)*this == i);
	}
	
	bool String::operator==(const bool b) const
	{
		const char* cstr = stdstr::c_str();
		return ((strcmp(cstr, "1")	 == 0 &&  b) ||
				(strcmp(cstr, "0")	 == 0 && !b) ||
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

	std::basic_string<unsigned int> String::u_str() const
	{
		std::basic_string<unsigned int> result;
#ifdef __APPLE__ // bugfix for apple llvm compiler, has allocation problems in std::string with unsigned int combination
		if (stdstr::size() == 0)
		{
			unsigned int ary[] = {'x', 0};
			result = ary;
			ary[0] = 0;
			result = ary;
			return result;
		}
#endif
		unsigned int code = 0;
		const unsigned char* str = (const unsigned char*)stdstr::c_str();
		int i = 0;
		int size = 0;
		while (str[i] != 0)
		{
			_TO_UNICODE_FAST(code, str, i, size);
			result += code;
			i += size;
		}
		return result;
	}

	std::basic_string<wchar_t> String::w_str() const
	{
		std::basic_string<wchar_t> result;
#ifdef __APPLE__ // bugfix for apple llvm compiler, has allocation problems in std::string with unsigned int combination
		if (stdstr::size() == 0)
		{
			unsigned int ary[] = {'x', 0};
			result = ary;
			ary[0] = 0;
			result = ary;
			return result;
		}
#endif
#ifdef _DEBUG
		static bool checked = false;
#endif
		unsigned int code = 0;
		const unsigned char* str = (const unsigned char*)stdstr::c_str();
		int i = 0;
		int size = 0;
		while (str[i] != 0)
		{
			_TO_UNICODE_FAST(code, str, i, size);
#ifdef _DEBUG
			if (!checked && code > 0xFFFF)
			{
				Log::warnf(hltypes::logTag, "String uses Unicode characters above 0xFFFF: %s", stdstr::c_str());
				checked = true;
			}
#endif
			result += code;
			i += size;
		}
		return result;
	}

	unsigned int String::first_unicode_char(int index, int* character_size) const
	{
		unsigned int result = 0;
		const unsigned char* str = (const unsigned char*)stdstr::c_str();
		int size = 0;
		_TO_UNICODE_FAST(result, str, index, size);
		if (character_size != NULL)
		{
			*character_size = size;
		}
		return result;
	}

	String String::from_unicode(unsigned int value)
	{
		String result;
		_FROM_UNICODE_FAST(result, value);
		return result;
	}

	String String::from_unicode(wchar_t value)
	{
		String result;
		unsigned int code = value;
		_FROM_UNICODE_FAST(result, code);
		return result;
	}

	String String::from_unicode(char value)
	{
		return value;
	}

	String String::from_unicode(unsigned char value)
	{
		return (char)value;
	}

	String String::from_unicode(const unsigned int* string)
	{
		String result;
		if (string != NULL)
		{
			for (int i = 0; string[i] != 0; i++)
			{
				_FROM_UNICODE_FAST(result, string[i]);
			}
		}
		return result;
	}

	String String::from_unicode(const wchar_t* string)
	{
		String result;
		if (string != NULL)
		{
			unsigned int code;
			for (int i = 0; string[i] != 0; i++)
			{
				code = string[i];
				_FROM_UNICODE_FAST(result, code);
			}
		}
		return result;
	}

	String String::from_unicode(const char* string)
	{
		return (string != NULL ? string : "");
	}

	String String::from_unicode(const unsigned char* string)
	{
		return (string != NULL ? (const char*)string : "");
	}

	String String::from_unicode(Array<unsigned int> chars)
	{
		String result;
		foreach (unsigned int, it, chars)
		{
			_FROM_UNICODE_FAST(result, (*it));
		}
		return result;
	}

	String String::from_unicode(Array<wchar_t> chars)
	{
		String result;
		unsigned int code;
		foreach (wchar_t, it, chars)
		{
			code = (*it);
			_FROM_UNICODE_FAST(result, code);
		}
		return result;
	}

	String String::from_unicode(Array<char> chars)
	{
		String result;
		foreach (char, it, chars)
		{
			result += (*it);
		}
		return result;
	}

	String String::from_unicode(Array<unsigned char> chars)
	{
		String result;
		foreach (unsigned char, it, chars)
		{
			result += (char)(*it);
		}
		return result;
	}

}

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
	int size = 256; // safe assumption that most strings will be under 257 characters
	char* c = new char[size + 1];
	c[0] = '\0';
	int count = 0;
	int i;
	for_iterx (i, 0, 8)
	{
		count = vsnprintf(c, size, format, args);
		if (count >= 0 && count <= size)
		{
			c[count] = '\0'; // terminate string
			break;
		}
		size *= 2; // not enough characters, double current buffer
		delete [] c;
		c = new char[size + 1];
		c[0] = '\0';
	}
#ifdef _DEBUG
	if (i == 8)
	{
		delete [] c;
		throw hl_exception("Resulting string for hsprintf is longer than 2^16 (65536) characters!");
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
