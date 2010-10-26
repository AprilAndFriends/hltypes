/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "harray.h"
#include "hstring.h"
#include "util.h"

int hrand(int min, int max)
{
#ifdef _WIN32
	return min + rand() * (max - min) / (RAND_MAX + 1);
#else
	return min + ((double) rand()) / (RAND_MAX + 1) * (max - min);
#endif
}

int hrand(int max)
{
#ifdef _WIN32
	return rand() * max / (RAND_MAX + 1);
#else
	return (((double) rand()) / (RAND_MAX + 1)) * max;
#endif
}

float hrandf(float min, float max)
{
	return min + rand() * (max - min) / (RAND_MAX + 1);
}

float hrandf(float max)
{
	return rand() * max / (RAND_MAX + 1);
}

hstr normalize_path(chstr path)
{
	return path.replace('\\', '/').rtrim('/');
}


