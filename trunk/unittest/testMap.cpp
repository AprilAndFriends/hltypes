/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Domagoj Cerjan
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

/******* MAP *******************************************************************************/

TEST(Map_structure_01)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	a["test"] = 7;
	CHECK(a["a"] == 1);
	CHECK(a["abc"] == 2);
	CHECK(a["test"] == 7);
	CHECK(a.has_key("a") == true);
	CHECK(a.has_key("abc") == true);
	CHECK(a.has_key("test") == true);
	CHECK(a.has_value(1) == true);
	CHECK(a.has_value(2) == true);
	CHECK(a.has_value(7) == true);
	harray<hstr> keys = a.keys();
	CHECK(keys.contains("a") == true);
	CHECK(keys.contains("abc") == true);
	CHECK(keys.contains("test") == true);
	CHECK(a.has_keys(keys) == true);
	keys.remove("abc");
	CHECK(a.has_keys(keys) == true);
	keys += "not existent";
	CHECK(a.has_keys(keys) == false);
	harray<int> values = a.values();
	CHECK(values.contains(1) == true);
	CHECK(values.contains(2) == true);
	CHECK(values.contains(7) == true);
	CHECK(a.has_values(values) == true);
	values.remove(7);
	CHECK(a.has_values(values) == true);
	values += 3;
	CHECK(a.has_values(values) == false);
}

TEST(Map_structure_02)
{
	hmap<hstr, int> a;
	a["A"] = 1;
	a["B"] = 2;
	a["C"] = 7;
	harray<hstr> keys = a.keys();
	CHECK(a.has_key("A") == true);
	a.remove_key("A");
	CHECK(a.has_key("A") == false);
	CHECK(a.has_keys(keys) == false);
	a["A"] = 1;
	harray<int> values = a.values();
	CHECK(a.has_value(7) == true);
	a.remove_value(7);
	CHECK(a.has_value(7) == false);
	CHECK(a.has_values(values) == false);
	a["C"] = 7;
}

TEST(Map_structure_03)
{
	hmap<hstr, int> a;
	a["A"] = 1;
	a["B"] = 2;
	a["C"] = 7;
	hmap<hstr, int> b = a;
	CHECK(a == b);
	CHECK(!(a != b));
}

TEST(Map_iteration)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	foreach_m (int, it, a)
	{
		CHECK(it->first == "a" && it->second == 1 ||
			it->first == "abc" && it->second == 2);
	}
}

TEST(Map_comparison)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	hmap<hstr, int> b;
	b["abc"] = 2;
	b["a"] = 1;
	CHECK(a == b);
	hmap<hstr, int> c;
	c["a"] = 3;
	c["abc"] = 2;
	CHECK(a != c);
	hmap<hstr, int> d;
	d["a2"] = 1;
	d["abc2"] = 2;
	CHECK(a != d);
}
#include <stdio.h>
TEST(Map_random)
{
	hmap<int, int> a;
	a[0] = 10;
	a[1] = 11;
	a[2] = 12;
	a[3] = 13;
	int value;
	int key = a.random(&value);
	CHECK(a.has_key(key));
	CHECK(a.has_value(value));
	hmap<int, int> b = a.random(2);
	CHECK(b.size() == 2);
	CHECK(a.has_keys(b.keys()));
	CHECK(a.has_values(b.values()));
	CHECK(!b.has_keys(a.keys()));
	CHECK(!b.has_values(a.values()));
}

bool negative_keys(hstr key, int value) { return ((int)key < 0); }
bool positive_values(hstr key, int value) { return (value >= 0); }
bool over_9000_keys_values(hstr key, int value) { return ((int)key > 9000 && value > 9000); }

TEST(Map_match)
{
	hmap<hstr, int> a;
	a["0"] = 0;
	a["1"] = 1;
	a["2"] = -2;
	a["-500"] = -1;
	a["-600"] = 2;
	a["9001"] = 9001;
	a["9002"] = -1;
	CHECK(a.matches_any(&positive_values));
	CHECK(a.matches_any(&negative_keys));
	CHECK(!a.matches_all(&negative_keys));
	CHECK(!a.matches_all(&positive_values));
	hmap<hstr, int> c = a.find_all(&negative_keys);
	CHECK(c.size() == 2);
	CHECK(c.matches_any(&negative_keys));
	CHECK(c.matches_any(&positive_values));
	CHECK(c.matches_all(&negative_keys));
	CHECK(!c.matches_all(&positive_values));
	c = a.find_all(&positive_values);
	CHECK(c.size() == 4);
	CHECK(c.matches_any(&negative_keys));
	CHECK(c.matches_any(&positive_values));
	CHECK(!c.matches_all(&negative_keys));
	CHECK(c.matches_all(&positive_values));
	c = a.find_all(&over_9000_keys_values);
	CHECK(c.size() == 1);
	CHECK(!c.matches_any(&negative_keys));
	CHECK(c.matches_any(&positive_values));
	CHECK(c.matches_all(&positive_values));
}

TEST(Map_cast)
{
	hmap<hstr, int> a;
	a["1"] = 0;
	a["5"] = -10;
	a["100"] = 22;
	a["-54321"] = -3;
	hmap<int, hstr> b = a.cast<int, hstr>();
	CHECK(b.size() == 4);
	harray<int> keys = a.keys().cast<int>();
	CHECK(b.has_keys(keys));
	harray<hstr> values = a.values().cast<hstr>();
	CHECK(b.has_values(values));
	CHECK(b.has_key(1));
	CHECK(b.has_key(5));
	CHECK(b.has_key(100));
	CHECK(b.has_key(-54321));
	CHECK(b[1] == "0");
	CHECK(b[5] == "-10");
	CHECK(b[100] == "22");
	CHECK(b[-54321] == "-3");
}

