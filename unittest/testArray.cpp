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

#include <hltypes/Array.h>

/******* ARRAY *****************************************************************************/

TEST(Array_adding_01)
{
	harray<int> a;
	a.push_back(1);
	a.push_front(0);
	a.insert_at(1, 2);
	a.append(5);
	a += 7;
	a << 20;
	CHECK(a[0] == 0);
	CHECK(a[1] == 2);
	CHECK(a[2] == 1);
	CHECK(a[3] == 5);
	CHECK(a[4] == 7);
	CHECK(a[5] == 20);
	CHECK(a.at(0) == 0);
	CHECK(a.at(1) == 2);
	CHECK(a.at(2) == 1);
	CHECK(a.at(3) == 5);
	CHECK(a.at(4) == 7);
	CHECK(a.at(5) == 20);
	CHECK(a.size() == 6);
}

TEST(Array_adding_02)
{
	harray<int> a;
	a += 0;
	a += 1;
	harray<int> b;
	b.insert_at(0, a);
	CHECK(b.size() == 2);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[1]);
	b.push_back(a, 1, 1);
	CHECK(b.size() == 3);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[1]);
	CHECK(b[2] == a[1]);
	b.clear();
	b.append(a);
	CHECK(b.size() == 2);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[1]);
	b.insert_at(1, a);
	CHECK(b.size() == 4);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[0]);
	CHECK(b[2] == a[1]);
	CHECK(b[3] == a[1]);
	b.push_front(a, 1);
	CHECK(b.size() == 5);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[0]);
	CHECK(b[2] == a[0]);
	CHECK(b[3] == a[1]);
	CHECK(b[4] == a[1]);
	b.clear();
	b += a;
	CHECK(b.size() == 2);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[1]);
	b << a;
	CHECK(b.size() == 4);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[1]);
	CHECK(b[2] == a[0]);
	CHECK(b[3] == a[1]);
}

TEST(Array_adding_03)
{
	int a[2] = {0, 1};
	harray<int> b;
	b.insert_at(0, a, 2);
	CHECK(b.size() == 2);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[1]);
	b.push_back(a, 1);
	CHECK(b.size() == 3);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[1]);
	CHECK(b[2] == a[0]);
	b.clear();
	b.append(a, 2);
	CHECK(b.size() == 2);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[1]);
	b.insert_at(1, a, 2);
	CHECK(b.size() == 4);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[0]);
	CHECK(b[2] == a[1]);
	CHECK(b[3] == a[1]);
	b.push_front(a, 1);
	CHECK(b.size() == 5);
	CHECK(b[0] == a[0]);
	CHECK(b[1] == a[0]);
	CHECK(b[2] == a[0]);
	CHECK(b[3] == a[1]);
	CHECK(b[4] == a[1]);
}

TEST(Array_removing_01)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	a += 4;
	a += 5;
	a += 6;
	int b = a.remove_at(2);
	CHECK(b == 2);
	b = a.pop(1);
	CHECK(b == 1);
	b = a.pop_front();
	CHECK(b == 0);
	b = a.pop_back();
	CHECK(b == 6);
	CHECK(a.size() == 3);
	a -= 4;
	CHECK(a.size() == 2);
}

TEST(Array_removing_02)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	a += 4;
	a += 5;
	a += 6;
	harray<int> b(a);
	harray<int> c;
	c += 3;
	c += 4;
	c += 5;
	a -= c;
	CHECK(a.size() == 4);
	CHECK(a[0] == 0);
	CHECK(a[1] == 1);
	CHECK(a[2] == 2);
	CHECK(a[3] == 6);
	b.remove(c);
	CHECK(a == b);
}

TEST(Array_container)
{
	harray<int> a;
	a.push_back(0);
	a.push_back(1);
	a.push_back(2);
	a.push_back(2);
	a.push_back(3);
	CHECK(a.index_of(5) == -1);
	CHECK(a.index_of(3) == 4);
	CHECK(a.index_of(2) == 2);
	CHECK(a.contains(2) == true);
	CHECK(a.contains(8) == false);
	CHECK(a.contains(2) == a.includes(2));
	CHECK(a.contains(9) == a.includes(9));
	CHECK(a.contains(2) == a.has(2));
	CHECK(a.contains(9) == a.has(9));
	CHECK(a.contains(2) == a.has_element(2));
	CHECK(a.contains(9) == a.has_element(9));
}

TEST(Array_comparison)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	harray<int> b;
	b += 1;
	b += 2;
	b += 3;
	CHECK(a.equals(b) == false);
	CHECK((a == b) == a.equals(b));
	a.remove_at(0);
	b.remove_at(2);
	CHECK(a.equals(b) == true);
	CHECK((a == b) == a.equals(b));
	a += a.remove_at(0);
	CHECK(a.equals(b) == false);
}

TEST(Array_operations)
{
	harray<int> a;
	a += 5;
	a += 1;
	a += 2;
	a += 2;
	a += 2;
	harray<int> b;
	b += 2;
	b += 2;
	b += 2;
	b += 1;
	b += 5;
	CHECK(a == b.reversed());
	b.reverse();
	CHECK(a == b);
	a.remove_duplicates();
	CHECK(a.size() == 3);
	CHECK(a[0] == 5);
	CHECK(a[1] == 1);
	CHECK(a[2] == 2);
	b.removed_duplicates();
	CHECK(a != b);
	b.clear();
	b += 5;
	b += 1;
	b += 2;
	CHECK(a == b);
	a.sort();
	CHECK(a == b.sorted());
	CHECK(a != b);
	b.sort();
	CHECK(a == b);
	CHECK(a.min() == 1);
	CHECK(b.max() == 5);
}
#include <stdio.h>
TEST(Array_iterations)
{
	harray<int> a;
	a += 5;
	a += 6;
	a += 2;
	a += 0;
	int i = 0;
	for (int* it = a.iterate(); it; it = a.next(), i++)
	{
		CHECK((*it) == a[i]);
	}
	i = 0;
	for (int* it = a.riterate(); it; it = a.rnext(), i++)
	{
		CHECK((*it) == a[a.size() - 1 - i]);
	}
}

TEST(Array_construct)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	harray<int> b(a);
	CHECK(a == b);
}

TEST(Array_intersection)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	harray<int> b;
	b += 2;
	b += 3;
	b += 4;
	b += 5;
	harray<int> c = a.intersected(b);
	CHECK(c.size() == 2);
	CHECK(c[0] == 2);
	CHECK(c[1] == 3);
	CHECK(c == (a & b));
	a.intersect(b);
	CHECK(a == c);
}

TEST(Array_union)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	harray<int> b;
	b += 2;
	b += 3;
	b += 4;
	b += 5;
	harray<int> c = a.united(b);
	CHECK(c.size() == 6);
	CHECK(c[0] == 0);
	CHECK(c[1] == 1);
	CHECK(c[2] == 2);
	CHECK(c[3] == 3);
	CHECK(c[4] == 4);
	CHECK(c[5] == 5);
	CHECK(c == (a | b));
	a.unite(b);
	CHECK(a == c);
}

TEST(Array_difference)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	harray<int> b;
	b += 2;
	b += 3;
	b += 4;
	b += 5;
	harray<int> c = a.differenciated(b);
	CHECK(c.size() == 2);
	CHECK(c[0] == 0);
	CHECK(c[1] == 1);
	CHECK(c == (a / b));
	a.differenciate(b);
	CHECK(a == c);
}

TEST(Array_join)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	hstr b = a.join(",");
	CHECK(b == "0,1,2,3");
	b = a.join(2);
	CHECK(b == "0212223");
	b = a.join(1.5f);
	CHECK(b == "01.50000011.50000021.5000003");
}

