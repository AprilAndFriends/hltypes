/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#elif defined(__APPLE__)
#include <sys/stat.h>
#define _mkdir(name) mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

#include "harray.h"
#include "hstring.h"
#include "util.h"

int hrand(int min, int max)
{
#ifdef _WIN32
	return min + rand() * (max - min) / RAND_MAX;
#else
	return min + (((double) rand()) / RAND_MAX) * (max - min);
#endif
}

int hrand(int max)
{
#ifdef _WIN32
	return rand() * max / RAND_MAX;
#else
	return (((double) rand()) / RAND_MAX) * max;
#endif
}

float hrandf(float min, float max)
{
	return min + rand() * (max - min) / RAND_MAX;
}

float hrandf(float max)
{
	return rand() * max / RAND_MAX;
}

void makedirs(chstr path)
{
	harray<hstr> folders = path.split("/");
	if (folders.size() > 0)
	{
		hstr path = folders.pop_front();
		foreach (hstr, it, folders)
		{
			path += "/" + (*it);
			_mkdir(path.c_str());
		}
	}
}

