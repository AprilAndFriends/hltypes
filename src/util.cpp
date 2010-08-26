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

#include "util.h"

int hrand(int min, int max)
{
	return min + rand() * max / RAND_MAX;
}

int hrand(int max)
{
	return hrand(0, max);
}

float hrandf(float min, float max)
{
	return min + rand() * max / RAND_MAX;
}

float hrandf(float max)
{
	return hrandf(0.0f, max);
}

