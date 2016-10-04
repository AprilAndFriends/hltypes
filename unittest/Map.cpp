#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "hmap.h"
#include "hstring.h"

HL_UT_TEST_CLASS(Map)
{
	HL_UT_TEST_FUNCTION(adding)
	{
		hmap<hstr, int> a;
		a["a"] = 1;
		a["abc"] = 2;
		a["test"] = 7;
		HL_UT_ASSERT(a["a"] == 1, "");
		HL_UT_ASSERT(a["abc"] == 2, "");
		HL_UT_ASSERT(a["test"] == 7, "");
		HL_UT_ASSERT(a.valueOf("a") == 1, "");
		HL_UT_ASSERT(a.valueOf("abc") == 2, "");
		HL_UT_ASSERT(a.valueOf("test") == 7, "");
		HL_UT_ASSERT(a(1) == "a", "");
		HL_UT_ASSERT(a(2) == "abc", "");
		HL_UT_ASSERT(a(7) == "test", "");
		HL_UT_ASSERT(a.keyOf(1) == "a", "");
		HL_UT_ASSERT(a.keyOf(2) == "abc", "");
		HL_UT_ASSERT(a.keyOf(7) == "test", "");
		HL_UT_ASSERT(a.hasValue(1), "");
		HL_UT_ASSERT(a.hasValue(2), "");
		HL_UT_ASSERT(a.hasValue(7), "");
		HL_UT_ASSERT(!a.hasValue(6), "");
		HL_UT_ASSERT(a.hasKey("a"), "");
		HL_UT_ASSERT(a.hasKey("abc"), "");
		HL_UT_ASSERT(a.hasKey("test"), "");
		HL_UT_ASSERT(!a.hasKey("key"), "");
		harray<int> values = a.values();
		HL_UT_ASSERT(values.size() == 3, "");
		HL_UT_ASSERT(values.has(1), "");
		HL_UT_ASSERT(values.has(2), "");
		HL_UT_ASSERT(values.has(7), "");
		HL_UT_ASSERT(a.hasAllValues(values), "");
		values.remove(7);
		HL_UT_ASSERT(a.hasAllValues(values), "");
		values += 3;
		HL_UT_ASSERT(!a.hasAllValues(values), "");
		harray<hstr> keys = a.keys();
		HL_UT_ASSERT(keys.size() == 3, "");
		HL_UT_ASSERT(keys.has("a"), "");
		HL_UT_ASSERT(keys.has("abc"), "");
		HL_UT_ASSERT(keys.has("test"), "");
		HL_UT_ASSERT(!keys.has("key"), "");
		HL_UT_ASSERT(a.hasAllKeys(keys), "");
		keys.remove("abc");
		HL_UT_ASSERT(a.hasAllKeys(keys), "");
		keys += "not existent";
		HL_UT_ASSERT(!a.hasAllKeys(keys), "");
	}

	HL_UT_TEST_FUNCTION(removing)
	{
		hmap<hstr, int> a;
		a["A"] = 1;
		a["B"] = 2;
		a["C"] = 7;
		harray<hstr> keys = a.keys();
		HL_UT_ASSERT(a.hasKey("A"), "");
		a.removeKey("A");
		HL_UT_ASSERT(!a.hasKey("A"), "");
		HL_UT_ASSERT(!a.hasAllKeys(keys), "");
		a["A"] = 1;
		harray<int> values = a.values();
		HL_UT_ASSERT(a.hasValue(7), "");
		a.removeValue(7);
		HL_UT_ASSERT(!a.hasValue(7), "");
		HL_UT_ASSERT(!a.hasAllValues(values), "");
		a["C"] = 7;
	}

	HL_UT_TEST_FUNCTION(structure)
	{
		hmap<hstr, int> a;
		a["A"] = 1;
		a["B"] = 2;
		a["C"] = 7;
		hmap<hstr, int> b = a;
		HL_UT_ASSERT(a == b, "");
		HL_UT_ASSERT(!(a != b), "");
		b.clear();
		b["A"] = 5;
		b["D"] = 6;
		HL_UT_ASSERT(a["A"] == 1, "");
		HL_UT_ASSERT(!a.hasKey("D"), "");
		HL_UT_ASSERT(a.tryGet("E", 10) == 10, "");
		HL_UT_ASSERT(a.tryGet("E", 5) == 5, "");
		a.insert(b); // does not overwrite already existing keys
		HL_UT_ASSERT(a["A"] == 1, "");
		HL_UT_ASSERT(a["D"] == 6, "");
		b["E"] = 8;
		HL_UT_ASSERT(!a.hasKey("E"), "");
		a.inject(b); // overwrites already existing keys
		HL_UT_ASSERT(a["A"] == 5, "");
		HL_UT_ASSERT(a["D"] == 6, "");
		HL_UT_ASSERT(a["E"] == 8, "");
		HL_UT_ASSERT(a.tryGet("E", 10) == 8, "");
	}

	HL_UT_TEST_FUNCTION(iteration)
	{
		hmap<hstr, int> a;
		a["a"] = 1;
		a["abc"] = 2;
		foreach_m(int, it, a)
		{
			HL_UT_ASSERT(it->first == "a" && it->second == 1 || it->first == "abc" && it->second == 2, "");
		}
		foreach_map(hstr, int, it, a)
		{
			HL_UT_ASSERT(it->first == "a" && it->second == 1 || it->first == "abc" && it->second == 2, "");
		}
	}

	HL_UT_TEST_FUNCTION(comparison)
	{
		hmap<hstr, int> a;
		a["a"] = 1;
		a["abc"] = 2;
		hmap<hstr, int> b;
		b["abc"] = 2;
		b["a"] = 1;
		HL_UT_ASSERT(a == b, "");
		hmap<hstr, int> c;
		c["a"] = 3;
		c["abc"] = 2;
		HL_UT_ASSERT(a != c, "");
		hmap<hstr, int> d;
		d["a2"] = 1;
		d["abc2"] = 2;
		HL_UT_ASSERT(a != d, "");
	}

	HL_UT_TEST_FUNCTION(random)
	{
		hmap<int, int> a;
		a[0] = 10;
		a[1] = 11;
		a[2] = 12;
		a[3] = 13;
		int value;
		int key = a.random(&value);
		HL_UT_ASSERT(a.hasKey(key), "");
		HL_UT_ASSERT(a.hasValue(value), "");
		hmap<int, int> b = a.random(2);
		HL_UT_ASSERT(b.size() == 2, "");
		HL_UT_ASSERT(a.hasAllKeys(b.keys()), "");
		HL_UT_ASSERT(a.hasAllValues(b.values()), "");
		HL_UT_ASSERT(!b.hasAllKeys(a.keys()), "");
		HL_UT_ASSERT(!b.hasAllValues(a.values()), "");
	}

	static bool negative_keys(hstr const& key, int const& value) { return ((int)key < 0); }
	static bool positive_values(hstr const& key, int const& value) { return (value >= 0); }
	static bool over_9000_keys_values(hstr const& key, int const& value) { return ((int)key > 9000 && value > 9000); }

	HL_UT_TEST_FUNCTION(match)
	{
		hmap<hstr, int> a;
		a["0"] = 0;
		a["1"] = 1;
		a["2"] = -2;
		a["-500"] = -1;
		a["-600"] = 2;
		a["9001"] = 9001;
		a["9002"] = -1;
		HL_UT_ASSERT(a.matchesAny(&positive_values), "");
		HL_UT_ASSERT(a.matchesAny(&negative_keys), "");
		HL_UT_ASSERT(!a.matchesAll(&negative_keys), "");
		HL_UT_ASSERT(!a.matchesAll(&positive_values), "");
		hmap<hstr, int> c = a.findAll(&negative_keys);
		HL_UT_ASSERT(c.size() == 2, "");
		HL_UT_ASSERT(c.matchesAny(&negative_keys), "");
		HL_UT_ASSERT(c.matchesAny(&positive_values), "");
		HL_UT_ASSERT(c.matchesAll(&negative_keys), "");
		HL_UT_ASSERT(!c.matchesAll(&positive_values), "");
		c = a.findAll(&positive_values);
		HL_UT_ASSERT(c.size() == 4, "");
		HL_UT_ASSERT(c.matchesAny(&negative_keys), "");
		HL_UT_ASSERT(c.matchesAny(&positive_values), "");
		HL_UT_ASSERT(!c.matchesAll(&negative_keys), "");
		HL_UT_ASSERT(c.matchesAll(&positive_values), "");
		c = a.findAll(&over_9000_keys_values);
		HL_UT_ASSERT(c.size() == 1, "");
		HL_UT_ASSERT(!c.matchesAny(&negative_keys), "");
		HL_UT_ASSERT(c.matchesAny(&positive_values), "");
		HL_UT_ASSERT(c.matchesAll(&positive_values), "");
	}

	HL_UT_TEST_FUNCTION(cast)
	{
		hmap<hstr, int> a;
		a["1"] = 0;
		a["5"] = -10;
		a["100"] = 22;
		a["-54321"] = -3;
		hmap<int, hstr> b = a.cast<int, hstr>();
		HL_UT_ASSERT(b.size() == 4, "");
		harray<int> keys = a.keys().cast<int>();
		HL_UT_ASSERT(b.hasAllKeys(keys), "");
		harray<hstr> values = a.values().cast<hstr>();
		HL_UT_ASSERT(b.hasAllValues(values), "");
		HL_UT_ASSERT(b.hasKey(1), "");
		HL_UT_ASSERT(b.hasKey(5), "");
		HL_UT_ASSERT(b.hasKey(100), "");
		HL_UT_ASSERT(b.hasKey(-54321), "");
		HL_UT_ASSERT(b[1] == "0", "");
		HL_UT_ASSERT(b[5] == "-10", "");
		HL_UT_ASSERT(b[100] == "22", "");
		HL_UT_ASSERT(b[-54321] == "-3", "");
	}
}
