/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <algorithm>
#include <cstring>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harray.h"
#include "hexception.h"
#include "hlog.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

// some platforms don't have this defined in this way
#ifndef va_copy
	#ifdef __va_copy
		#define va_copy __va_copy
	#else
		#define va_copy(dest, src) ((dest) = (src))
		#ifdef va_end
			#undef va_end
		#endif
		#define va_end(dest)
	#endif
#endif

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

// MacOS 10.9 SDK has problems linking to tolower/toupper for some reason, so this is used... sigh..
#ifdef __APPLE__
	inline int __tolower(int c)
	{
		return tolower(c);
	}

	inline int __toupper(int c)
	{
		return toupper(c);
	}
#else
	#define __tolower tolower
	#define __toupper toupper
#endif

#define MIN_STRING_CAPACITY 16

namespace hltypes
{
	String::String() : capacity(MIN_STRING_CAPACITY)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->data[0] = '\0';
	}

	String::String(const char c) : capacity(MIN_STRING_CAPACITY)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(c);
	}

	String::String(const char c, const int times) : capacity(times + 1)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(c, times);
	}

	String::String(const char* string) : capacity((int)strlen(string))
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(string);
	}

	String::String(const String& string) : capacity(string.capacity)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(string);
	}

	String::String(const char* string, const int length) : capacity(length + 1)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(string, length);
	}

	String::String(const String& string, const int length) : capacity(length + 1)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(string, length);
	}

	String::String(const short s) : capacity(MIN_STRING_CAPACITY)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(s);
	}

	String::String(const unsigned short s) : capacity(MIN_STRING_CAPACITY)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(s);
	}

	String::String(const int i) : capacity(MIN_STRING_CAPACITY)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(i);
	}

	String::String(const unsigned int i) : capacity(MIN_STRING_CAPACITY)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(i);
	}

	String::String(const int64_t i) : capacity(MIN_STRING_CAPACITY * 2)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(i);
	}

	String::String(const uint64_t i) : capacity(MIN_STRING_CAPACITY * 2)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(i);
	}

	String::String(const float f) : capacity(MIN_STRING_CAPACITY)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(f);
	}

	String::String(const float f, int precision) : capacity(precision + 2)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(f, precision);
	}

	String::String(const double d) : capacity(MIN_STRING_CAPACITY * 2)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(d);
	}

	String::String(const double d, int precision) : capacity(precision + 2)
	{
		this->data = (char*)malloc((int)this->capacity);
		this->set(d, precision);
	}

	String::~String()
	{
		free(this->data);
	}

	void String::set(const char c)
	{
		if (this->_tryIncreaseCapacity(2))
		{
			this->data[0] = c;
			this->data[1] = '\0';
		}
	}

	void String::set(const char c, const int times)
	{
		if (this->_tryIncreaseCapacity(times + 1))
		{
			memset(this->data, c, times);
			this->data[times] = '\0';
		}
	}

	void String::set(char* string)
	{
		this->set((const char*)string);
	}

	void String::set(char* string, const int length)
	{
		this->set((const char*)string, length);
	}

	void String::set(const char* string)
	{
		int size = (int)strlen(string);
		if (this->_tryIncreaseCapacity(size + 1))
		{
			memcpy(this->data, string, size + 1);
		}
	}

	void String::set(const char* string, const int length)
	{
		int size = hmin((int)strlen(string), length);
		if (this->_tryIncreaseCapacity(size + 1))
		{
			memcpy(this->data, string, size);
			this->data[size] = '\0';
		}
	}

	void String::set(const String& string)
	{
		this->set(string.data);
	}

	void String::set(const String& string, const int length)
	{
		this->set(string.data, length);
	}

	void String::set(const short s)
	{
		char _string[64] = { '\0' };
		_platformSprintf(_string, "%hd", s);
		this->set((const char*)_string);
	}

	void String::set(const unsigned short s)
	{
		char _string[64] = { '\0' };
		_platformSprintf(_string, "%hu", s);
		this->set((const char*)_string);
	}

	void String::set(const int i)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%d", i);
		this->set((const char*)_string);
	}

	void String::set(const unsigned int i)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%u", i);
		this->set((const char*)_string);
	}

	void String::set(const int64_t i)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%lld", i);
		this->set((const char*)_string);
	}

	void String::set(const uint64_t i)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%llu", i);
		this->set((const char*)_string);
	}

	void String::set(const float f)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%f", f);
		this->set((const char*)_string);
	}

	void String::set(const float f, int precision)
	{
		char _format[16] = { '\0' };
		char _string[16] = { '\0' };
		_platformSprintf(_format, "%%.%df", precision);
		_platformSprintf(_string, _format, f);
		this->set((const char*)_string);
	}

	void String::set(const double d)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%lf", d);
		this->set((const char*)_string);
	}

	void String::set(const double d, int precision)
	{
		char _format[16] = { '\0' };
		char _string[16] = { '\0' };
		_platformSprintf(_format, "%%.%dlf", precision);
		_platformSprintf(_string, _format, d);
		this->set((const char*)_string);
	}

	void String::set(const bool b)
	{
		this->set(b ? "true" : "false");
	}

	void String::add(const char c)
	{
		int size = (int)strlen(this->data);
		if (this->_tryIncreaseCapacity(size + 1))
		{
			this->data[size] = c;
			this->data[size + 1] = '\0';
		}
	}

	void String::add(const char c, const int times)
	{
		int size = (int)strlen(this->data);
		if (this->_tryIncreaseCapacity(size + times))
		{
			memset(this->data + size, c, times);
			this->data[size + times] = '\0';
		}
	}

	void String::add(char* string)
	{
		this->add((const char*)string);
	}

	void String::add(char* string, const int length)
	{
		this->add((const char*)string, length);
	}

	void String::add(const char* string)
	{
		int size = (int)strlen(this->data);
		int addedSize = (int)strlen(string);
		if (this->_tryIncreaseCapacity(size + addedSize + 1))
		{
			memcpy(this->data + size, string, addedSize + 1);
		}
	}

	void String::add(const char* string, const int length)
	{
		int size = (int)strlen(this->data);
		if (this->_tryIncreaseCapacity(size + length + 1))
		{
			memcpy(this->data + size, string, length);
			this->data[size + length] = '\0';
		}
	}

	void String::add(const String& string)
	{
		this->add(string.data);
	}

	void String::add(const String& string, const int length)
	{
		this->add(string.data, length);
	}

	void String::add(const short s)
	{
		char _string[64] = { '\0' };
		_platformSprintf(_string, "%hd", s);
		this->add((const char*)_string);
	}

	void String::add(const unsigned short s)
	{
		char _string[64] = { '\0' };
		_platformSprintf(_string, "%hu", s);
		this->add((const char*)_string);
	}

	void String::add(const int i)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%d", i);
		this->add((const char*)_string);
	}

	void String::add(const unsigned int i)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%u", i);
		this->add((const char*)_string);
	}

	void String::add(const int64_t i)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%lld", i);
		this->add((const char*)_string);
	}

	void String::add(const uint64_t i)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%llu", i);
		this->add((const char*)_string);
	}

	void String::add(const float f)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%f", f);
		this->add((const char*)_string);
	}

	void String::add(const float f, int precision)
	{
		char _format[16] = { '\0' };
		char _string[16] = { '\0' };
		_platformSprintf(_format, "%%.%df", precision);
		_platformSprintf(_string, _format, f);
		this->add((const char*)_string);
	}

	void String::add(const double d)
	{
		char _string[16] = { '\0' };
		_platformSprintf(_string, "%lf", d);
		this->add((const char*)_string);
	}

	void String::add(const double d, int precision)
	{
		char _format[16] = { '\0' };
		char _string[16] = { '\0' };
		_platformSprintf(_format, "%%.%dlf", precision);
		_platformSprintf(_string, _format, d);
		this->add((const char*)_string);
	}

	String String::lowered() const
	{
		std::ustring characters = this->uStr();
		int size = (int)characters.size();
		for_iter (i, 0, size)
		{
			characters[i] = __tolower(characters[i]);
		}
		return String::fromUnicode(characters.c_str());
	}

	String String::uppered() const
	{
		std::ustring characters = this->uStr();
		int size = (int)characters.size();
		for_iter (i, 0, size)
		{
			characters[i] = __toupper(characters[i]);
		}
		return String::fromUnicode(characters.c_str());
	}

	String String::reversed() const
	{
		int size = (int)strlen(this->data);
		char* characters = new char[size + 1];
		characters[size] = '\0';
		for_iter (i, 0, size)
		{
			characters[i] = this->data[size - 1 - i];
		}
		String result(characters);
		delete[] characters;
		return result;
	}

	String String::utf8Reversed() const
	{
		std::ustring characters = this->uStr();
		std::reverse(characters.begin(), characters.end());
		return String::fromUnicode(characters.c_str());
	}

	String String::trimmed(const char c) const
	{
		return this->trimmedLeft(c).trimmedRight(c);
	}

	String String::trimmedLeft(const char c) const
	{
		const char* string = this->data;
		while (*string == c)
		{
			++string;
		}
		return string;
	}

	String String::trimmedRight(const char c) const
	{
		int size = (int)strlen(this->data);
		if (size == 0)
		{
			return "";
		}
		int i = size - 1;
		while (i >= 0 && this->data[i] == c)
		{
			--i;
		}
		return String(this->data, i + 1);
	}

	void String::replace(const String& what, const String& withWhat)
	{
		this->replace(what.data, withWhat.data);
	}

	void String::replace(const String& what, const String& withWhat, int times)
	{
		this->replace(what.data, withWhat.data, times);
	}

	void String::replace(const String& what, const char* withWhat)
	{
		this->replace(what.data, withWhat);
	}

	void String::replace(const String& what, const char* withWhat, int times)
	{
		this->replace(what.data, withWhat, times);
	}

	void String::replace(const String& what, const char withWhat, int times)
	{
		this->replace(what.data, withWhat, times);
	}

	void String::replace(const char* what, const String& withWhat)
	{
		this->replace(what, withWhat.data);
	}

	void String::replace(const char* what, const String& withWhat, int times)
	{
		this->replace(what, withWhat.data, times);
	}

	void String::replace(const char* what, const char* withWhat)
	{
		int whatSize = (int)strlen(what);
		if (whatSize <= 0)
		{
			Log::warn(logTag, "Cannot replace empty string in string: " + *this);
			return;
		}
		int withWhatSize = (int)strlen(withWhat);
		int times = 0;
		char* pointer = this->data;
		while (true)
		{
			pointer = strstr(pointer, what);
			if (pointer == NULL)
			{
				break;
			}
			pointer += whatSize;
			++times;
		}
		this->replace(what, withWhat, times);
	}

	void String::replace(const char* what, const char* withWhat, int times)
	{
		if (times <= 0)
		{
			return;
		}
		int whatSize = (int)strlen(what);
		if (whatSize <= 0)
		{
			Log::warn(logTag, "Cannot replace empty string in string: " + *this);
			return;
		}
		int withWhatSize = (int)strlen(withWhat);
		char* found = this->data;
		char* offsetData = NULL;
		int initialTimes = times;
		int size = (int)strlen(this->data);
		if (whatSize < withWhatSize)
		{
			int maxSizeAdded = times * (withWhatSize - whatSize);
			if (this->_tryIncreaseCapacity(size + maxSizeAdded + 1))
			{
				char* oldData = NULL;
				while (times > 0)
				{
					found = strstr(found, what);
					if (found == NULL)
					{
						break;
					}
					oldData = found + whatSize;
					offsetData = found + withWhatSize;
					memmove(offsetData, oldData, strlen(oldData) + 1);
					memcpy(found, withWhat, withWhatSize * sizeof(char));
					found += withWhatSize;
					--times;
				}
				this->data[size + (initialTimes - times) * (withWhatSize - whatSize)] = '\0';
			}
		}
		else if (whatSize > withWhatSize)
		{
			while (times > 0)
			{
				found = strstr(found, what);
				if (found == NULL)
				{
					break;
				}
				offsetData = found + whatSize;
				memcpy(found, withWhat, withWhatSize * sizeof(char));
				found += withWhatSize;
				memmove(found, offsetData, strlen(offsetData) + 1);
				--times;
			}
		}
		else if (whatSize == withWhatSize)
		{
			while (times > 0)
			{
				found = strstr(found, what);
				if (found == NULL)
				{
					break;
				}
				memcpy(found, withWhat, withWhatSize * sizeof(char));
				found += withWhatSize;
				--times;
			}
		}
	}

	void String::replace(const char* what, const char withWhat, int times)
	{
		char* data = new char[times + 1];
		data[times] = '\0';
		memset(data, withWhat, times);
		this->replace(what, data);
		delete[] data;
	}

	void String::replace(const char what, const String& withWhat)
	{
		const char string[2] = { what, '\0' };
		return this->replace(string, withWhat.data);
	}

	void String::replace(const char what, const char* withWhat)
	{
		const char string[2] = { what, '\0' };
		return this->replace(string, withWhat);
	}

	void String::replace(const char what, const char withWhat, int times)
	{
		const char string[2] = { what, '\0' };
		return this->replace(string, withWhat, times);
	}

	void String::replace(int position, int count, const String& string)
	{
		this->replace(position, count, string.data);
	}

	void String::replace(int position, int count, const char* string)
	{
		if (count <= 0)
		{
			Log::warn(logTag, "Cannot replace empty string in string: " + *this);
			return;
		}
		int size = (int)strlen(this->data);
		if (position >= size)
		{
			Log::warnf(logTag, "Cannot replace substring at %d, it's out of bounds: %s", position, this->data);
			return;
		}
		count = hmin(count, size - position);
		int withWhatSize = (int)strlen(string);
		char* found = this->data + position;
		if (count < withWhatSize)
		{
			int maxSizeAdded = withWhatSize - count;
			if (this->_tryIncreaseCapacity(size + maxSizeAdded + 1))
			{
				char* oldData = found + count;
				memmove(found + withWhatSize, oldData, strlen(oldData) + 1);
				memcpy(found, string, withWhatSize * sizeof(char));
				this->data[size + maxSizeAdded] = '\0';
			}
		}
		else if (count > withWhatSize)
		{
			char* offsetData = found + count;
			memcpy(found, string, withWhatSize * sizeof(char));
			memmove(found + withWhatSize, offsetData, strlen(offsetData) + 1);
		}
		else if (count == withWhatSize)
		{
			memcpy(found, string, withWhatSize * sizeof(char));
		}
	}

	void String::replace(int position, int count, const char character, int times)
	{
		char* data = new char[times + 1];
		data[times] = '\0';
		memset(data, character, times);
		this->replace(position, count, data);
		delete[] data;
	}

	String String::replaced(const String& what, const String& withWhat) const
	{
		String result(*this);
		result.replace(what, withWhat);
		return result;
	}

	String String::replaced(const String& what, const char* withWhat) const
	{
		String result(*this);
		result.replace(what, withWhat);
		return result;
	}

	String String::replaced(const String& what, const char withWhat, int times) const
	{
		String result(*this);
		result.replace(what, withWhat, times);
		return result;
	}

	String String::replaced(const char* what, const String& withWhat) const
	{
		String result(*this);
		result.replace(what, withWhat);
		return result;
	}

	String String::replaced(const char* what, const char* withWhat) const
	{
		String result(*this);
		result.replace(what, withWhat);
		return result;
	}

	String String::replaced(const char* what, const char withWhat, int times) const
	{
		String result(*this);
		result.replace(what, withWhat, times);
		return result;
	}

	String String::replaced(const char what, const String& withWhat) const
	{
		String result(*this);
		result.replace(what, withWhat);
		return result;
	}

	String String::replaced(const char what, const char* withWhat) const
	{
		String result(*this);
		result.replace(what, withWhat);
		return result;
	}

	String String::replaced(const char what, const char withWhat, int times) const
	{
		String result(*this);
		result.replace(what, withWhat, times);
		return result;
	}

	String String::replaced(int position, int count, const String& string) const
	{
		String result(*this);
		result.replace(position, count, string);
		return result;
	}

	String String::replaced(int position, int count, const char* string) const
	{
		String result(*this);
		result.replace(position, count, string);
		return result;
	}

	String String::replaced(int position, int count, const char character, int times) const
	{
		String result(*this);
		result.replace(position, count, character, times);
		return result;
	}

	void String::insertAt(int position, const String& string)
	{
		this->insertAt(position, string.data);
	}

	void String::insertAt(int position, const char* string)
	{
		int size = (int)strlen(this->data);
		if (position > size)
		{
			Log::warnf(logTag, "Cannot replace substring at %d, it's out of bounds: %s", position, this->data);
			return;
		}
		int withWhatSize = (int)strlen(string);
		if (withWhatSize <= 0)
		{
			return;
		}
		if (this->_tryIncreaseCapacity(size + withWhatSize + 1))
		{
			char* found = this->data + position;
			memmove(found + withWhatSize, found, strlen(found) + 1);
			memcpy(found, string, withWhatSize * sizeof(char));
			this->data[size + withWhatSize] = '\0';
		}
	}

	void String::insertAt(int position, const char character, int times)
	{
		char* data = new char[times + 1];
		data[times] = '\0';
		memset(data, character, times);
		this->insertAt(position, data);
		delete[] data;
	}

	String String::insertedAt(int position, const String& string) const
	{
		String result(*this);
		result.insertAt(position, string);
		return result;
	}

	String String::insertedAt(int position, const char* string) const
	{
		String result(*this);
		result.insertAt(position, string);
		return result;
	}

	String String::insertedAt(int position, const char character, int times) const
	{
		String result(*this);
		result.insertAt(position, character, times);
		return result;
	}

	void String::randomize()
	{
		int size = (int)strlen(this->data);
		char* characters = new char[size];
		Array<int> indices = Array<char>('\0', size).indices();
		indices.randomize();
		int indicesSize = indices.size();
		for_iter (i, 0, indicesSize)
		{
			characters[i] = this->data[indices[i]];
		}
		memcpy(this->data, characters, size * sizeof(char));
		delete[] characters;
	}

	void String::utf8Randomize()
	{
		std::ustring characters = this->uStr();
		std::random_shuffle(characters.begin(), characters.end());
		this->set(String::fromUnicode(characters.c_str()));
	}

	String String::randomized() const
	{
		String result(*this);
		result.randomize();
		return result;
	}

	String String::utf8Randomized() const
	{
		String result(*this);
		result.utf8Randomize();
		return result;
	}

	Array<char> String::split() const
	{
		return Array<char>(this->data, (int)strlen(this->data));
	}
	
	Array<String> String::split(const char* delimiter, unsigned int n, bool removeEmpty) const
	{
		Array<String> out;
		const char* s = this->data;
		const char* p = NULL;
		int delimiterLength = (int)strlen(delimiter);
		while ((p = strstr(s, delimiter)) != 0 && n > 0)
		{
			out += String(s, (int)(p - s));
			s = p + delimiterLength;
			--n;
		}
		out += String(s);
		if (removeEmpty)
		{
			out.removeAll("");
		}
		return out;
	}
	
	Array<String> String::split(const char delimiter, unsigned int n, bool removeEmpty) const
	{
		const char string[2] = { delimiter, '\0'};
		return this->split(string, n, removeEmpty);
	}

	Array<String> String::split(const String& delimiter, unsigned int n, bool removeEmpty) const
	{
		return this->split(delimiter.data, n, removeEmpty);
	}

	bool String::split(const char* delimiter, String& outLeft, String& outRight) const
	{
		int index = this->indexOf(delimiter);
		if (index < 0)
		{
			return false;
		}
		outLeft = String(this->data, index);
		outRight = String(this->data + (index + strlen(delimiter)));
		return true;
	}
	
	bool String::split(const char delimiter, String& outLeft, String& outRight) const
	{
		const char string[2] = { delimiter, '\0'};
		return this->split(string, outLeft, outRight);
	}

	bool String::split(const String& delimiter, String& outLeft, String& outRight) const
	{
		return this->split(delimiter.data, outLeft, outRight);
	}

	Array<String> String::rsplit(const char* delimiter, unsigned int n, bool removeEmpty) const
	{
		Array<String> out;
		const char* s = this->data;
		const char* p = NULL;
		int delimiter_len = (int)strlen(delimiter);
		for (p = s + strlen(s) - 1; p != s && n > 0; --p)
		{
			if (strncmp(p, delimiter, delimiter_len) == 0)
			{
				--n;
			}
		}
		if (s != p)
		{
			out += String(s, (int)(p - s + 1));
			s = p + 1 + delimiter_len;
		}
		while ((p = strstr(s, delimiter)) != 0)
		{
			out += String(s, (int)(p - s));
			s = p + delimiter_len;
		}
		out += String(s);
		if (removeEmpty)
		{
			out.removeAll("");
		}
		return out;
	}

	Array<String> String::rsplit(const char delimiter, unsigned int n, bool removeEmpty) const
	{
		const char string[2] = { delimiter, '\0' };
		return this->rsplit(string, n, removeEmpty);
	}

	Array<String> String::rsplit(const String& delimiter, unsigned int n, bool removeEmpty) const
	{
		return this->rsplit(delimiter.data, n, removeEmpty);
	}

	bool String::rsplit(const char* delimiter, String& outLeft, String& outRight) const
	{
		int index = this->rindexOf(delimiter);
		if (index < 0)
		{
			return false;
		}
		outLeft = String(this->data, index);
		outRight = String(this->data + (index + strlen(delimiter)));
		return true;
	}

	bool String::rsplit(const char delimiter, String& outLeft, String& outRight) const
	{
		int index = this->rindexOf(delimiter);
		if (index < 0)
		{
			return false;
		}
		outLeft = String(this->data, index);
		outRight = String(this->data + (index + 1));
		return true;
	}

	bool String::rsplit(const String& delimiter, String& outLeft, String& outRight) const
	{
		return this->rsplit(delimiter.data, outLeft, outRight);
	}

	int String::indexOf(const char c, int index) const
	{
		if (index < this->size())
		{
			const char* found = strchr(&this->data[index], c);
			if (found != NULL)
			{
				return (found - this->data);
			}
		}
		return -1;
	}

	int String::indexOf(const char* string, int index) const
	{
		if (index < this->size())
		{
			const char* found = strstr(&this->data[index], string);
			if (found != NULL)
			{
				return (found - this->data);
			}
		}
		return -1;
	}

	int String::indexOf(const String& string, int index) const
	{
		return this->indexOf(string.data, index);
	}

	int String::rindexOf(const char c, int index) const
	{
		if (index < this->size())
		{
			const char* found = strrchr(&this->data[index], c);
			if (found != NULL)
			{
				return (found - this->data);
			}
		}
		return -1;
	}

	int String::rindexOf(const char* string, int index) const
	{
		int result = this->indexOf(string[0], index);
		if (result >= 0)
		{
			result -= index;
			int size = this->size() + 1 - index;
			char* data = new char[size];
			memcpy(data, &this->data[index], size);
			int stringSize = strlen(string);
			const char* found = NULL;
			while (true)
			{
				if (memcmp(&data[result], string, stringSize) == 0)
				{
					break;
				}
				data[result] = '\0';
				found = strrchr(data, string[0]);
				if (found == NULL)
				{
					result = -1;
					break;
				}
				result = (found - data);
			}
			delete[] data;
			if (result >= 0)
			{
				result += index;
			}
		}
		return result;
	}

	int String::rindexOf(const String& string, int index) const
	{
		return this->rindexOf(string.data, index);
	}

	int String::indexOfAny(const char* string, int index) const
	{
		int stringSize = (int)strlen(string);
		int result = -1;
		for_iter (i, 0, stringSize)
		{
			result = this->indexOf(string[i], index);
			if (result >= 0)
			{
				return result;
			}
		}
		return -1;
	}

	int String::indexOfAny(const String& string, int index) const
	{
		return this->indexOfAny(string.data, index);
	}

	int String::rindexOfAny(const char* string, int index) const
	{
		int stringSize = (int)strlen(string);
		int result = -1;
		for_iter (i, 0, stringSize)
		{
			result = this->rindexOf(string[i], index);
			if (result >= 0)
			{
				return result;
			}
		}
		return -1;
	}

	int String::rindexOfAny(const String& string, int index) const
	{
		return this->rindexOfAny(string.data, index);
	}

	int String::count(const char c) const
	{
		const char string[2] = { c, '\0' };
		return this->count(string);
	}

	int String::count(const char* string) const
	{
		int result = 0;
		char* pointer = this->data;
		int stringSize = (int)strlen(string);
		while (true)
		{
			pointer = strstr(pointer, string);
			if (pointer == NULL)
			{
				break;
			}
			pointer += stringSize;
			++result;
		}
		return result;
	}

	int String::count(const String& string) const
	{
		return this->count(string.data);
	}

	bool String::startsWith(const char* string) const
	{
		return (strncmp(this->data, string, strlen(string)) == 0);
	}

	bool String::startsWith(const String& string) const
	{
		return this->startsWith(string.data);
	}

	bool String::endsWith(const char* string) const
	{
		int size = (int)strlen(this->data);
		int stringSize = (int)strlen(string);
		if (stringSize > size)
		{
			return false;
		}
		return (strcmp(this->data + size - stringSize, string) == 0);
	}

	bool String::endsWith(const String& string) const
	{
		return this->endsWith(string.data);
	}

	bool String::contains(const char c) const
	{
		return (strchr(this->data, c) != NULL);
	}

	bool String::contains(const char* string) const
	{
		return (strstr(this->data, string) != NULL);
	}

	bool String::contains(const String& string) const
	{
		return this->contains(string.data);
	}

	bool String::containsAny(const char* string) const
	{
		int stringSize = (int)strlen(string);
		for_iter (i, 0, stringSize)
		{
			if (strchr(this->data, string[i]) != NULL)
			{
				return true;
			}
		}
		return false;
	}

	bool String::containsAny(const String& string) const
	{
		return this->containsAny(string.data);
	}

	bool String::containsAll(const char* string) const
	{
		int stringSize = (int)strlen(string);
		for_iter (i, 0, stringSize)
		{
			if (strchr(this->data, string[i]) == NULL)
			{
				return false;
			}
		}
		return true;
	}

	bool String::containsAll(const String& string) const
	{
		return this->containsAll(string.data);
	}

	bool String::isDigit() const
	{
		return ((int)strlen(this->data) == 1 && isdigit(this->data[0]));
	}
	
	bool String::isInt() const
	{
		if (this->data[0] == '\0')
		{
			return false;
		}
		int i = 0;
		if (this->data[i] == '-')
		{
			++i;
		}
		while (this->data[i] != '\0')
		{
			if (!isdigit(this->data[i]))
			{
				return false;
			}
			++i;
		}
		return true;
	}

	bool String::isFloat(bool requireDot) const
	{
		if (this->data[0] == '\0')
		{
			return false;
		}
		bool foundDot = false;
		int i = 0;
		if (this->data[i] == '-')
		{
			++i;
		}
		while (this->data[i] != '\0')
		{
			if (this->data[i] == '.')
			{
				if (foundDot)
				{
					return false;
				}
				foundDot = true;
			}
			else if (!isdigit(this->data[i]))
			{
				return false;
			}
			++i;
		}
		return (!requireDot || foundDot);
	}

	bool String::isNumber() const
	{
		return this->isFloat(false);
	}

	bool String::isHex() const
	{
		if (this->data[0] == '\0')
		{
			return false;
		}
		for (int i = 0; this->data[i] != '\0'; ++i)
		{
			if (!isxdigit(this->data[i]))
			{
				return false;
			}
		}
		return true;
	}

	bool String::isAscii() const
	{
		const unsigned char* string = (const unsigned char*)this->data;
		int i = 0;
		while (string[i] != 0)
		{
			if (string[i] > 0x7F)
			{
				return false;
			}
			++i;
		}
		return true;
	}

	String String::subString(int start, int count) const
	{
		int size = (int)strlen(this->data);
		if (count <= 0 || start >= size)
		{
			return "";
		}
		return String(this->data + start, hmin(count, size - start));
	}

	String String::utf8SubString(int start, int count) const
	{
		const unsigned char* string = (const unsigned char*)this->data;
		int startIndex = 0;
		int i = 0;
		while (string[i] != 0 && startIndex < start)
		{
			if (string[i] < 0x80)
			{
				i += 1;
			}
			else if ((string[i] & 0xE0) == 0xC0)
			{
				i += 2;
			}
			else if ((string[i] & 0xF0) == 0xE0)
			{
				i += 3;
			}
			else
			{
				i += 4;
			}
			++startIndex;
		}
#ifdef _DEBUG
		if (string[i] == 0 && count > 0)
		{
			Log::warn(logTag, "Parameter 'start' in utf8SubString() is after end of string: " + *this);
		}
#endif
		int startSize = i;
		int size = 0;
		while (string[i] != 0 && size < count)
		{
			if (string[i] < 0x80)
			{
				i += 1;
			}
			else if ((string[i] & 0xE0) == 0xC0)
			{
				i += 2;
			}
			else if ((string[i] & 0xF0) == 0xE0)
			{
				i += 3;
			}
			else
			{
				i += 4;
			}
			++size;
		}
#ifdef _DEBUG
		if (size < count)
		{
			Log::warn(logTag, "Parameter 'count' in utf8SubString() is out of bounds in string: " + *this);
		}
#endif
		return String((char*)&string[startSize], i - startSize);
	}
	
	int String::size() const
	{
		return (int)strlen(this->data);
	}
	
	int String::utf8Size() const
	{
		int result = 0;
		const unsigned char* string = (const unsigned char*)this->data;
		int i = 0;
		while (string[i] != 0)
		{
			if (string[i] < 0x80)
			{
				i += 1;
			}
			else if ((string[i] & 0xE0) == 0xC0)
			{
				i += 2;
			}
			else if ((string[i] & 0xF0) == 0xE0)
			{
				i += 3;
			}
			else
			{
				i += 4;
			}
			++result;
		}
		return result;
	}
	
	String String::toHex() const
	{
		String hex;
		int size = this->size();
		for_iter (i, 0, size)
		{
			hex += hsprintf("%02X", this->data[i]);
		}
		return hex;
	}

	unsigned int String::unhex() const
	{
		unsigned int i = 0;
		if (this->isHex())
		{
			sscanf(this->uppered().data, "%X", &i);
		}
		return i;
	}

	String String::operator()(int start, int count) const
	{
		if (count < 0)
		{
			count = (int)strlen(this->data) + count + 1;
		}
		return this->subString(start, count);
	}
	
	String String::operator()(int start, int count, int step) const
	{
		if (count < 0)
		{
			count = (int)strlen(this->data) + count + 1;
		}
		if (step == 1)
		{
			return this->subString(start, count);
		}
		String result;
		for_iter_step (i, start, start + count, step)
		{
			result += this->data[i];
		}
		return result;
	}
	
	String String::operator()(int index) const
	{
		return this->data[index];
	}
	
	char& String::operator[](int index)
	{
		return this->data[index];
	}
	
	const char& String::operator[](int index) const
	{
		return this->data[index];
	}
	
	String::operator short() const
	{
		short s = 0;
		sscanf(this->data, "%hd", &s);
		return s;
	}

	String::operator unsigned short() const
	{
		unsigned short s = 0;
		sscanf(this->data, "%hu", &s);
		return s;
	}

	String::operator int() const
	{
		int i = 0;
		sscanf(this->data, "%d", &i);
		return i;
	}

	String::operator unsigned int() const
	{
		unsigned int i = 0U;
		sscanf(this->data, "%u", &i);
		return i;
	}

	String::operator int64_t() const
	{
		int64_t i = 0LL;
		sscanf(this->data, "%lld", &i);
		return i;
	}

	String::operator uint64_t() const
	{
		uint64_t i = 0ULL;
		sscanf(this->data, "%llu", &i);
		return i;
	}

	String::operator float() const
	{
		float f = 0.0f;
		sscanf(this->data, "%f", &f);
		return f;
	}

	String::operator double() const
	{
		double d = 0.0;
		sscanf(this->data, "%lf", &d);
		return d;
	}

	String::operator bool() const
	{
		return (*this != "" && *this != "0" && this->lowered() != "false");
	}
	
	String String::operator=(const short s)
	{
		this->set(s);
		return *this;
	}

	String String::operator=(const unsigned short s)
	{
		this->set(s);
		return *this;
	}

	String String::operator=(const int i)
	{
		this->set(i);
		return *this;
	}

	String String::operator=(const unsigned int i)
	{
		this->set(i);
		return *this;
	}

	String String::operator=(const int64_t i)
	{
		this->set(i);
		return *this;
	}

	String String::operator=(const uint64_t i)
	{
		this->set(i);
		return *this;
	}

	String String::operator=(const float f)
	{
		this->set(f);
		return *this;
	}
	
	String String::operator=(const double d)
	{
		this->set(d);
		return *this;
	}
	
	String String::operator=(const bool b)
	{
		this->set(b ? "true" : "false");
		return *this;
	}

	String String::operator=(const char* string)
	{
		this->set(string);
		return *this;
	}

	String String::operator=(char* string)
	{
		this->set((const char*)string);
		return *this;
	}

	String String::operator=(const String& string)
	{
		this->set(string);
		return *this;
	}

	void String::operator+=(const short s)
	{
		this->add(s);
	}

	void String::operator+=(const unsigned short s)
	{
		this->add(s);
	}

	void String::operator+=(const int i)
	{
		this->add(i);
	}

	void String::operator+=(const unsigned int i)
	{
		this->add(i);
	}

	void String::operator+=(const int64_t i)
	{
		this->add(i);
	}

	void String::operator+=(const uint64_t i)
	{
		this->add(i);
	}

	void String::operator+=(const float f)
	{
		this->add(f);
	}

	void String::operator+=(const double d)
	{
		this->add(d);
	}

	void String::operator+=(const bool b)
	{
		this->add(b);
	}

	void String::operator+=(const char c)
	{
		this->add(c);
	}

	void String::operator+=(char* string)
	{
		this->add(string);
	}
	
	void String::operator+=(const char* string)
	{
		this->add(string);
	}

	void String::operator+=(const String& string)
	{
		this->add(string);
	}

	String String::operator+(const char c) const
	{
		String result(*this);
		result.add(c);
		return result;
	}

	String String::operator+(char* string) const
	{
		String result(*this);
		result.add((const char*)string);
		return result;
	}

	String String::operator+(const char* string) const
	{
		String result(*this);
		result.add(string);
		return result;
	}

	String String::operator+(const String& string) const
	{
		String result(*this);
		result.add(string.data);
		return result;
	}

	bool String::operator==(const short s) const
	{
		return ((short)*this == s);
	}

	bool String::operator==(const unsigned short s) const
	{
		return ((unsigned short)*this == s);
	}

	bool String::operator==(const int i) const
	{
		return ((int)*this == i);
	}

	bool String::operator==(const unsigned int i) const
	{
		return ((unsigned int)*this == i);
	}

	bool String::operator==(const int64_t i) const
	{
		return ((int64_t)*this == i);
	}

	bool String::operator==(const uint64_t i) const
	{
		return ((uint64_t)*this == i);
	}

	bool String::operator==(const float f) const
	{
		return heqf((float)*this, f);
	}

	bool String::operator==(const double d) const
	{
		return heqd((double)*this, d);
	}

	bool String::operator==(const bool b) const
	{
		return ((strcmp(this->data, "1") == 0 && b) ||
				(strcmp(this->data, "0") == 0 && !b) ||
				(strcmp(this->data, "true") == 0 && b) ||
				(strcmp(this->data, "false") == 0 && !b));
	}

	bool String::operator==(const char* string) const
	{
		return (strcmp(this->data, string) == 0);
	}
	
	bool String::operator==(const String& string) const
	{
		return (strcmp(this->data, string.data) == 0);
	}

	bool String::operator!=(const short s) const
	{
		return !(this->operator==(s));
	}

	bool String::operator!=(const unsigned short s) const
	{
		return !(this->operator==(s));
	}

	bool String::operator!=(const int i) const
	{
		return !(this->operator==(i));
	}

	bool String::operator!=(const unsigned int i) const
	{
		return !(this->operator==(i));
	}

	bool String::operator!=(const int64_t i) const
	{
		return !(this->operator==(i));
	}

	bool String::operator!=(const uint64_t i) const
	{
		return !(this->operator==(i));
	}

	bool String::operator!=(const float f) const
	{
		return !(this->operator==(f));
	}

	bool String::operator!=(const double d) const
	{
		return !(this->operator==(d));
	}

	bool String::operator!=(const bool b) const
	{
		return !(this->operator==(b));
	}

	bool String::operator!=(const char* string) const
	{
		return !(this->operator==(string));
	}

	bool String::operator!=(const String& string) const
	{
		return !(this->operator==(string));
	}

	bool String::operator<(const String& string) const
	{
		return (strcmp(this->data, string.data) < 0);
	}

	bool String::operator>(const String& string) const
	{
		return (strcmp(this->data, string.data) > 0);
	}

	bool String::operator<=(const String& string) const
	{
		return (strcmp(this->data, string.data) <= 0);
	}

	bool String::operator>=(const String& string) const
	{
		return (strcmp(this->data, string.data) >= 0);
	}

	const char* String::cStr() const
	{
		return this->data;
	}

	std::ustring String::uStr() const
	{
		Array<unsigned int> result;
		unsigned int code = 0;
		const unsigned char* string = (const unsigned char*)this->data;
		int i = 0;
		int size = 0;
		while (string[i] != 0)
		{
			_TO_UNICODE_FAST(code, string, i, size);
			result += code;
			i += size;
		}
		size = (int)result.size();
		return (size > 0 ? std::ustring(&result[0], size) : std::ustring());
	}

	std::wstring String::wStr() const
	{
		Array<wchar_t> result;
		unsigned int code = 0;
		const unsigned char* string = (const unsigned char*)this->data;
		int i = 0;
		int size = 0;
#ifdef _DEBUG
		bool checked = false;
#endif
		while (string[i] != 0)
		{
			_TO_UNICODE_FAST(code, string, i, size);
#ifdef _DEBUG
			if (!checked && code > 0xFFFF)
			{
				hltypes::_platformPrint(logTag, "String uses Unicode characters above 0xFFFF:", 1000);
				hltypes::_platformPrint(logTag, *this, 1000); // usually causes a stack overflow
				checked = true;
			}
#endif
			result += (wchar_t)code;
			i += size;
		}
		size = (int)result.size();
		return (size > 0 ? std::wstring(&result[0], size) : std::wstring());
	}

	unsigned int String::firstUnicodeChar(int index, int* byteCount) const
	{
		unsigned int result = 0;
		const unsigned char* string = (const unsigned char*)this->data;
		int size = 0;
		_TO_UNICODE_FAST(result, string, index, size);
		if (byteCount != NULL)
		{
			*byteCount = size;
		}
		return result;
	}

	String String::fromUnicode(unsigned int value)
	{
		String result;
		_FROM_UNICODE_FAST(result, value);
		return result;
	}

	String String::fromUnicode(wchar_t value)
	{
		String result;
		unsigned int code = value;
		_FROM_UNICODE_FAST(result, code);
		return result;
	}

	String String::fromUnicode(const char value)
	{
		return value;
	}

	String String::fromUnicode(unsigned char value)
	{
		return (char)value;
	}

	String String::fromUnicode(const unsigned int* string)
	{
		String result;
		if (string != NULL)
		{
			for (int i = 0; string[i] != 0; ++i)
			{
				_FROM_UNICODE_FAST(result, string[i]);
			}
		}
		return result;
	}

	String String::fromUnicode(const wchar_t* string)
	{
		String result;
		if (string != NULL)
		{
			unsigned int code;
			for (int i = 0; string[i] != 0; ++i)
			{
				code = string[i];
				_FROM_UNICODE_FAST(result, code);
			}
		}
		return result;
	}

	String String::fromUnicode(const char* string)
	{
		return (string != NULL ? string : "");
	}

	String String::fromUnicode(const unsigned char* string)
	{
		return (string != NULL ? (const char*)string : "");
	}

	String String::fromUnicode(Array<unsigned int> chars)
	{
		String result;
		foreach (unsigned int, it, chars)
		{
			_FROM_UNICODE_FAST(result, (*it));
		}
		return result;
	}

	String String::fromUnicode(Array<wchar_t> chars)
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

	String String::fromUnicode(Array<char> chars)
	{
		String result;
		foreach (char, it, chars)
		{
			result += (*it);
		}
		return result;
	}

	String String::fromUnicode(Array<unsigned char> chars)
	{
		String result;
		foreach (unsigned char, it, chars)
		{
			result += (char)(*it);
		}
		return result;
	}

	bool String::_tryIncreaseCapacity(int size)
	{
		if (size > this->capacity)
		{
			int newCapacity = hmax(MIN_STRING_CAPACITY, hpotCeil(size)); // not allowing less than MIN_STRING_CAPACITY bytes
			if (this->capacity != newCapacity)
			{
				char* newData = (char*)realloc(this->data, newCapacity);
				if (newData == NULL) // could not reallocate enough memory
				{
					return false;
				}
				this->data = newData;
				this->capacity = newCapacity;
			}
		}
		return true;
	}

}

hltypes::String operator+(const char* string1, const hltypes::String& string2)
{
	return (hltypes::String(string1) + string2);
}

hltypes::String operator+(char* string1, const hltypes::String& string2)
{
	return (hltypes::String(string1) + string2);
}

hltypes::String hvsprintf(const char* format, va_list args)
{
	va_list vaCopy; // need to copy args because vsnprintf delets va_args on some platforms, and if we re-call it with a larger buffer, it will fail
	int size = 256; // safe assumption that most strings will be under 257 characters
	// not using a static buffer here to assure thread safety
	char* c = new char[size + 1];
	c[0] = '\0';
	int count = 0;
	int i;
	for_iterx (i, 0, 8)
	{
		// due to different (and non-standard) behavior in different implementations, there is one safe byte
		va_copy(vaCopy, args);
		count = hltypes::_platformVsnprintf(c, size, format, vaCopy);
		va_end(vaCopy);
		if (count >= 0 && count < size)
		{
			c[count] = '\0'; // terminate string
			break;
		}
		size *= 2; // not enough characters, double current buffer
		delete[] c;
		c = new char[size + 1];
		c[0] = '\0';
	}
#ifdef _DEBUG
	if (i >= 8)
	{
		delete[] c;
		throw Exception("Resulting string for hsprintf is longer than 2^16 (65536) characters!");
	}
#endif
	hltypes::String result(c);
	delete[] c;
	return result;
}

hltypes::String hsprintf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	hltypes::String result = hvsprintf(format, args);
	va_end(args);
	return result;
}
