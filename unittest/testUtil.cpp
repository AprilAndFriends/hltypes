/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <stdlib.h>
#include <time.h>

#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

TEST(Util_hrand)
{
	srand((unsigned int)time(NULL));
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

TEST(Util_hround)
{
	CHECK(hround(0.5f) == 1);
	CHECK(hround(9.5) == 10);
	CHECK(hroundf(6.5f) == 7.0f);
	CHECK(hroundd(9000.5) == 9001.0);
	// respecting 7/15 digits float/double precision
	CHECK(hround(0.4999999f) == 0);
	CHECK(hroundf(6.499999f) == 6.0f);
	CHECK(hround(9.49999999999999) == 9);
	CHECK(hroundd(9000.49999999999) == 9000.0);
	// not respecting 7/15 digits float/double precision
	CHECK(hround(0.49999999999f) == 1);
	CHECK(hround(9.499999999999999999) == 10);
	CHECK(hroundf(6.4999999999f) == 7.0f);
	CHECK(hroundd(9000.499999999999999) == 9001.0);
}

TEST(Util_hmod)
{
	CHECK(hmod(10, 20) == 10);
	CHECK(hmod(30, 20) == 10);
	CHECK(hmod(20, 20) == 0);
	CHECK(hmod(-10, 20) == 10);
	CHECK(hmod(-30, 20) == 10);
	CHECK(hmodf(1.0f, 2.5f) == 1.0f);
	CHECK(hmodf(3.0f, 2.5f) == 0.5f);
	CHECK(hmodf(2.5f, 2.5f) == 0.0f);
	CHECK(hmodf(-1.0f, 2.5f) == 1.5f);
	CHECK(hmodf(-3.5f, 2.5f) == 1.5f);
	CHECK(hmodd(1.0, 2.5) == 1.0);
	CHECK(hmodd(3.0, 2.5) == 0.5);
	CHECK(hmodd(2.5, 2.5) == 0.0);
	CHECK(hmodd(-1.0, 2.5) == 1.5);
	CHECK(hmodd(-3.5, 2.5) == 1.5);
}

TEST(Util_compare_floating_point)
{
	CHECK(heqf(10.0f, 10.01f, 0.1f));
	CHECK(!heqf(10.0f, 10.01f, 0.001f));
	CHECK(heqd(10.0, 10.01, 0.1));
	CHECK(!heqd(10.0, 10.01, 0.001));
	CHECK(hcmpf(10.0f, 10.01f, 0.1f) == 0);
	CHECK(hcmpf(10.0f, 10.01f, 0.001f) == -1);
	CHECK(hcmpf(10.01f, 10.0f, 0.001f) == 1);
	CHECK(hcmpd(10.0, 10.01, 0.1) == 0);
	CHECK(hcmpd(10.0, 10.01, 0.001) == -1);
	CHECK(hcmpd(10.01, 10.0, 0.001) == 1);
}

TEST(Util_get_basedir)
{
	CHECK(get_basedir("../../test/test2") == "../../test");
	CHECK(get_basedir("test/testttt/file.ext") == "test/testttt");
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

TEST(Util_hswap)
{
	int a = 0;
	int b = 1;
	hswap(a, b);
	CHECK(a == 1 && b == 0);
	int fs[2] = {0, 1};
	hswap(fs[0], fs[1]);
	CHECK(fs[0] == 1 && fs[1] == 0);
}

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

TEST(Util_ranges)
{
	// is_between
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
	// is_within
	CHECK(is_within(6, 0, 22));
	CHECK(!is_within(0, 0, 22));
	CHECK(!is_within(22, 0, 22));
	CHECK(!is_within(23, 0, 22));
	CHECK(!is_within(-1, 0, 22));
	CHECK(is_within(6.7f, 5.0f, 10.0f));
	CHECK(!is_within(5.0f, 5.0f, 10.0f));
	CHECK(!is_within(10.0f, 5.0f, 10.0f));
	CHECK(!is_within(3.0f, 5.0f, 10.0f));
	CHECK(!is_within(10.0001f, -5.0f, 10.0f));
	// is_in_range
	CHECK(is_in_range(6, 0, 22));
	CHECK(is_in_range(0, 0, 22));
	CHECK(!is_in_range(22, 0, 22));
	CHECK(!is_in_range(23, 0, 22));
	CHECK(!is_in_range(-1, 0, 22));
	CHECK(is_in_range(6.7f, 5.0f, 10.0f));
	CHECK(is_in_range(5.0f, 5.0f, 10.0f));
	CHECK(!is_in_range(10.0f, 5.0f, 10.0f));
	CHECK(!is_in_range(3.0f, 5.0f, 10.0f));
	CHECK(!is_in_range(10.0001f, -5.0f, 10.0f));
	// is_inside
	CHECK(is_inside(6, 0, 22));
	CHECK(!is_inside(0, 0, 22));
	CHECK(is_inside(22, 0, 22));
	CHECK(!is_inside(23, 0, 22));
	CHECK(!is_inside(-1, 0, 22));
	CHECK(is_inside(6.7f, 5.0f, 10.0f));
	CHECK(!is_inside(5.0f, 5.0f, 10.0f));
	CHECK(is_inside(10.0f, 5.0f, 10.0f));
	CHECK(!is_inside(3.0f, 5.0f, 10.0f));
	CHECK(!is_inside(10.0001f, -5.0f, 10.0f));
}

