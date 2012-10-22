/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifndef _MSC_VER // required for memset on non-MSVC compilers
#include <string.h>
#endif
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <stdio.h>
#include <time.h>

#include "harray.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hresource.h"
#include "hstring.h"

unsigned int get_system_tick_count()
{
#ifdef _WIN32
#if !_HL_WINRT // because GetTickCount64() is not available pre-Vista
	return GetTickCount();
#else
	return (unsigned int)GetTickCount64();
#endif
#else
	timeval tv = {0, 0};
	gettimeofday(&tv, NULL);
	return (unsigned int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

unsigned int get_system_time()
{
	return (unsigned int)time(NULL);
}

int hrand(int min, int max)
{
	if (max <= min)
	{
		return min;
	}
#ifdef _WIN32
	return (min + rand() * (max - min) / (RAND_MAX + 1));
#else
	return (min + ((double)rand()) / ((double)RAND_MAX + 1) * (max - min));
#endif
}

int hrand(int max)
{
	if (max <= 0)
	{
		return 0;
	}
#ifdef _WIN32
	return (rand() * max / (RAND_MAX + 1));
#else
	return ((((double)rand()) / ((double)RAND_MAX + 1)) * max);
#endif
}

float hrandf(float min, float max)
{
	if (max <= min)
	{
		return min;
	}
#ifdef _WIN32
	return (min + rand() * (max - min) / (RAND_MAX + 1));
#else
	return (min + ((double)rand()) / ((double)RAND_MAX + 1) * (max - min));
#endif
}

float hrandf(float max)
{
	if (max <= 0.0f)
	{
		return 0.0f;
	}
#ifdef _WIN32
	return (rand() * max / (RAND_MAX + 1));
#else
	return ((((double)rand()) / ((double)RAND_MAX + 1)) * max);
#endif
}

double hrandd(double min, double max)
{
	if (max <= min)
	{
		return min;
	}
#ifdef _WIN32
	return (min + rand() * (max - min) / (RAND_MAX + 1));
#else
	return (min + ((double)rand()) / ((double)RAND_MAX + 1) * (max - min));
#endif
}

double hrandd(double max)
{
	if (max <= 0.0)
	{
		return 0.0;
	}
#ifdef _WIN32
	return (rand() * max / (RAND_MAX + 1));
#else
	return ((((double)rand()) / ((double)RAND_MAX + 1)) * max);
#endif
}

int hfloor(float value)
{
	return (int)(value > 0.0f || value == (int)value ? value : value - 1);
}

int hfloor(double value)
{
	return (int)(value > 0.0 || value == (int)value ? value : value - 1);
}

float hfloorf(float value)
{
	return (float)hfloor(value);
}

double hfloord(double value)
{
	return (double)hfloor(value);
}

int hceil(float value)
{
	return (int)(value < 0.0f || value == (int)value ? value : value + 1);
}

int hceil(double value)
{
	return (int)(value < 0.0 || value == (int)value ? value : value + 1);
}

float hceilf(float value)
{
	return (float)hceil(value);
}

double hceild(double value)
{
	return (double)hceil(value);
}

int hround(float value)
{
	return hfloor(value + 0.5);
}

int hround(double value)
{
	return hfloor(value + 0.5);
}

float hroundf(float value)
{
	return (float)hround(value);
}

double hroundd(double value)
{
	return (double)hround(value);
}

int habs(int value)
{
	return abs(value);
}

long habs(long value)
{
	return labs(value);
}

float habs(float value)
{
	return fabs(value);
}

double habs(double value)
{
	return fabs(value);
}

long double habs(long double value)
{
	return fabs(value);
}

int hmod(int i, int m)
{
	return (i >= 0 ? i % m : (m - (-i % m)) % m);
}

float hmodf(float f, float m)
{
	return (f >= 0.0f ? fmod(f, m) : fmod(m - fmod(-f, m), m));
}

double hmodd(double d, double m)
{
	return (d >= 0.0 ? fmod(d, m) : fmod(m - fmod(-d, m), m));
}

double hhypot(double a, double b)
{
	return sqrt(a * a + b * b);
}

float hhypot(float a, float b)
{
	return (float)sqrt(a * a + b * b);
}

double hhypotSquared(double a, double b)
{
	return (a * a + b * b);
}

float hhypotSquared(float a, float b)
{
	return (a * a + b * b);
}

bool heqf(float a, float b, float tolerance)
{
	return (habs(a - b) < tolerance);
}

bool heqd(double a, double b, double tolerance)
{
	return (habs(a - b) < tolerance);
}

int hcmpf(float a, float b, float tolerance)
{
	return (heqf(a, b, tolerance) ? 0 : (a > b ? 1 : -1));
}

int hcmpd(double a, double b, double tolerance)
{
	return (heqd(a, b, tolerance) ? 0 : (a > b ? 1 : -1));
}

hstr systemize_path(chstr path)
{
	return path.replace('\\', '/');
}

hstr normalize_path(chstr path)
{
	harray<hstr> directories = systemize_path(path).rtrim('/').split('/', -1, false);
	harray<hstr> result;
	while (directories.size() > 0)
	{
		if (directories.first() == ".")
		{
			directories.pop_first();
		}
		else if (directories.first() == "..")
		{
			if (result.size() == 0)
			{
				result += directories.pop_first();
			}
			else if (result.last() == "..")
			{
				result += directories.pop_first();
			}
			else
			{
				result.pop_last();
				directories.pop_first();
			}
		}
		else
		{
			result += directories.pop_first();
		}
	}
	if (result.size() == 0)
	{
		return ".";
	}
	return result.join('/');
}

hstr get_basedir(chstr path)
{
	harray<hstr> result = path.replace('\\', '/').rtrim('/').split('/', -1, false);
	if (result.size() < 2)
	{
		return ".";
	}
	result.pop_last();
	return result.join("/");
}

hstr get_basename(chstr path)
{
	harray<hstr> result = path.replace('\\', '/').rtrim('/').split('/', -1, false);
	if (result.size() == 0)
	{
		return "";
	}
	return result.pop_last();
}

hstr get_environment_variable(chstr name)
{
#ifdef _WIN32
#if !_HL_WINRT
	return unicode_to_utf8(_wgetenv(name.w_str().c_str()));
#else
	return ""; // WinRT does not support environment variables
#endif
#else
	return hstr(getenv(name.c_str()));
#endif
}

// Unicode stuff

hstr unicode_to_utf8(unsigned int value)
{
	hstr result;
	if (value < 0x80)
	{
		result += (char)value;
	}
	else if (value < 0x800)
	{
		result += (char)(0xC0 | (value >> 6));
		result += (char)(0x80 | (value & 0x3F));
	}
	else if (value < 0x10000)
	{
		result += (char)(0xE0 | (value >> 12));
		result += (char)(0x80 | ((value >> 6) & 0x3F));
		result += (char)(0x80 | (value & 0x3F));
	}
	else if (value < 0x200000)
	{
		result += (char)(0xF0 | (value >> 18));
		result += (char)(0x80 | ((value >> 12) & 0x3F));
		result += (char)(0x80 | ((value >> 6) & 0x3F));
		result += (char)(0x80 | (value & 0x3F));
	}
	else if (value < 0x4000000)
	{
		result += (char)(0xF8 | (value >> 24));
		result += (char)(0x80 | ((value >> 18) & 0x3F));
		result += (char)(0x80 | ((value >> 12) & 0x3F));
		result += (char)(0x80 | ((value >> 6) & 0x3F));
		result += (char)(0x80 | (value & 0x3F));
	}
	else if (value < 0x80000000)
	{
		result += (char)(0xFC | (value >> 30));
		result += (char)(0x80 | ((value >> 24) & 0x3F));
		result += (char)(0x80 | ((value >> 18) & 0x3F));
		result += (char)(0x80 | ((value >> 12) & 0x3F));
		result += (char)(0x80 | ((value >> 6) & 0x3F));
		result += (char)(0x80 | (value & 0x3F));
	}
	return result;
}

hstr unicode_to_utf8(wchar_t value)
{
	return unicode_to_utf8((unsigned int)value);
}

hstr unicode_to_utf8(char value)
{
	return value;
}

hstr unicode_to_utf8(unsigned char value)
{
	return (char)value;
}

hstr unicode_to_utf8(const unsigned int* string)
{
	hstr result;
	if (string != NULL)
	{
		for (int i = 0; string[i] != 0; i++)
		{
			result += unicode_to_utf8(string[i]);
		}
	}
	return result;
}

hstr unicode_to_utf8(const wchar_t* string)
{
	hstr result;
	if (string != NULL)
	{
		for (int i = 0; string[i] != 0; i++)
		{
			result += unicode_to_utf8((unsigned int)string[i]);
		}
	}
	return result;
}

hstr unicode_to_utf8(const char* string)
{
	return (string != NULL ? string : "");
}

hstr unicode_to_utf8(const unsigned char* string)
{
	return (string != NULL ? (const char*)string : "");
}

hstr unicode_to_utf8(harray<unsigned int> chars)
{
	hstr result;
	foreach (unsigned int, it, chars)
	{
		result += unicode_to_utf8(*it);
	}
	return result;
}

hstr unicode_to_utf8(harray<wchar_t> chars)
{
	hstr result;
	foreach (wchar_t, it, chars)
	{
		result += unicode_to_utf8((unsigned int)(*it));
	}
	return result;
}

hstr unicode_to_utf8(harray<char> chars)
{
	hstr result;
	foreach (char, it, chars)
	{
		result += (*it);
	}
	return result;
}

hstr unicode_to_utf8(harray<unsigned char> chars)
{
	hstr result;
	foreach (unsigned char, it, chars)
	{
		result += (char)(*it);
	}
	return result;
}

// TODO - this should be removed
unsigned int utf8_to_uint(chstr input, int* character_length)
{
	/*
	 7	U+7F		0xxxxxxx
	11	U+7FF		110xxxxx	10xxxxxx
	16	U+FFFF		1110xxxx	10xxxxxx	10xxxxxx
	21	U+1FFFFF	11110xxx	10xxxxxx	10xxxxxx	10xxxxxx
	26	U+3FFFFFF	111110xx	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx
	31	U+7FFFFFFF	1111110x	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx
	*/
	unsigned int result = 0;
	const unsigned char* u = (const unsigned char*)input.c_str();
	int length = 0;
	if (u[0] < 0x80)
	{
		result = u[0];
		length = 1;
	}
	else if ((u[0] & 0xE0) == 0xC0)
	{
		result = ((u[0] & 0x1F) << 6) | (u[1] & 0x3F);
		length = 2;
	}
	else if ((u[0] & 0xF0) == 0xE0)
	{
		result = ((((u[0] & 0xF) << 6) | (u[1] & 0x3F)) << 6) | (u[2] & 0x3F);
		length = 3;
	}
	else
	{
		result = ((((((u[0] & 0x7) << 6) | (u[1] & 0x3F)) << 6) | (u[2] & 0x3F)) << 6) | (u[3] & 0x3F);
		length = 4;
	}
	if (character_length != NULL)
	{
		*character_length = length;
	}
	return result;
}

std::basic_string<unsigned int> utf8_to_unicode(chstr input)
{
	std::basic_string<unsigned int> result;
	const unsigned char* str = (const unsigned char*)input.c_str();
	int i = 0;
	while (str[i] != 0)
	{
		if (str[i] < 0x80)
		{
			result += str[i];
			i += 1;
		}
		else if ((str[i] & 0xE0) == 0xC0)
		{
			result += ((str[i] & 0x1F) << 6) | (str[i + 1] & 0x3F);
			i += 2;
		}
		else if ((str[i] & 0xF0) == 0xE0)
		{
			result += ((((str[i] & 0xF) << 6) | (str[i + 1] & 0x3F) ) << 6) | (str[i + 2] & 0x3F);
			i += 3;
		}
		else
		{
			result += ((((((str[i] & 0x7) << 6) | (str[i + 1] & 0x3F)) << 6) | (str[i + 2] & 0x3F)) << 6) | (str[i + 3] & 0x3F);
			i += 4;
		}
	}
	return result;
}

std::basic_string<wchar_t> utf8_to_wchars(chstr input)
{
	std::basic_string<unsigned int> ustring = utf8_to_unicode(input);
	std::basic_string<wchar_t> result;
	for_itert (unsigned int, i, 0, ustring.size())
	{
		result += (wchar_t)ustring[i];
	}
	return result;
}

// CRC32 stuff

static unsigned int crc32_table[256];
static bool crc32_table_created = false;
void create_crc32_table()
{
	if (crc32_table_created)
	{
		return;
	}
	unsigned int polynome = 0xEDB88320;
	unsigned int sum;
	for_iter (i, 0, 256)
	{
		sum = i;
		for_iter (j, 0, 8)
		{
			if ((sum & 1) != 0)
			{
				sum = (sum >> 1) ^ polynome;
			}
			else
			{
				sum >>= 1;
			}
		}
		crc32_table[i] = sum;
	}
	crc32_table_created = true;
}

unsigned int calc_crc32(unsigned char* data, long size)
{
	create_crc32_table();
	unsigned int crc = 0xFFFFFFFF;
	for_itert (long, i, 0, size)
	{
		crc = (crc >> 8) ^ crc32_table[(crc ^ data[i]) & 0xFF];
	}
	return ((crc & 0xFFFFFFFF) ^ 0xFFFFFFFF);
}

unsigned int calc_crc32(hsbase* stream, long size)
{
	if (size <= 0)
	{
		return 0;
	}
	unsigned char* data = new unsigned char[size];
	stream->read_raw(data, size);
	unsigned int result = calc_crc32(data, size);
	delete [] data;
	return result;
}

unsigned int calc_crc32(hsbase* stream)
{
	return calc_crc32(stream, stream->size() - stream->position());
}

