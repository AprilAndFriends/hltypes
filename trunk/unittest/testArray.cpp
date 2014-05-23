/// @file
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

#include <hltypes/harray.h>

TEST(Array_adding_01)
{
	harray<int> a;
	a.push_back(1);
	a.push_front(0);
	a.insert_at(1, 2);
	a.append(5);
	a.add(15);
	a += 7;
	a << 20;
	CHECK(a[0] == 0);
	CHECK(a.front() == 0);
	CHECK(a.first() == 0);
	CHECK(a[1] == 2);
	CHECK(a[2] == 1);
	CHECK(a[3] == 5);
	CHECK(a[4] == 15);
	CHECK(a[5] == 7);
	CHECK(a[6] == 20);
	CHECK(a.back() == 20);
	CHECK(a.last() == 20);
	CHECK(a.at(0) == 0);
	CHECK(a.at(1) == 2);
	CHECK(a.at(2) == 1);
	CHECK(a.at(3) == 5);
	CHECK(a.at(4) == 15);
	CHECK(a.at(5) == 7);
	CHECK(a.at(6) == 20);
	CHECK(a.size() == 7);
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
	a += 7;
	a += 8;
	a += 9;
	a += 10;
	int b = a.remove_at(2);
	CHECK(b == 2);
	b = a.pop(1);
	CHECK(b == 1);
	b = a.pop_front();
	CHECK(b == 0);
	b = a.pop_back();
	CHECK(b == 10);
	CHECK(a.size() == 7);
	a -= 4;
	CHECK(a.size() == 6);
	harray<int> c = a.pop_front(2);
	CHECK(a.size() == 4);
	CHECK(c.size() == 2);
	CHECK(c[0] == 3);
	CHECK(c[1] == 5);
	c = a.pop_back(3);
	CHECK(a.size() == 1);
	CHECK(c.size() == 3);
	CHECK(c[0] == 7);
	CHECK(c[1] == 8);
	CHECK(c[2] == 9);
	c.remove_at(1, 2);
	CHECK(c.size() == 1);
	CHECK(c[0] == 7);
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
	harray<int> indexes;
	a.push_back(0);
	a.push_back(1);
	a.push_back(2);
	a.push_back(2);
	a.push_back(3);
	CHECK(a.index_of(5) == -1);
	CHECK(a.index_of(3) == 4);
	CHECK(a.index_of(2) == 2);
	indexes = a.indexes_of(2);
	CHECK(indexes.size() == 2);
	CHECK(indexes[0] == 2);
	CHECK(indexes[1] == 3);
	indexes = a.indexes_of(3);
	CHECK(indexes.size() == 1);
	CHECK(indexes[0] == 4);
	indexes = a.indexes_of(9);
	CHECK(indexes.size() == 0);
	CHECK(a.contains(2) == true);
	CHECK(a.contains(8) == false);
	CHECK(a.contains(2) == a.includes(2));
	CHECK(a.contains(9) == a.includes(9));
	CHECK(a.contains(2) == a.has(2));
	CHECK(a.contains(9) == a.has(9));
	CHECK(a.contains(2) == a.has_element(2));
	CHECK(a.contains(9) == a.has_element(9));
	CHECK(a.count(0) == 1);
	CHECK(a.count(2) == 2);
	CHECK(a.count(5) == 0);
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
	harray<bool> c;
	c += true;
	c += false;
	c += true;
	CHECK(c.at(0) == true);
	CHECK(c.at(1) == false);
	CHECK(c.at(2) == true);
}

TEST(Array_iterations)
{
	harray<int> a;
	a += 5;
	a += 6;
	a += 2;
	a += 0;
	int i = 0;
	foreach (int, it, a)
	{
		CHECK((*it) == a[i]);
		i++;
	}
	i = 0;
	foreach_r (int, it, a)
	{
		CHECK((*it) == a[a.size() - 1 - i]);
		i++;
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
	harray<int> c = a;
	CHECK(a == c);
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
	a.unite(4);
	CHECK(a == c);
	a.unite(999);
	CHECK(a != c);
}

TEST(Array_difference)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 2;
	a += 3;
	a += 3;
	a += 4;
	harray<int> b;
	b += 2;
	b += 2;
	b += 3;
	b += 4;
	b += 4;
	b += 5;
	harray<int> c = a.differentiated(b);
	CHECK(c.size() == 3);
	CHECK(c[0] == 0);
	CHECK(c[1] == 1);
	CHECK(c[2] == 3);
	CHECK(c == (a / b));
	a.differentiate(b);
	CHECK(a == c);
	harray<hstr> a1;
	a1 += "a";
	a1 += "a";
	harray<hstr> a2;
	a2 += "a";
	a2 += "a";
	CHECK(a1 == a2);
	CHECK((a1 / a2).size() == 0);
}

TEST(Array_random)
{
	harray<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	int i = a.random();
	CHECK(a.contains(i));
	harray<int> b = a.random(2);
	CHECK(b.size() == 2);
	CHECK(a.contains(b));
	CHECK(!b.contains(a));
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

bool negative(int i) { return (i < 0); }
bool positive(int i) { return (i >= 0); }
bool over_9000(int i) { return (i > 9000); }

TEST(Array_match)
{
	int* result;
	harray<int> a;
	a += 0;
	a += -1;
	a += 2;
	a += -3;
	CHECK(a.matches_any(&positive) == true);
	CHECK(a.matches_any(&negative) == true);
	CHECK(a.matches_all(&negative) == false);
	CHECK(a.matches_all(&positive) == false);
	CHECK(a.find_first(&negative) != NULL);
	CHECK(*a.find_first(&negative) == -1);
	CHECK(a.find_first(&positive) != NULL);
	CHECK(*a.find_first(&positive) == 0);
	CHECK(a.find_first(&over_9000) == NULL);
	harray<int> c = a.find_all(&negative);
	CHECK(c.size() == 2 && c[0] == -1 && c[1] == -3);
	CHECK(c.matches_any(&negative) == true);
	CHECK(c.matches_any(&positive) == false);
	CHECK(c.matches_all(&negative) == true);
	CHECK(c.matches_all(&positive) == false);
}

TEST(Array_cast)
{
	harray<int> a;
	a += 0;
	a += -1;
	a += 2;
	a += -3;
	harray<hstr> b = a.cast<hstr>();
	CHECK(b.size() == 4);
	CHECK(b[0] == "0");
	CHECK(b[1] == "-1");
	CHECK(b[2] == "2");
	CHECK(b[3] == "-3");
}

