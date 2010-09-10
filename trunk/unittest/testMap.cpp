/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

/******* MAP *******************************************************************************/

TEST(Map_Structure)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	CHECK(a["a"] == 1);
	CHECK(a["abc"] == 2);
}

TEST(Map_Iteration)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	//2DO - needs to be changed
	foreach_m (int, it, a)
	{
		CHECK(it->first == "a" && it->second == 1 ||
			it->first == "abc" && it->second == 2);
	}
}

