/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifndef _MSC_VER // required for memset on non-MSVC compilers
#include <string.h>
#endif

#include "harray.h"
#include "hfile.h"
#include "hstring.h"
#include "util.h"

double hroundd(double value)
{
	return floor(value + 0.5);
}

float hroundf(float value)
{
	return floor(value + 0.5f);
}

int hround(double value)
{
	return (int)(floor(value + 0.5));
}

int hrand(int min, int max)
{
#ifdef _WIN32
	return min + rand() * (max - min) / (RAND_MAX + 1);
#else
	return min + ((double)rand()) / ((double)RAND_MAX + 1) * (max - min);
#endif
}

int hrand(int max)
{
#ifdef _WIN32
	return rand() * max / (RAND_MAX + 1);
#else
	return (((double)rand()) / ((double)RAND_MAX + 1)) * max;
#endif
}

float hrandf(float min, float max)
{
#ifdef _WIN32
	return min + rand() * (max - min) / (RAND_MAX + 1);
#else
	return min + ((double)rand()) / ((double)RAND_MAX + 1) * (max - min);
#endif
}

float hrandf(float max)
{
#ifdef _WIN32
	return rand() * max / (RAND_MAX + 1);
#else
	return (((double)rand()) / ((double)RAND_MAX + 1)) * max;
#endif
}

bool heqf(float a, float b, float tolerance)
{
	return (fabs(a - b) < tolerance);
}

int hcmpf(float a, float b, float tolerance)
{
	return (heqf(a, b, tolerance) ? 0 : (a > b ? 1 : -1));
}

hstr normalize_path(chstr path)
{
	harray<hstr> directories = path.replace('\\', '/').rtrim('/').split('/', -1, false);
	harray<hstr> result;
	while (directories.size() > 0)
	{
		if (directories.front() == ".")
		{
			directories.pop_front();
		}
		else if (directories.front() == "..")
		{
			if (result.size() == 0)
			{
				result += directories.pop_front();
			}
			else if (result.back() == "..")
			{
				result += directories.pop_front();
			}
			else
			{
				result.pop_back();
				directories.pop_front();
			}
		}
		else
		{
			result += directories.pop_front();
		}
	}
	if (result.size() == 0)
	{
		return ".";
	}
	return result.join('/');
}

hstr systemize_path(chstr path)
{
	hstr result = normalize_path(path);
#ifdef NO_FS_TREE
	result = result.replace("/", "_");
#endif
	return result;
}

hstr get_basedir(chstr filename)
{
	harray<hstr> result = filename.replace('\\', '/').rtrim('/').split('/', -1, false);
	if (result.size() < 2)
	{
		return ".";
	}
	result.pop_back();
	return result.join("/");
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

hstr unicode_to_utf8(const unsigned int* string)
{
	hstr result;
	for (int i = 0; string[i] != 0; i++)
	{
		result += unicode_to_utf8(string[i]);
	}
	return result;
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

hstr unicode_to_utf8(wchar_t value)
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
	return result;
}

hstr unicode_to_utf8(const wchar_t* string)
{
	hstr result;
	for (int i = 0; string[i] != 0; i++)
	{
		result += unicode_to_utf8(string[i]);
	}
	return result;
}

hstr unicode_to_utf8(harray<wchar_t> chars)
{
	hstr result;
	foreach (wchar_t, it, chars)
	{
		result += unicode_to_utf8(*it);
	}
	return result;
}

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

unsigned int* utf8_to_unicode(chstr input, int* length)
{
	unsigned int* result = new unsigned int[input.size() + 1];
	memset(result, 0, (input.size() + 1) * sizeof(unsigned int));
	const unsigned char* str = (const unsigned char*)input.c_str();
	int i = 0;
	int j = 0;
	while (str[i] != 0)
	{
		if (str[i] < 0x80)
		{
			result[j] = str[i];
			i += 1;
		}
		else if ((str[i] & 0xE0) == 0xC0)
		{
			result[j] = ((str[i] & 0x1F) << 6) | (str[i + 1] & 0x3F);
			i += 2;
		}
		else if ((str[i] & 0xF0) == 0xE0)
		{
			result[j] = ((((str[i] & 0xF) << 6) | (str[i + 1] & 0x3F) ) << 6) | (str[i + 2] & 0x3F);
			i += 3;
		}
		else
		{
			result[j] = ((((((str[i] & 0x7) << 6) | (str[i + 1] & 0x3F)) << 6) | (str[i + 2] & 0x3F)) << 6) | (str[i + 3] & 0x3F);
			i += 4;
		}
		j++;
	}
	if (length != NULL)
	{
		*length = j;
	}
	return result;
}

// CRC32 stuff

unsigned int crc32_table[256];
bool crc32_table_created = false;
void create_crc32_table()
{
	if (crc32_table_created)
	{
		return;
	}
	unsigned int polynome = 0xEDB88320;
	unsigned int sum;
	for (int i = 0; i < 256; i++)
	{
		sum = i;
		for (int j = 8; j > 0; j--)
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

unsigned int calc_crc32(unsigned char* data, int size)
{
	create_crc32_table();
	unsigned int crc = 0xFFFFFFFF;
	for (int i = 0; i < size; i++)
	{
		crc = ((crc >> 8) & 0x00FFFFFF) ^ crc32_table[(crc ^ data[i]) & 0xFF];
	}
	return (crc ^ 0xFFFFFFFF);
}

unsigned int calc_crc32(chstr filename)
{
	create_crc32_table();
	hfile f(filename);
	int size = f.size();
	unsigned char* data = new unsigned char[size];
	f.read_raw(data, size);
	f.close();
	unsigned int crc = 0xFFFFFFFF;
	for (int i = 0; i < size; i++)
	{
		crc = ((crc >> 8) & 0x00FFFFFF) ^ crc32_table[(crc ^ data[i]) & 0xFF];
	}
	delete data;
	return (crc ^ 0xFFFFFFFF);
}