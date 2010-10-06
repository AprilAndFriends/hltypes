/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef HLTYPES_UTIL_H
#define HLTYPES_UTIL_H

#include <math.h>

#include "hltypesExport.h"
#include "hstring.h"

#define HL_RAD_TO_DEG_RATIO 57.295779513082320876798154814105
#define HL_DEG_TO_RAD_RATIO 0.01745329251994329576923690768489

/******* ITERATOR MACROS ***********************************************/
#define foreach_in_list(type, name, lst) for (std::list<type>::iterator name = lst.begin(); name != lst.end(); name++)

/******* OTHER MACROS **************************************************/
#define dsin(degrees) sin((degrees) * HL_DEG_TO_RAD_RATIO)
#define dcos(degrees) cos((degrees) * HL_DEG_TO_RAD_RATIO)
#define dasin(value) (asin(value) * HL_RAD_TO_DEG_RATIO)
#define dacos(value) (acos(value) * HL_RAD_TO_DEG_RATIO)
#define HL_E_TOLERANCE (0.01f)

/******* PROTOTYPES ****************************************************/
int hltypesFnExport hrand(int min, int max);
int hltypesFnExport hrand(int max);
float hltypesFnExport hrandf(float min, float max);
float hltypesFnExport hrandf(float max);
//2DO - implement
//void hltypesFnExport removedirs(chstr path);

/******* TEMPLATE FUNCTIONS ********************************************/
template <class T> T hmin(T a, T b)
{
	return (a < b ? a : b);
}

template <class T> T hmax(T a, T b)
{
	return (a > b ? a : b);
}

template <class T> T hclamp(T value, T min, T max)
{
	return (value < min ? min : (value > max ? max : value));
}

template <class T> int sgn(T value)
{
	return (value >= 0 ? 1 : -1);
}

template <class T> bool is_between(T value, T min, T max)
{
	return (value >= min && value <= max);
}

/******** MACROS FOR HELP IN DEBUGGING ******************************/
#define HLBREAKPT \
	printf("HL Breakpoint:\n"); \
	printf(" -> File: %s\n", __FILE__); \
	printf(" -> Line: %d\n", __LINE__); \
	printf(" -> Func: %s\n", __PRETTY_FUNCTION__); \
	system("pause"); // TODO implement pause in a nicer, xplatform way

#endif
