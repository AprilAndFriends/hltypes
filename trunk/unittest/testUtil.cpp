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

#include <stdlib.h>
#include <time.h>

#include <hltypes/hstring.h>
#include <hltypes/util.h>

/******* UTIL ******************************************************************************/
TEST(Util_sgn)
{
	CHECK(sgn(-2) == -1);
	CHECK(sgn(-1) == -1);
	CHECK(sgn(0) == 1);
	CHECK(sgn(1) == 1);
	CHECK(sgn(2) == 1);
	CHECK(sgn(1.5f) == 1);
	CHECK(sgn(-1.5f) == -1);
}

TEST(Util_is_between)
{
	CHECK(is_between(6, 0, 22));
	CHECK(is_between(0, 0, 22));
	CHECK(is_between(22, 0, 22));
	CHECK(!is_between(23, 0, 22));
	CHECK(!is_between(-1, 0, 22));
	CHECK(is_between(6.7f, 5.0f, 10.0f));
	CHECK(is_between(5.0f, 5.0f, 10.0f));
	CHECK(is_between(10.0f, 5.0f, 10.0f));
	CHECK(!is_between(3.0f, 5.0f, 10.0f));
	CHECK(!is_between(10.0001f, -5.0f, 10.0f));
}

TEST(Util_hmin)
{
	CHECK(hmin(6, 0) == 0);
	CHECK(hmin(0, 0) == 0);
	CHECK(hmin(22, 0) == 0);
	CHECK(hmin(-1, 0) == -1);
	CHECK(hmin(6.7f, 5.0f) == 5.0f);
	CHECK(hmin(5.0f, 5.0f) == 5.0f);
	CHECK(hmin(10.0f, 5.0f) == 5.0f);
	CHECK(hmin(3.0f, 5.0f) == 3.0f);
	CHECK(hmin(10.0001f, -5.0f) == -5.0f);
}

TEST(Util_hmax)
{
	CHECK(hmax(6, 0) == 6);
	CHECK(hmax(0, 0) == 0);
	CHECK(hmax(22, 0) == 22);
	CHECK(hmax(-1, 0) == 0);
	CHECK(hmax(6.7f, 5.0f) == 6.7f);
	CHECK(hmax(5.0f, 5.0f) == 5.0f);
	CHECK(hmax(10.0f, 5.0f) == 10.0f);
	CHECK(hmax(3.0f, 5.0f) == 5.0f);
	CHECK(hmax(10.0001f, -5.0f) == 10.0001f);
}

TEST(Util_hclamp)
{
	CHECK(hclamp(6, 0, 22) == 6);
	CHECK(hclamp(0, 0, 22) == 0);
	CHECK(hclamp(22, 0, 22) == 22);
	CHECK(hclamp(23, 0, 22) == 22);
	CHECK(hclamp(-1, 0, 22) == 0);
	CHECK(hclamp(6.7f, 5.0f, 10.0f) == 6.7f);
	CHECK(hclamp(5.0f, 5.0f, 10.0f) == 5.0f);
	CHECK(hclamp(10.0f, 5.0f, 10.0f) == 10.0f);
	CHECK(hclamp(3.0f, 5.0f, 10.0f) == 5.0f);
	CHECK(hclamp(10.0001f, -5.0f, 10.0f) == 10.0f);
}

TEST(Util_hrand)
{
	srand(time(NULL));
	int value;
	for (int i = 0; i < 1000000; i++)
	{
		value = hrand(500, 1000);
		CHECK(value >= 500 && value < 1000);
		value = hrand(1000);
		CHECK(value >= 0 && value < 1000);
	}
	float valuef;
	for (int i = 0; i < 1000000; i++)
	{
		valuef = hrandf(54.527f, 8236.24f);
		CHECK(valuef >= 54.527f && valuef < 8236.24f);
		valuef = hrandf(4.7357f);
		CHECK(valuef >= 0 && valuef < 4.7357f);
	}
}

TEST(Util_normalize_path)
{
	hstr path = "../..\\test\\test2\\";
	hstr normalized = "../../test/test2";
	CHECK(normalize_path(path) == normalized);
	path = "./.././test\\./one_more_test/../testttt";
	normalized = "../test/testttt";
	CHECK(normalize_path(path) == normalized);
	path = "../../";
	normalized = "../..";
	CHECK(normalize_path(path) == normalized);
}
