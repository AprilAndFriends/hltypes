/// @file
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

#include <hltypes/hdeque.h>

TEST(Deque_adding)
{
	hdeque<int> a;
	a.push_back(1);
	a.push_front(0);
	a += 7;
	a << 20;
	CHECK(a[0] == 0);
	CHECK(a.front() == 0);
	CHECK(a[1] == 1);
	CHECK(a[2] == 7);
	CHECK(a[3] == 20);
	CHECK(a.back() == 20);
	CHECK(a.at(0) == 0);
	CHECK(a.at(1) == 1);
	CHECK(a.at(2) == 7);
	CHECK(a.at(3) == 20);
	CHECK(a.size() == 4);
}
