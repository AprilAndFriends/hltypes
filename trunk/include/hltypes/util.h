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

template <class T> T sign(T number)
{
	if (number >= 0) return 1;
	else             return -1;
}

template <class T> T clamp(T value,T min_value,T max_value)
{
	if      (value < min_value) value=min_value;
	else if (value > max_value) value=max_value;
	return value;
}

#define dsin(degrees) sin(degrees/57.295779513f)
#define dcos(degrees) cos(degrees/57.295779513f)

#define foreach(type,lst) for (type* it = lst.iter(); it; it = lst.next())
#define foreach_reverse(type,lst) for (type* it = lst.riter(); it; it = lst.rnext())
#define foreach_in_map(type,list) for (std::map<hstr,type>::iterator it=list.begin();it != list.end(); it++)
#define foreach_in_list(type,lst) for (std::list<type>::iterator it=lst.begin();it != lst.end();it++)

#endif
