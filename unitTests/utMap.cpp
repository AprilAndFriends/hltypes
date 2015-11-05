#include "generated/aprilUT.h"
#include "hmap.h"
#include "hstring.h"

namespace testMap
{
	void testAdding()
	{
		hmap<hstr, int> a;
		a["a"] = 1;
		a["abc"] = 2;
		a["test"] = 7;
		assertTrue(a["a"] == 1, "hmap - adding");
		assertTrue(a["abc"] == 2, "hmap - adding");
		assertTrue(a["test"] == 7, "hmap - adding");
		assertTrue(a.valueOf("a") == 1, "hmap - adding");
		assertTrue(a.valueOf("abc") == 2, "hmap - adding");
		assertTrue(a.valueOf("test") == 7, "hmap - adding");
		assertTrue(a(1) == "a", "hmap - adding");
		assertTrue(a(2) == "abc", "hmap - adding");
		assertTrue(a(7) == "test", "hmap - adding");
		assertTrue(a.keyOf(1) == "a", "hmap - adding");
		assertTrue(a.keyOf(2) == "abc", "hmap - adding");
		assertTrue(a.keyOf(7) == "test", "hmap - adding");
		assertTrue(a.hasValue(1), "hmap - adding");
		assertTrue(a.hasValue(2), "hmap - adding");
		assertTrue(a.hasValue(7), "hmap - adding");
		assertTrue(!a.hasValue(6), "hmap - adding");
		assertTrue(a.hasKey("a"), "hmap - adding");
		assertTrue(a.hasKey("abc"), "hmap - adding");
		assertTrue(a.hasKey("test"), "hmap - adding");
		assertTrue(!a.hasKey("key"), "hmap - adding");
		harray<int> values = a.values();
		assertTrue(values.size() == 3, "hmap - adding");
		assertTrue(values.has(1), "hmap - adding");
		assertTrue(values.has(2), "hmap - adding");
		assertTrue(values.has(7), "hmap - adding");
		assertTrue(a.hasAllValues(values), "hmap - adding");
		values.remove(7);
		assertTrue(a.hasAllValues(values), "hmap - adding");
		values += 3;
		assertTrue(!a.hasAllValues(values), "hmap - adding");
		harray<hstr> keys = a.keys();
		assertTrue(keys.size() == 3, "hmap - adding");
		assertTrue(keys.has("a"), "hmap - adding");
		assertTrue(keys.has("abc"), "hmap - adding");
		assertTrue(keys.has("test"), "hmap - adding");
		assertTrue(!keys.has("key"), "hmap - adding");
		assertTrue(a.hasAllKeys(keys), "hmap - adding");
		keys.remove("abc");
		assertTrue(a.hasAllKeys(keys), "hmap - adding");
		keys += "not existent";
		assertTrue(!a.hasAllKeys(keys), "hmap - adding");
	}

	void testRemoving()
	{
		hmap<hstr, int> a;
		a["A"] = 1;
		a["B"] = 2;
		a["C"] = 7;
		harray<hstr> keys = a.keys();
		assertTrue(a.hasKey("A"), "hmap - removing");
		a.removeKey("A");
		assertTrue(!a.hasKey("A"), "hmap - removing");
		assertTrue(!a.hasAllKeys(keys), "hmap - removing");
		a["A"] = 1;
		harray<int> values = a.values();
		assertTrue(a.hasValue(7), "hmap - removing");
		a.removeValue(7);
		assertTrue(!a.hasValue(7), "hmap - removing");
		assertTrue(!a.hasAllValues(values), "hmap - removing");
		a["C"] = 7;
	}

	void testStructure()
	{
		hmap<hstr, int> a;
		a["A"] = 1;
		a["B"] = 2;
		a["C"] = 7;
		hmap<hstr, int> b = a;
		assertTrue(a == b, "hmap - structure");
		assertTrue(!(a != b), "hmap - structure");
		b.clear();
		b["A"] = 5;
		b["D"] = 6;
		assertTrue(a["A"] == 1, "hmap - structure");
		assertTrue(!a.hasKey("D"), "hmap - structure");
		assertTrue(a.tryGet("E", 10) == 10, "hmap - structure");
		assertTrue(a.tryGet("E", 5) == 5, "hmap - structure");
		a.insert(b); // does not overwrite already existing keys
		assertTrue(a["A"] == 1, "hmap - structure");
		assertTrue(a["D"] == 6, "hmap - structure");
		b["E"] = 8;
		assertTrue(!a.hasKey("E"), "hmap - structure");
		a.inject(b); // overwrites already existing keys
		assertTrue(a["A"] == 5, "hmap - structure");
		assertTrue(a["D"] == 6, "hmap - structure");
		assertTrue(a["E"] == 8, "hmap - structure");
		assertTrue(a.tryGet("E", 10) == 8, "hmap - structure");
	}

	void testIteration()
	{
		hmap<hstr, int> a;
		a["a"] = 1;
		a["abc"] = 2;
		foreach_m(int, it, a)
		{
			assertTrue(it->first == "a" && it->second == 1 || it->first == "abc" && it->second == 2, "hmap - iteration");
		}
		foreach_map(hstr, int, it, a)
		{
			assertTrue(it->first == "a" && it->second == 1 || it->first == "abc" && it->second == 2, "hmap - iteration");
		}
	}

	void testComparison()
	{
		hmap<hstr, int> a;
		a["a"] = 1;
		a["abc"] = 2;
		hmap<hstr, int> b;
		b["abc"] = 2;
		b["a"] = 1;
		assertTrue(a == b, "hmap - comparison");
		hmap<hstr, int> c;
		c["a"] = 3;
		c["abc"] = 2;
		assertTrue(a != c, "hmap - comparison");
		hmap<hstr, int> d;
		d["a2"] = 1;
		d["abc2"] = 2;
		assertTrue(a != d, "hmap - comparison");
	}

	void testRandom()
	{
		hmap<int, int> a;
		a[0] = 10;
		a[1] = 11;
		a[2] = 12;
		a[3] = 13;
		int value;
		int key = a.random(&value);
		assertTrue(a.hasKey(key), "hmap - random");
		assertTrue(a.hasValue(value), "hmap - random");
		hmap<int, int> b = a.random(2);
		assertTrue(b.size() == 2, "hmap - random");
		assertTrue(a.hasAllKeys(b.keys()), "hmap - random");
		assertTrue(a.hasAllValues(b.values()), "hmap - random");
		assertTrue(!b.hasAllKeys(a.keys()), "hmap - random");
		assertTrue(!b.hasAllValues(a.values()), "hmap - random");
	}

	bool negative_keys(hstr key, int value) { return ((int)key < 0); }
	bool positive_values(hstr key, int value) { return (value >= 0); }
	bool over_9000_keys_values(hstr key, int value) { return ((int)key > 9000 && value > 9000); }

	void testMatch()
	{
		hmap<hstr, int> a;
		a["0"] = 0;
		a["1"] = 1;
		a["2"] = -2;
		a["-500"] = -1;
		a["-600"] = 2;
		a["9001"] = 9001;
		a["9002"] = -1;
		assertTrue(a.matchesAny(&positive_values), "hmap - match");
		assertTrue(a.matchesAny(&negative_keys), "hmap - match");
		assertTrue(!a.matchesAll(&negative_keys), "hmap - match");
		assertTrue(!a.matchesAll(&positive_values), "hmap - match");
		hmap<hstr, int> c = a.findAll(&negative_keys);
		assertTrue(c.size() == 2, "hmap - match");
		assertTrue(c.matchesAny(&negative_keys), "hmap - match");
		assertTrue(c.matchesAny(&positive_values), "hmap - match");
		assertTrue(c.matchesAll(&negative_keys), "hmap - match");
		assertTrue(!c.matchesAll(&positive_values), "hmap - match");
		c = a.findAll(&positive_values);
		assertTrue(c.size() == 4, "hmap - match");
		assertTrue(c.matchesAny(&negative_keys), "hmap - match");
		assertTrue(c.matchesAny(&positive_values), "hmap - match");
		assertTrue(!c.matchesAll(&negative_keys), "hmap - match");
		assertTrue(c.matchesAll(&positive_values), "hmap - match");
		c = a.findAll(&over_9000_keys_values);
		assertTrue(c.size() == 1, "hmap - match");
		assertTrue(!c.matchesAny(&negative_keys), "hmap - match");
		assertTrue(c.matchesAny(&positive_values), "hmap - match");
		assertTrue(c.matchesAll(&positive_values), "hmap - match");
	}

	void testCast()
	{
		hmap<hstr, int> a;
		a["1"] = 0;
		a["5"] = -10;
		a["100"] = 22;
		a["-54321"] = -3;
		hmap<int, hstr> b = a.cast<int, hstr>();
		assertTrue(b.size() == 4, "hmap - cast");
		harray<int> keys = a.keys().cast<int>();
		assertTrue(b.hasAllKeys(keys), "hmap - cast");
		harray<hstr> values = a.values().cast<hstr>();
		assertTrue(b.hasAllValues(values), "hmap - cast");
		assertTrue(b.hasKey(1), "hmap - cast");
		assertTrue(b.hasKey(5), "hmap - cast");
		assertTrue(b.hasKey(100), "hmap - cast");
		assertTrue(b.hasKey(-54321), "hmap - cast");
		assertTrue(b[1] == "0", "hmap - cast");
		assertTrue(b[5] == "-10", "hmap - cast");
		assertTrue(b[100] == "22", "hmap - cast");
		assertTrue(b[-54321] == "-3", "hmap - cast");
	}
}
