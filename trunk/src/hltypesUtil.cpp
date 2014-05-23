/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef _MSC_VER // required for memset on non-MSVC compilers
#include <string.h>
#endif
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "harray.h"
#include "hlog.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	hstr logTag = "hltypes";
}

unsigned int get_system_tick_count()
{
#ifdef _WIN32
#ifndef _WINRT // because GetTickCount64() is not available pre-Vista
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

float hsqrt(int value)
{
	return sqrt((float)value);
}

float hsqrt(float value)
{
	return sqrt(value);
}

double hsqrt(double value)
{
	return sqrt(value);
}

long double hsqrt(long double value)
{
	return sqrt(value);
}

float hhypot(float a, float b)
{
	return (float)sqrt(a * a + b * b);
}

double hhypot(double a, double b)
{
	return sqrt(a * a + b * b);
}

float hhypot(int a, int b)
{
	return (float)sqrt((float)a * a + (float)b * b);
}

double hhypotd(int a, int b)
{
	return sqrt((double)a * a + (double)b * b);
}

float hhypotSquared(float a, float b)
{
	return (a * a + b * b);
}

double hhypotSquared(double a, double b)
{
	return (a * a + b * b);
}

int hhypotSquared(int a, int b)
{
	return (a * a + b * b);
}

float hhypotSquaredf(int a, int b)
{
	return (float)(a * a + b * b);
}

double hhypotSquaredd(int a, int b)
{
	return (double)(a * a + b * b);
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

// DEPRECATED
#include "hdir.h"
hstr get_basedir(chstr path)	{ return hdir::basedir(path); }
hstr get_basename(chstr path)	{ return hdir::basename(path); }
hstr systemize_path(chstr path)	{ return hdir::systemize(path); }
hstr normalize_path(chstr path)	{ return hdir::normalize(path); }

hstr get_environment_variable(chstr name)
{
#ifdef _WIN32
#ifndef _WINRT
	return hstr::from_unicode(_wgetenv(name.w_str().c_str()));
#else
	return ""; // WinRT does not support environment variables
#endif
#else
	return hstr(getenv(name.c_str()));
#endif
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
				sum = ((sum >> 1) & 0x7FFFFFFF) ^ polynome;
			}
			else
			{
				sum = ((sum >> 1) & 0x7FFFFFFF);
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
		crc = ((crc >> 8) & 0xFFFFFF) ^ crc32_table[(crc ^ data[i]) & 0xFF];
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

