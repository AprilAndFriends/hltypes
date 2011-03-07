/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "harray.h"
#include "hstring.h"
#include "util.h"

double hround(double x)
{
	return floor(x + 0.5);
}

float hroundf(float x)
{
	return floor(x + 0.5f);
}

long hroundl(double x)
{
	return static_cast<long>(floor(x + 0.5));
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

hstr normalize_path(chstr path)
{
	harray<hstr> directories = path.replace('\\', '/').rtrim('/').split('/');
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
	else// if (value < 0x10000)
	{
		result += (char)(0xE0 | (value >> 12));
		result += (char)(0x80 | ((value >> 6) & 0x3F));
		result += (char)(0x80 | (value & 0x3F));
	}
	/*
	else
	{
		result += (char)(0xF0 | (value >> 18));
		result += (char)(0x80 | ((value >> 12) & 0x3F));
		result += (char)(0x80 | ((value >> 6) & 0x3F));
		result += (char)(0x80 | (value & 0x3F));
	}
	*/
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

unsigned int getCharUtf8(const char* s, int* char_len_out)
{
	if (*s < 0)
	{
		const unsigned char* u = (const unsigned char*)s;
		const unsigned char first = *u;
		if ((first & 0xE0) == 0xC0)
		{
			*char_len_out = 2;
			return ((first & 0x1F) << 6) | (u[1] & 0x3F);
		}
		if ((first & 0xF0) == 0xE0)
		{
			*char_len_out = 3;
			return ((((first & 0xF) << 6) | (u[1] & 0x3F) ) << 6) | (u[2] & 0x3F);
		}
		*char_len_out = 4;
		return ((((((first & 7) << 6) | (u[1] & 0x3F) ) << 6) | (u[2] & 0x3F)) << 6) | (u[3] & 0x3F);
	}
	*char_len_out = 1;
	return *s;
}

void utf8_to_unicode(chstr input, wchar_t* result)
{
	const unsigned char* str = (const unsigned char*)input.c_str();
	int i = 0;
	int j = 0;
	int length = 1;
	while (str[i] != 0)
	{
		if (str[i] < 0x80)
		{
			result[j] = str[i];
			length = 1;
		}
		else if ((str[i] & 0xE0) == 0xC0)
		{
			result[j] = ((str[i] & 0x1F) << 6) | (str[i + 1] & 0x3F);
			length = 2;
		}
		else if ((str[i] & 0xF0) == 0xE0)
		{
			result[j] = ((((str[i] & 0xF) << 6) | (str[i + 1] & 0x3F) ) << 6) | (str[i + 2] & 0x3F);
			length = 3;
		}
		else
		{
			result[j] = ((((((str[i] & 0x7) << 6) | (str[i + 1] & 0x3F)) << 6) | (str[i + 2] & 0x3F)) << 6) | (str[i + 3] & 0x3F);
			length = 4;
		}
		i += length;
		j++;
	}
	result[j] = 0;
}
