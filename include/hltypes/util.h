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

template <class T> T hmin(T a,T b)
{
	return (a < b) ? a : b;
}

template <class T> T hmax(T a,T b)
{
	return (a > b) ? a : b;
}

#endif
