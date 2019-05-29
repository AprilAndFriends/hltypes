/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <algorithm>
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

#define FORMAT_BUFFER_SIZE 16
#define FORMATTING_STRING_BUFFER_SIZE 64

#define BOOL_TO_STRING(b) ((b) ? "true" : "false")

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

// MacOS 10.9 SDK has problems linking to towlower/towupper for some reason, so this is used... sigh..
#ifdef __APPLE__
	inline int __towlower__(int c)
	{
		return towlower(c);
	}

	inline int __towupper__(int c)
	{
		return towupper(c);
	}
#else
	#define __towlower__ towlower
	#define __towupper__ towupper
#endif

#ifdef _IOS
	#define _MANUAL_CASE_CONVERSION
#endif

typedef std::basic_string<char> stdstr;

namespace hltypes
{
#ifdef _MANUAL_CASE_CONVERSION
#define MAX_CASE_CONVERSION_ENTRIES 128
	// iOS can't handle locale so it's impossible to convert this using towlower() or towupper()
	static std::pair<unsigned int, unsigned int> _caseConversionTable[MAX_CASE_CONVERSION_ENTRIES] =
	{
		///Cyrillic extensions - 16 signs
		std::pair<unsigned int, unsigned int>(0x0400, 0x0450),
		std::pair<unsigned int, unsigned int>(0x0401, 0x0451),
		std::pair<unsigned int, unsigned int>(0x0402, 0x0452),
		std::pair<unsigned int, unsigned int>(0x0403, 0x0453),
		std::pair<unsigned int, unsigned int>(0x0404, 0x0454),
		std::pair<unsigned int, unsigned int>(0x0405, 0x0455),
		std::pair<unsigned int, unsigned int>(0x0406, 0x0456),
		std::pair<unsigned int, unsigned int>(0x0407, 0x0457),
		std::pair<unsigned int, unsigned int>(0x0408, 0x0458),
		std::pair<unsigned int, unsigned int>(0x0409, 0x0459),
		std::pair<unsigned int, unsigned int>(0x040A, 0x045A),
		std::pair<unsigned int, unsigned int>(0x040B, 0x045B),
		std::pair<unsigned int, unsigned int>(0x040C, 0x045C),
		std::pair<unsigned int, unsigned int>(0x040D, 0x045D),
		std::pair<unsigned int, unsigned int>(0x040E, 0x045E),
		std::pair<unsigned int, unsigned int>(0x040F, 0x045F),
		///Basic Cyrillic alphabet - 32 signs
		std::pair<unsigned int, unsigned int>(0x0410, 0x0430),
		std::pair<unsigned int, unsigned int>(0x0411, 0x0431),
		std::pair<unsigned int, unsigned int>(0x0412, 0x0432),
		std::pair<unsigned int, unsigned int>(0x0413, 0x0433),
		std::pair<unsigned int, unsigned int>(0x0414, 0x0434),
		std::pair<unsigned int, unsigned int>(0x0415, 0x0435),
		std::pair<unsigned int, unsigned int>(0x0416, 0x0436),
		std::pair<unsigned int, unsigned int>(0x0417, 0x0437),
		std::pair<unsigned int, unsigned int>(0x0418, 0x0438),
		std::pair<unsigned int, unsigned int>(0x0419, 0x0439),
		std::pair<unsigned int, unsigned int>(0x041A, 0x043A),
		std::pair<unsigned int, unsigned int>(0x041B, 0x043B),
		std::pair<unsigned int, unsigned int>(0x041C, 0x043C),
		std::pair<unsigned int, unsigned int>(0x041D, 0x043D),
		std::pair<unsigned int, unsigned int>(0x041E, 0x043E),
		std::pair<unsigned int, unsigned int>(0x041F, 0x043F),
		std::pair<unsigned int, unsigned int>(0x0420, 0x0440),
		std::pair<unsigned int, unsigned int>(0x0421, 0x0441),
		std::pair<unsigned int, unsigned int>(0x0422, 0x0442),
		std::pair<unsigned int, unsigned int>(0x0423, 0x0443),
		std::pair<unsigned int, unsigned int>(0x0424, 0x0444),
		std::pair<unsigned int, unsigned int>(0x0425, 0x0445),
		std::pair<unsigned int, unsigned int>(0x0426, 0x0446),
		std::pair<unsigned int, unsigned int>(0x0427, 0x0447),
		std::pair<unsigned int, unsigned int>(0x0428, 0x0448),
		std::pair<unsigned int, unsigned int>(0x0429, 0x0449),
		std::pair<unsigned int, unsigned int>(0x042A, 0x044A),
		std::pair<unsigned int, unsigned int>(0x042B, 0x044B),
		std::pair<unsigned int, unsigned int>(0x042C, 0x044C),
		std::pair<unsigned int, unsigned int>(0x042D, 0x044D),
		std::pair<unsigned int, unsigned int>(0x042E, 0x044E),
		std::pair<unsigned int, unsigned int>(0x042F, 0x044F),
		///Historic letters - in case we need it. - 17
		std::pair<unsigned int, unsigned int>(0x0460, 0x0461),
		std::pair<unsigned int, unsigned int>(0x0462, 0x0463),
		std::pair<unsigned int, unsigned int>(0x0464, 0x0465),
		std::pair<unsigned int, unsigned int>(0x0466, 0x0467),
		std::pair<unsigned int, unsigned int>(0x0468, 0x0469),
		std::pair<unsigned int, unsigned int>(0x046A, 0x046B),
		std::pair<unsigned int, unsigned int>(0x046C, 0x046D),
		std::pair<unsigned int, unsigned int>(0x046E, 0x046F),
		std::pair<unsigned int, unsigned int>(0x0470, 0x0471),
		std::pair<unsigned int, unsigned int>(0x0472, 0x0473),
		std::pair<unsigned int, unsigned int>(0x0474, 0x0475),
		std::pair<unsigned int, unsigned int>(0x0476, 0x0477),
		std::pair<unsigned int, unsigned int>(0x0478, 0x0479),
		std::pair<unsigned int, unsigned int>(0x047A, 0x047B),
		std::pair<unsigned int, unsigned int>(0x047C, 0x047D),
		std::pair<unsigned int, unsigned int>(0x047E, 0x047F),
		std::pair<unsigned int, unsigned int>(0x0480, 0x0481),
		///Historic miscellaneous - in case we need it - 8 signs 
		///No conversion it is still here system understands the simbols
		std::pair<unsigned int, unsigned int>(0x0482, 0x0482),
		std::pair<unsigned int, unsigned int>(0x0483, 0x0483),
		std::pair<unsigned int, unsigned int>(0x0484, 0x0484),
		std::pair<unsigned int, unsigned int>(0x0485, 0x0485),
		std::pair<unsigned int, unsigned int>(0x0486, 0x0486),
		std::pair<unsigned int, unsigned int>(0x0487, 0x0487),
		std::pair<unsigned int, unsigned int>(0x0488, 0x0488),
		std::pair<unsigned int, unsigned int>(0x0489, 0x0489),
		///Extended Cyrillic  - 54 signs.
		std::pair<unsigned int, unsigned int>(0x048A, 0x048B),
		std::pair<unsigned int, unsigned int>(0x048C, 0x048D),
		std::pair<unsigned int, unsigned int>(0x048E, 0x048F),
		std::pair<unsigned int, unsigned int>(0x0490, 0x0491),
		std::pair<unsigned int, unsigned int>(0x0492, 0x0493),
		std::pair<unsigned int, unsigned int>(0x0494, 0x0495),
		std::pair<unsigned int, unsigned int>(0x0496, 0x0497),
		std::pair<unsigned int, unsigned int>(0x0498, 0x0499),
		std::pair<unsigned int, unsigned int>(0x049A, 0x049B),
		std::pair<unsigned int, unsigned int>(0x049E, 0x049F),
		std::pair<unsigned int, unsigned int>(0x04A0, 0x04A1),
		std::pair<unsigned int, unsigned int>(0x04A2, 0x04A3),
		std::pair<unsigned int, unsigned int>(0x04A4, 0x04A5),
		std::pair<unsigned int, unsigned int>(0x04A6, 0x04A7),
		std::pair<unsigned int, unsigned int>(0x04A8, 0x04A9),
		std::pair<unsigned int, unsigned int>(0x04AA, 0x04AB),
		std::pair<unsigned int, unsigned int>(0x04AC, 0x04AD),
		std::pair<unsigned int, unsigned int>(0x04AE, 0x04AF),
		std::pair<unsigned int, unsigned int>(0x04B0, 0x04B1),
		std::pair<unsigned int, unsigned int>(0x04B2, 0x04B3),
		std::pair<unsigned int, unsigned int>(0x04B4, 0x04B5),
		std::pair<unsigned int, unsigned int>(0x04B6, 0x04B7),
		std::pair<unsigned int, unsigned int>(0x04B8, 0x04B9),
		std::pair<unsigned int, unsigned int>(0x04BA, 0x04BB),
		std::pair<unsigned int, unsigned int>(0x04BC, 0x04BD),
		std::pair<unsigned int, unsigned int>(0x04BE, 0x04BF),
		std::pair<unsigned int, unsigned int>(0x04C0, 0x04CF),
		std::pair<unsigned int, unsigned int>(0x04C1, 0x04C2),
		std::pair<unsigned int, unsigned int>(0x04C3, 0x04C4),
		std::pair<unsigned int, unsigned int>(0x04C5, 0x04C6),
		std::pair<unsigned int, unsigned int>(0x04C7, 0x04C8),
		std::pair<unsigned int, unsigned int>(0x04C9, 0x04CA),
		std::pair<unsigned int, unsigned int>(0x04CB, 0x04CC),
		std::pair<unsigned int, unsigned int>(0x04CD, 0x04CE),
		std::pair<unsigned int, unsigned int>(0x04D0, 0x04D1),
		std::pair<unsigned int, unsigned int>(0x04D2, 0x04D3),
		std::pair<unsigned int, unsigned int>(0x04D4, 0x04D5),
		std::pair<unsigned int, unsigned int>(0x04D6, 0x04D7),
		std::pair<unsigned int, unsigned int>(0x04D8, 0x04D9),
		std::pair<unsigned int, unsigned int>(0x04DA, 0x04DB),
		std::pair<unsigned int, unsigned int>(0x04DC, 0x04DD),
		std::pair<unsigned int, unsigned int>(0x04DE, 0x04DF),
		std::pair<unsigned int, unsigned int>(0x04E0, 0x04E1),
		std::pair<unsigned int, unsigned int>(0x04E2, 0x04E3),
		std::pair<unsigned int, unsigned int>(0x04E4, 0x04E5),
		std::pair<unsigned int, unsigned int>(0x04E6, 0x04E7),
		std::pair<unsigned int, unsigned int>(0x04E8, 0x04E9),
		std::pair<unsigned int, unsigned int>(0x04EA, 0x04EB),
		std::pair<unsigned int, unsigned int>(0x04EC, 0x04ED),
		std::pair<unsigned int, unsigned int>(0x04EE, 0x04EF),
		std::pair<unsigned int, unsigned int>(0x04F0, 0x04F1),
		std::pair<unsigned int, unsigned int>(0x04F2, 0x04F3),
		std::pair<unsigned int, unsigned int>(0x04F4, 0x04F5),
		std::pair<unsigned int, unsigned int>(0x04F6, 0x04F7),
		std::pair<unsigned int, unsigned int>(0x04F8, 0x04F9)
	};
#endif

	String::String() :
		stdstr()
	{
	}

	String::String(const char c) :
		stdstr(1, c)
	{
	}

	String::String(const char c, const int times) :
		stdstr(times, c)
	{
	}

	String::String(const char* string) :
		stdstr(string)
	{
	}

	String::String(const String& string) :
		stdstr(string.c_str())
	{
	}

	String::String(const char* string, const int length) :
		stdstr(string, length)
	{
	}

	String::String(const String& string, const int length) :
		stdstr(string.c_str(), length)
	{
	}

	String::String(const bool b) :
		// TODObool - fix this inconsistency
		//stdstr(BOOL_TO_STRING(b))
		stdstr(b ? "1" : "0")
	{
	}

	String::String(const short s) :
		stdstr()
	{
		this->set(s);
	}

	String::String(const unsigned short s) :
		stdstr()
	{
		this->set(s);
	}

	String::String(const int i) :
		stdstr()
	{
		this->set(i);
	}

	String::String(const unsigned int i) :
		stdstr()
	{
		this->set(i);
	}

	String::String(const int64_t i) :
		stdstr()
	{
		this->set(i);
	}

	String::String(const uint64_t i) :
		stdstr()
	{
		this->set(i);
	}

	String::String(const float f) :
		stdstr()
	{
		this->set(f);
	}

	String::String(const float f, int precision) :
		stdstr()
	{
		this->set(f, precision);
	}

	String::String(const double d) :
		stdstr()
	{
		this->set(d);
	}

	String::String(const double d, int precision) :
		stdstr()
	{
		this->set(d, precision);
	}

	String::~String() // do not remove this, the superclass doesn't have a virtual destructor
	{
	}

	void String::set(const char c)
	{
		stdstr::assign(1, c);
	}

	void String::set(const char c, const int times)
	{
		stdstr::assign(times, c);
	}

	void String::set(char* string)
	{
		stdstr::assign(string);
	}

	void String::set(char* string, const int length)
	{
		stdstr::assign(string, length);
	}

	void String::set(const char* string)
	{
		stdstr::assign(string);
	}

	void String::set(const char* string, const int length)
	{
		stdstr::assign(string, length);
	}

	void String::set(const String& string)
	{
		stdstr::assign(string.c_str());
	}

	void String::set(const String& string, const int length)
	{
		stdstr::assign(string.c_str(), length);
	}

	void String::set(const bool b)
	{
		stdstr::assign(BOOL_TO_STRING(b));
	}

	void String::set(const short s)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%hd", s);
		stdstr::assign(string);
	}

	void String::set(const unsigned short s)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%hu", s);
		stdstr::assign(string);
	}

	void String::set(const int i)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%d", i);
		stdstr::assign(string);
	}

	void String::set(const unsigned int i)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%u", i);
		stdstr::assign(string);
	}

	void String::set(const int64_t i)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%lld", i);
		stdstr::assign(string);
	}

	void String::set(const uint64_t i)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%llu", i);
		stdstr::assign(string);
	}

	void String::set(const float f)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%f", f);
		stdstr::assign(String(string).trimmedRight('0').trimmedRight('.').cStr());
	}

	void String::set(const float f, int precision)
	{
		char format[FORMAT_BUFFER_SIZE] = { '\0' };
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(format, "%%.%df", precision);
		_platformSprintf(string, format, f);
		stdstr::assign(string);
	}

	void String::set(const double d)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%lf", d);
		stdstr::assign(String(string).trimmedRight('0').trimmedRight('.').cStr());
	}

	void String::set(const double d, int precision)
	{
		char format[FORMAT_BUFFER_SIZE] = { '\0' };
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(format, "%%.%dlf", precision);
		_platformSprintf(string, format, d);
		stdstr::assign(string);
	}

	void String::add(const char c)
	{
		stdstr::append(1, c);
	}

	void String::add(const char c, const int times)
	{
		stdstr::append(times, c);
	}

	void String::add(char* string)
	{
		stdstr::append(string);
	}

	void String::add(char* string, const int length)
	{
		stdstr::append(string, length);
	}

	void String::add(const char* string)
	{
		stdstr::append(string);
	}

	void String::add(const char* string, const int length)
	{
		stdstr::append(string, length);
	}

	void String::add(const String& string)
	{
		stdstr::append(string.c_str());
	}

	void String::add(const String& string, const int length)
	{
		stdstr::append(string.c_str(), length);
	}

	void String::add(const bool b)
	{
		stdstr::append(BOOL_TO_STRING(b));
	}

	void String::add(const short s)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%hd", s);
		stdstr::append(string);
	}

	void String::add(const unsigned short s)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%hu", s);
		stdstr::append(string);
	}

	void String::add(const int i)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%d", i);
		stdstr::append(string);
	}

	void String::add(const unsigned int i)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%u", i);
		stdstr::append(string);
	}

	void String::add(const int64_t i)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%lld", i);
		stdstr::append(string);
	}

	void String::add(const uint64_t i)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%llu", i);
		stdstr::append(string);
	}

	void String::add(const float f)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%f", f);
		stdstr::append(String(string).trimmedRight('0').trimmedRight('.').cStr());
	}

	void String::add(const float f, int precision)
	{
		char format[FORMAT_BUFFER_SIZE] = { '\0' };
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(format, "%%.%df", precision);
		_platformSprintf(string, format, f);
		stdstr::append(string);
	}

	void String::add(const double d)
	{
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(string, "%lf", d);
		stdstr::append(String(string).trimmedRight('0').trimmedRight('.').cStr());
	}

	void String::add(const double d, int precision)
	{
		char format[FORMAT_BUFFER_SIZE] = { '\0' };
		char string[FORMATTING_STRING_BUFFER_SIZE] = { '\0' };
		_platformSprintf(format, "%%.%dlf", precision);
		_platformSprintf(string, format, d);
		stdstr::append(string);
	}

	String String::lowered() const
	{
		std::ustring uString = this->uStr();
#ifndef _MANUAL_CASE_CONVERSION
		std::transform(uString.begin(), uString.end(), uString.begin(), __towlower__);
#else
		// iOS can't handle locale so it's impossible to convert this using towlower() or towupper()
		unsigned int value = 0;
		for_itert (unsigned int, i, 0, uString.size())
		{
			value = __towlower__(uString[i]);
			if (value == uString[i])
			{
				for_iter (j, 0, MAX_CASE_CONVERSION_ENTRIES)
				{
					if (value == _caseConversionTable[j].first)
					{
						uString[i] = _caseConversionTable[j].second;
						break;
					}
				}
			}
			else
			{
				uString[i] = value;
			}
		}
#endif
		return String::fromUnicode(uString.c_str());
	}

	String String::uppered() const
	{
		std::ustring uString = this->uStr();
#ifndef _MANUAL_CASE_CONVERSION
		std::transform(uString.begin(), uString.end(), uString.begin(), __towupper__);
#else
		// iOS can't handle locale so it's impossible to convert this using towlower() or towupper()
		unsigned int value = 0;
		for_itert (unsigned int, i, 0, uString.size())
		{
			value = __towupper__(uString[i]);
			if (value == uString[i])
			{
				for_iter (j, 0, MAX_CASE_CONVERSION_ENTRIES)
				{
					if (value == _caseConversionTable[j].second)
					{
						uString[i] = _caseConversionTable[j].first;
						break;
					}
				}
			}
			else
			{
				uString[i] = value;
			}
		}
#endif
		return String::fromUnicode(uString.c_str());
	}

	String String::reversed() const
	{
		String result(*this);
		std::reverse(result.begin(), result.end());
		return result;
	}

	String String::utf8Reversed() const
	{
		std::ustring uString = this->uStr();
		std::reverse(uString.begin(), uString.end());
		return fromUnicode(uString.c_str());
	}

	String String::trimmed(const char c) const
	{
		return this->trimmedLeft(c).trimmedRight(c);
	}

	String String::trimmedLeft(const char c) const
	{
		const char* string = stdstr::c_str();
		while (*string == c)
		{
			++string;
		}
		return string;
	}

	String String::trimmedRight(const char c) const
	{
		int length = (int)stdstr::size();
		if (length == 0)
		{
			return "";
		}
		const char* string = stdstr::c_str();
		int i = length - 1;
		while (i >= 0 && string[i] == c)
		{
			--i;
		}
		return stdstr::substr(0, i + 1).c_str();
	}

	void String::replace(const String& what, const String& withWhat)
	{
		this->replace(what.c_str(), withWhat.c_str());
	}

	void String::replace(const String& what, const String& withWhat, int times)
	{
		this->replace(what.c_str(), withWhat.c_str(), times);
	}

	void String::replace(const String& what, const char* withWhat)
	{
		this->replace(what.c_str(), withWhat);
	}

	void String::replace(const String& what, const char* withWhat, int times)
	{
		this->replace(what.c_str(), withWhat, times);
	}

	void String::replace(const String& what, const char withWhat, int times)
	{
		this->replace(what.c_str(), withWhat, times);
	}

	void String::replace(const char* what, const String& withWhat)
	{
		this->replace(what, withWhat.c_str());
	}

	void String::replace(const char* what, const String& withWhat, int times)
	{
		this->replace(what, withWhat.c_str(), times);
	}

	void String::replace(const char* what, const char* withWhat)
	{
		int whatLength = (int)strlen(what);
		if (whatLength == 0)
		{
			Log::warn(logTag, "Cannot replace empty string in string: " + *this);
			return;
		}
		int withWhatLength = (int)strlen(withWhat);
		size_t position = 0;
		while (true)
		{
			position = stdstr::find(what, position);
			if (position == std::string::npos)
			{
				break;
			}
			stdstr::replace(position, whatLength, withWhat);
			position += withWhatLength;
		}
	}

	void String::replace(const char* what, const char* withWhat, int times)
	{
		int whatLength = (int)strlen(what);
		if (whatLength == 0)
		{
			Log::warn(logTag, "Cannot replace empty string in string: " + *this);
			return;
		}
		int withWhatLength = (int)strlen(withWhat);
		size_t position = 0;
		while (times > 0)
		{
			position = stdstr::find(what, position);
			if (position == std::string::npos)
			{
				break;
			}
			stdstr::replace(position, whatLength, withWhat);
			position += withWhatLength;
			--times;
		}
	}

	void String::replace(const char* what, const char withWhat, int times)
	{
		int whatLength = (int)strlen(what);
		if (whatLength == 0)
		{
			Log::warn(logTag, "Cannot replace empty string in string: " + *this);
			return;
		}
		size_t position = 0;
		while (true)
		{
			position = stdstr::find(what, position);
			if (position == std::string::npos)
			{
				break;
			}
			stdstr::replace(position, whatLength, times, withWhat);
			position += times;
		}
	}

	void String::replace(const char what, const String& withWhat)
	{
		const char string[2] = { what, '\0' };
		return this->replace(string, withWhat.c_str());
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
		stdstr::replace(position, count, string.c_str());
	}

	void String::replace(int position, int count, const char* string)
	{
		stdstr::replace(position, count, string);
	}

	void String::replace(int position, int count, const char character, int times)
	{
		stdstr::replace(position, count, times, character);
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
		if (position >= 0 && position <= (int)stdstr::size())
		{
			stdstr::insert(position, string.c_str()).c_str();
		}
	}

	void String::insertAt(int position, const char* string)
	{
		if (position >= 0 && position <= (int)stdstr::size())
		{
			stdstr::insert(position, string).c_str();
		}
	}

	void String::insertAt(int position, const char character, int times)
	{
		if (position >= 0 && position <= (int)stdstr::size())
		{
			stdstr::insert(position, times, character);
		}
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
		std::random_shuffle(stdstr::begin(), stdstr::end());
	}

	void String::utf8Randomize()
	{
		std::ustring uString = this->uStr();
		std::random_shuffle(uString.begin(), uString.end());
		this->operator=(fromUnicode(uString.c_str()));
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
		return Array<char>(stdstr::c_str(), (int)stdstr::size());
	}
	
	Array<String> String::split(const char* delimiter, int times, bool removeEmpty) const
	{
		if (times == 0)
		{
			return Array<String>(this, 1);
		}
		Array<String> result;
		const char* string = stdstr::c_str();
		int delimiterSize = (int)strlen(delimiter);
		if (delimiterSize == 0)
		{
			int size = (int)stdstr::size();
			if (times < 0 || times >= size)
			{
				for_iter (i, 0, size)
				{
					result += String(string[i]);
				}
			}
			else
			{
				for_iter (i, 0, times)
				{
					result += String(string[i]);
				}
				result += String(&string[times]);
			}
			return result;
		}
		const char* found = NULL;
		if (times < 0)
		{
			while (true)
			{
				found = strstr(string, delimiter);
				if (found == NULL)
				{
					break;
				}
				result += String(string, (int)(found - string));
				string = found + delimiterSize;
			}
		}
		else
		{
			while (times > 0)
			{
				found = strstr(string, delimiter);
				if (found == NULL)
				{
					break;
				}
				result += String(string, (int)(found - string));
				string = found + delimiterSize;
				--times;
			}
		}
		result += String(string);
		if (removeEmpty)
		{
			result.removeAll("");
		}
		return result;
	}
	
	Array<String> String::split(const char delimiter, int times, bool removeEmpty) const
	{
		const char string[2] = { delimiter, '\0'};
		return this->split(string, times, removeEmpty);
	}

	Array<String> String::split(const String& delimiter, int times, bool removeEmpty) const
	{
		return this->split(delimiter.c_str(), times, removeEmpty);
	}
	
	bool String::split(const char* delimiter, String& outLeft, String& outRight) const
	{
		int index = (int)stdstr::find(delimiter);
		if (index < 0)
		{
			return false;
		}
		outLeft = stdstr::substr(0, index).c_str();
		outRight = stdstr::substr(index + strlen(delimiter)).c_str();
		return true;
	}
	
	bool String::split(const char delimiter, String& outLeft, String& outRight) const
	{
		const char string[2] = { delimiter, '\0'};
		return this->split(string, outLeft, outRight);
	}

	bool String::split(const String& delimiter, String& outLeft, String& outRight) const
	{
		return this->split(delimiter.c_str(), outLeft, outRight);
	}

	Array<String> String::rsplit(const char* delimiter, int times, bool removeEmpty) const
	{
		if (times == 0)
		{
			return Array<String>(this, 1);
		}
		if (times < 0) // if all should be split, rsplit() behaves like split()
		{
			return this->split(delimiter, times, removeEmpty);
		}
		Array<String> result;
		const char* cString = stdstr::c_str();
		int size = (int)stdstr::size();
		int delimiterSize = (int)strlen(delimiter);
		if (delimiterSize == 0)
		{
			result += String(cString, size - times);
			for_iter (i, size - times, size)
			{
				result += String(cString[i]);
			}
			return result;
		}
		if (size < delimiterSize)
		{
			return Array<String>(this, 1);
		}
		const char* string = cString + (size - delimiterSize);
		const char* found = NULL;
		while (string > cString && times > 0)
		{
			if (memcmp(string, delimiter, delimiterSize) == 0)
			{
				found = string;
				string -= delimiterSize;
				--times;
			}
			else
			{
				--string;
			}
		}
		if (string < cString)
		{
			string = cString;
		}
		if (found != NULL)
		{
			result += (string != found && cString != found ? String(cString, (int)(found - cString)) : "");
			string = found + delimiterSize;
		}
		while (true)
		{
			found = strstr(string, delimiter);
			if (found == NULL)
			{
				break;
			}
			result += String(string, (int)(found - string));
			string = found + delimiterSize;
		}
		result += String(string);
		if (removeEmpty)
		{
			result.removeAll("");
		}
		return result;
	}

	Array<String> String::rsplit(const char delimiter, int times, bool removeEmpty) const
	{
		const char string[2] = { delimiter, '\0' };
		return this->rsplit(string, times, removeEmpty);
	}

	Array<String> String::rsplit(const String& delimiter, int times, bool removeEmpty) const
	{
		return this->rsplit(delimiter.c_str(), times, removeEmpty);
	}

	bool String::rsplit(const char* delimiter, String& outLeft, String& outRight) const
	{
		int index = (int)stdstr::rfind(delimiter);
		if (index < 0)
		{
			return false;
		}
		outLeft = stdstr::substr(0, index).c_str();
		outRight = stdstr::substr(index + strlen(delimiter)).c_str();
		return true;
	}

	bool String::rsplit(const char delimiter, String& outLeft, String& outRight) const
	{
		const char string[2] = { delimiter, '\0' };
		return this->rsplit(string, outLeft, outRight);
	}

	bool String::rsplit(const String& delimiter, String& outLeft, String& outRight) const
	{
		return this->rsplit(delimiter.c_str(), outLeft, outRight);
	}

	int String::indexOf(const char c, int start) const
	{
		return (int)stdstr::find(c, start);
	}

	int String::indexOf(const char* string, int start) const
	{
		return (int)stdstr::find(string, start);
	}

	int String::indexOf(const String& string, int start) const
	{
		return (int)stdstr::find(string, start);
	}

	int String::utf8IndexOf(const char c, int start) const
	{
		return (int)this->uStr().find((unsigned int)c, start);
	}

	int String::utf8IndexOf(const char* string, int start) const
	{
		return (int)this->uStr().find(hstr(string).uStr(), start);
	}

	int String::utf8IndexOf(const String& string, int start) const
	{
		return (int)this->uStr().find(string.uStr(), start);
	}

	int String::rindexOf(const char c, int start) const
	{
		return (int)stdstr::rfind(c, start);
	}

	int String::rindexOf(const char* string, int start) const
	{
		return (int)stdstr::rfind(string, start);
	}

	int String::rindexOf(const String& string, int start) const
	{
		return (int)stdstr::rfind(string, start);
	}

	int String::utf8RindexOf(const char c, int start) const
	{
		return (int)this->uStr().rfind((unsigned int)c, start);
	}

	int String::utf8RindexOf(const char* string, int start) const
	{
		return (int)this->uStr().rfind(hstr(string).uStr(), start);
	}

	int String::utf8RindexOf(const String& string, int start) const
	{
		return (int)this->uStr().rfind(string.uStr(), start);
	}

	int String::indexOfAny(const char* string, int start) const
	{
		return (int)stdstr::find_first_of(string, start);
	}

	int String::indexOfAny(const String& string, int start) const
	{
		return (int)stdstr::find_first_of(string.c_str(), start);
	}

	int String::utf8IndexOfAny(const char* string, int start) const
	{
		return (int)this->uStr().find_first_of(hstr(string).uStr(), start);
	}

	int String::utf8IndexOfAny(const String& string, int start) const
	{
		return (int)this->uStr().find_first_of(string.uStr(), start);
	}

	int String::rindexOfAny(const char* string, int start) const
	{
		return (int)stdstr::find_last_of(string, start);
	}

	int String::rindexOfAny(const String& string, int start) const
	{
		return (int)stdstr::find_last_of(string.c_str(), start);
	}

	int String::utf8RindexOfAny(const char* string, int start) const
	{
		return (int)this->uStr().find_last_of(hstr(string).uStr(), start);
	}

	int String::utf8RindexOfAny(const String& string, int start) const
	{
		return (int)this->uStr().find_last_of(string.uStr(), start);
	}

	int String::count(const char c) const
	{
		const char string[2] = { c, '\0' };
		return this->count(string);
	}

	int String::count(const char* string) const
	{
		int result = 0;
		String thisString = stdstr::c_str();
		for_iter (i, 0, this->size())
		{
			if (thisString(i, -1).startsWith(string))
			{
				++result;
				i += strlen(string) - 1;
			}
		}
		return result;
	}

	int String::count(const String& string) const
	{
		return this->count(string.c_str());
	}

	bool String::startsWith(const char* string) const
	{
		return (strncmp(stdstr::c_str(), string, strlen(string)) == 0);
	}

	bool String::startsWith(const String& string) const
	{
		return this->startsWith(string.c_str());
	}

	bool String::endsWith(const char* string) const
	{
		const char* cString = stdstr::c_str();
		int thisLength = this->size();
		int stringLength = (int)strlen(string);
		if (stringLength > thisLength)
		{
			return false;
		}
		return (strcmp(cString + thisLength - stringLength, string) == 0);
	}

	bool String::endsWith(const String& string) const
	{
		return this->endsWith(string.c_str());
	}

	bool String::contains(const char c) const
	{
		return (stdstr::find(c) != stdstr::npos);
	}

	bool String::contains(const char* string) const
	{
		return (stdstr::find(string) != stdstr::npos);
	}

	bool String::contains(const String& string) const
	{
		return (stdstr::find(string.c_str()) != stdstr::npos);
	}

	bool String::containsAny(const char* string) const
	{
		int size = (int)strlen(string);
		for_iter (i, 0, size)
		{
			if (stdstr::find(string[i]) != stdstr::npos)
			{
				return true;
			}
		}
		return false;
	}

	bool String::containsAny(const String& string) const
	{
		return this->containsAny(string.c_str());
	}

	bool String::containsAll(const char* string) const
	{
		int size = (int)strlen(string);
		for_iter (i, 0, size)
		{
			if (stdstr::find(string[i]) == stdstr::npos)
			{
				return false;
			}
		}
		return true;
	}

	bool String::containsAll(const String& string) const
	{
		return this->containsAll(string.c_str());
	}

	bool String::isDigit() const
	{
		return (this->size() == 1 && isdigit(stdstr::c_str()[0]));
	}
	
	bool String::isInt() const
	{
		const char* string = stdstr::c_str();
		if (string[0] == '\0')
		{
			return false;
		}
		int i = 0;
		if (string[i] == '-')
		{
			++i;
		}
		while (string[i] != '\0')
		{
			if (!isdigit(string[i]))
			{
				return false;
			}
			++i;
		}
		return true;
	}

	bool String::isFloat(bool requireDot) const
	{
		const char* string = stdstr::c_str();
		if (string[0] == '\0')
		{
			return false;
		}
		bool foundDot = false;
		int i = 0;
		if (string[i] == '-')
		{
			++i;
		}
		while (string[i] != '\0')
		{
			if (string[i] == '.')
			{
				if (foundDot)
				{
					return false;
				}
				foundDot = true;
			}
			else if (!isdigit(string[i]))
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
		const char* string = stdstr::c_str();
		if (string[0] == '\0')
		{
			return false;
		}
		for (int i = 0; string[i] != '\0'; ++i)
		{
			if (!isxdigit(string[i]))
			{
				return false;
			}
		}
		return true;
	}

	bool String::isAscii() const
	{
		const unsigned char* string = (const unsigned char*)stdstr::c_str();
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
		if (count < 0)
		{
			count = (int)stdstr::size() + count + 1;
		}
		return stdstr::substr(start, count).c_str();
	}

	String String::subString(int start, int count, int step) const
	{
		if (count < 0)
		{
			count = (int)stdstr::size() + count + 1;
		}
		if (step <= 1)
		{
			return stdstr::substr(start, count).c_str();
		}
		const char* cString = stdstr::c_str();
		String result;
		result.reserve((count + step - 1) / step);
		int size = start + count;
		for_iter_step (i, start, size, step)
		{
			result.append(1, cString[i]);
		}
		return result;
	}

	String String::utf8SubString(int start, int count) const
	{
		String result;
		const unsigned char* string = (const unsigned char*)stdstr::c_str();
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
		return (int)stdstr::size();
	}
	
	int String::utf8Size() const
	{
		int result = 0;
		const unsigned char* string = (const unsigned char*)stdstr::c_str();
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
		String result;
		int size = (int)stdstr::size();
		result.reserve(size * 2);
		for_iter (i, 0, size)
		{
			result.append(hsprintf("%02X", stdstr::at(i)));
		}
		return result;
	}

	unsigned int String::unhex() const
	{
		unsigned int i = 0;
		if (this->isHex())
		{
			sscanf(this->uppered().c_str(), "%X", &i);
		}
		return i;
	}

	String String::operator()(int index) const
	{
		return stdstr::at(index);
	}

	String String::operator()(int start, int count) const
	{
		return this->subString(start, count);
	}
	
	String String::operator()(int start, int count, int step) const
	{
		return this->subString(start, count, step);
	}
	
	char& String::operator[](int index)
	{
		return stdstr::at(index);
	}
	
	const char& String::operator[](int index) const
	{
		return stdstr::at(index);
	}
	
	String::operator bool() const
	{
		return (*this != "" && *this != "0" && this->lowered() != "false");
	}

	String::operator short() const
	{
		short s = 0;
		sscanf(stdstr::c_str(), "%hd", &s);
		return s;
	}

	String::operator unsigned short() const
	{
		unsigned short s = 0;
		sscanf(stdstr::c_str(), "%hu", &s);
		return s;
	}

	String::operator int() const
	{
		int i = 0;
		sscanf(stdstr::c_str(), "%d", &i);
		return i;
	}

	String::operator unsigned int() const
	{
		unsigned int i = 0U;
		sscanf(stdstr::c_str(), "%u", &i);
		return i;
	}

	String::operator int64_t() const
	{
		int64_t i = 0LL;
		sscanf(stdstr::c_str(), "%lld", &i);
		return i;
	}

	String::operator uint64_t() const
	{
		uint64_t i = 0ULL;
		sscanf(stdstr::c_str(), "%llu", &i);
		return i;
	}

	String::operator float() const
	{
		float f = 0.0f;
		sscanf(stdstr::c_str(), "%f", &f);
		return f;
	}

	String::operator double() const
	{
		double d = 0.0;
		sscanf(stdstr::c_str(), "%lf", &d);
		return d;
	}

	String String::operator=(char* string)
	{
		stdstr::assign(string);
		return *this;
	}

	String String::operator=(const char* string)
	{
		stdstr::assign(string);
		return *this;
	}

	String String::operator=(const String& string)
	{
		stdstr::assign(string.c_str());
		return *this;
	}

	String String::operator=(const bool b)
	{
		stdstr::assign(BOOL_TO_STRING(b));
		return *this;
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
	
	void String::operator+=(const char c)
	{
		stdstr::append(1, c);
	}

	void String::operator+=(char* string)
	{
		stdstr::append(string);
	}

	void String::operator+=(const char* string)
	{
		stdstr::append(string);
	}

	void String::operator+=(const String& string)
	{
		stdstr::append(string);
	}

	void String::operator+=(const bool b)
	{
		// TODObool - fix this inconsistency
		//stdstr::append(BOOL_TO_STRING(b));
		stdstr::append(b ? "1" : "0");
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

	String String::operator+(const char c) const
	{
		String result(*this);
		result.append(1, c);
		return result;
	}

	String String::operator+(char* string) const
	{
		String result(*this);
		result.append(string);
		return result;
	}

	String String::operator+(const char* string) const
	{
		String result(*this);
		result.append(string);
		return result;
	}

	String String::operator+(const String& string) const
	{
		String result(*this);
		result.append(string);
		return result;
	}

	bool String::operator==(const char* string) const
	{
		return (strcmp(stdstr::c_str(), string) == 0);
	}

	bool String::operator==(const String& string) const
	{
		return (strcmp(stdstr::c_str(), string.c_str()) == 0);
	}

	bool String::operator==(const bool b) const
	{
		const char* string = stdstr::c_str();
		if (b)
		{
			return (strcmp(string, "1") == 0 || strcmp(string, "true") == 0);
		}
		return (strcmp(string, "0") == 0 || strcmp(string, "false") == 0);
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

	bool String::operator!=(const char* string) const
	{
		return !(this->operator==(string));
	}

	bool String::operator!=(const String& string) const
	{
		return !(this->operator==(string));
	}

	bool String::operator!=(const bool b) const
	{
		return !(this->operator==(b));
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

	bool String::operator<(const String& string) const
	{
		return (strcmp(stdstr::c_str(), string.c_str()) < 0);
	}

	bool String::operator>(const String& string) const
	{
		return (strcmp(stdstr::c_str(), string.c_str()) > 0);
	}

	bool String::operator<=(const String& string) const
	{
		return (strcmp(stdstr::c_str(), string.c_str()) <= 0);
	}

	bool String::operator>=(const String& string) const
	{
		return (strcmp(stdstr::c_str(), string.c_str()) >= 0);
	}

	const char* String::cStr() const
	{
		return stdstr::c_str();
	}

	std::ustring String::uStr() const
	{
		std::ustring result;
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
		const unsigned char* string = (const unsigned char*)stdstr::c_str();
		int i = 0;
		int size = 0;
		while (string[i] != 0)
		{
			_TO_UNICODE_FAST(code, string, i, size);
			result += code;
			i += size;
		}
		return result;
	}

	std::wstring String::wStr() const
	{
		std::wstring result;
#ifdef __APPLE__ // bugfix for apple llvm compiler, has allocation problems in std::string with unsigned int combination
		if (stdstr::size() == 0)
		{
			wchar_t ary[] = {'x', 0};
			result = ary;
			ary[0] = 0;
			result = ary;
			return result;
		}
#endif
		unsigned int code = 0;
		const unsigned char* string = (const unsigned char*)stdstr::c_str();
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
			result += code;
			i += size;
		}
		return result;
	}

	unsigned int String::firstUnicodeChar(int index, int* byteCount) const
	{
		unsigned int result = 0;
		const unsigned char* string = (const unsigned char*)stdstr::c_str();
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
	if (i >= 8)
	{
#ifdef _DEBUG
		delete[] c;
		throw Exception("Resulting string for hsprintf is longer than 2^16 (65536) characters! format: " + hstr(format));
#else
		hlog::error(hltypes::logTag, "Resulting string for hsprintf is longer than 2^16 (65536) characters! format: " + hstr(format));
		c[0] = '\0'; // in case sprintf corrupted c
#endif
	}
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
