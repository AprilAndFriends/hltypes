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

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

TEST(Map_adding)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	a["test"] = 7;
	CHECK(a["a"] == 1);
	CHECK(a["abc"] == 2);
	CHECK(a["test"] == 7);
	CHECK(a.value_of("a") == 1);
	CHECK(a.value_of("abc") == 2);
	CHECK(a.value_of("test") == 7);
	CHECK(a(1) == "a");
	CHECK(a(2) == "abc");
	CHECK(a(7) == "test");
	CHECK(a.key_of(1) == "a");
	CHECK(a.key_of(2) == "abc");
	CHECK(a.key_of(7) == "test");
	CHECK(a.has_value(1));
	CHECK(a.has_value(2));
	CHECK(a.has_value(7));
	CHECK(!a.has_value(6));
	CHECK(a.has_key("a"));
	CHECK(a.has_key("abc"));
	CHECK(a.has_key("test"));
	CHECK(!a.has_key("key"));
	harray<int> values = a.values();
	CHECK(values.size() == 3);
	CHECK(values.contains(1));
	CHECK(values.contains(2));
	CHECK(values.contains(7));
	CHECK(a.has_values(values));
	values.remove(7);
	CHECK(a.has_values(values));
	values += 3;
	CHECK(!a.has_values(values));
	harray<hstr> keys = a.keys();
	CHECK(keys.size() == 3);
	CHECK(keys.contains("a"));
	CHECK(keys.contains("abc"));
	CHECK(keys.contains("test"));
	CHECK(!keys.contains("key"));
	CHECK(a.has_keys(keys));
	keys.remove("abc");
	CHECK(a.has_keys(keys));
	keys += "not existent";
	CHECK(!a.has_keys(keys));
}

TEST(Map_removing)
{
	hmap<hstr, int> a;
	a["A"] = 1;
	a["B"] = 2;
	a["C"] = 7;
	harray<hstr> keys = a.keys();
	CHECK(a.has_key("A"));
	a.remove_key("A");
	CHECK(!a.has_key("A"));
	CHECK(!a.has_keys(keys));
	a["A"] = 1;
	harray<int> values = a.values();
	CHECK(a.has_value(7));
	a.remove_value(7);
	CHECK(!a.has_value(7));
	CHECK(!a.has_values(values));
	a["C"] = 7;
}

TEST(Map_structure)
{
	hmap<hstr, int> a;
	a["A"] = 1;
	a["B"] = 2;
	a["C"] = 7;
	hmap<hstr, int> b = a;
	CHECK(a == b);
	CHECK(!(a != b));
	b.clear();
	b["A"] = 5;
	b["D"] = 6;
	CHECK(a["A"] == 1);
	CHECK(!a.has_key("D"));
	CHECK(a.try_get_by_key("E", 10) == 10);
	CHECK(a.try_get_by_key("E", 5) == 5);
	a.insert(b); // does not overwrite already existing keys
	CHECK(a["A"] == 1);
	CHECK(a["D"] == 6);
	b["E"] = 8;
	CHECK(!a.has_key("E"));
	a.inject(b); // overwrites already existing keys
	CHECK(a["A"] == 5);
	CHECK(a["D"] == 6);
	CHECK(a["E"] == 8);
	CHECK(a.try_get_by_key("E", 10) == 8);
}

TEST(Map_iteration)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	foreach_m (int, it, a)
	{
		CHECK(it->first == "a" && it->second == 1 || it->first == "abc" && it->second == 2);
	}
	foreach_map (hstr, int, it, a)
	{
		CHECK(it->first == "a" && it->second == 1 || it->first == "abc" && it->second == 2);
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

