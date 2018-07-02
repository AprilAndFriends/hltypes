#define _HTEST_LIB hltypes
#define _HTEST_CLASS Map
#include <htest/htest.h>
#include "hmap.h"
#include "hstring.h"

HTEST_SUITE_BEGIN

HTEST_CASE(adding)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	a["test"] = 7;
	HTEST_ASSERT(a["a"] == 1, "");
	HTEST_ASSERT(a["abc"] == 2, "");
	HTEST_ASSERT(a["test"] == 7, "");
	HTEST_ASSERT(a.valueOf("a") == 1, "");
	HTEST_ASSERT(a.valueOf("abc") == 2, "");
	HTEST_ASSERT(a.valueOf("test") == 7, "");
	HTEST_ASSERT(a(1) == "a", "");
	HTEST_ASSERT(a(2) == "abc", "");
	HTEST_ASSERT(a(7) == "test", "");
	HTEST_ASSERT(a.keyOf(1) == "a", "");
	HTEST_ASSERT(a.keyOf(2) == "abc", "");
	HTEST_ASSERT(a.keyOf(7) == "test", "");
	HTEST_ASSERT(a.hasValue(1), "");
	HTEST_ASSERT(a.hasValue(2), "");
	HTEST_ASSERT(a.hasValue(7), "");
	HTEST_ASSERT(!a.hasValue(6), "");
	HTEST_ASSERT(a.hasKey("a"), "");
	HTEST_ASSERT(a.hasKey("abc"), "");
	HTEST_ASSERT(a.hasKey("test"), "");
	HTEST_ASSERT(!a.hasKey("key"), "");
	harray<int> values = a.values();
	HTEST_ASSERT(values.size() == 3, "");
	HTEST_ASSERT(values.has(1), "");
	HTEST_ASSERT(values.has(2), "");
	HTEST_ASSERT(values.has(7), "");
	HTEST_ASSERT(a.hasAllValues(values), "");
	values.remove(7);
	HTEST_ASSERT(a.hasAllValues(values), "");
	values += 3;
	HTEST_ASSERT(!a.hasAllValues(values), "");
	harray<hstr> keys = a.keys();
	HTEST_ASSERT(keys.size() == 3, "");
	HTEST_ASSERT(keys.has("a"), "");
	HTEST_ASSERT(keys.has("abc"), "");
	HTEST_ASSERT(keys.has("test"), "");
	HTEST_ASSERT(!keys.has("key"), "");
	HTEST_ASSERT(a.hasAllKeys(keys), "");
	keys.remove("abc");
	HTEST_ASSERT(a.hasAllKeys(keys), "");
	keys += "not existent";
	HTEST_ASSERT(!a.hasAllKeys(keys), "");
}

HTEST_CASE(removing)
{
	hmap<hstr, int> a;
	a["A"] = 1;
	a["B"] = 2;
	a["C"] = 7;
	harray<hstr> keys = a.keys();
	HTEST_ASSERT(a.hasKey("A"), "");
	a.removeKey("A");
	HTEST_ASSERT(!a.hasKey("A"), "");
	HTEST_ASSERT(!a.hasAllKeys(keys), "");
	a["A"] = 1;
	harray<int> values = a.values();
	HTEST_ASSERT(a.hasValue(7), "");
	a.removeValue(7);
	HTEST_ASSERT(!a.hasValue(7), "");
	HTEST_ASSERT(!a.hasAllValues(values), "");
	a["C"] = 7;
}

HTEST_CASE(structure)
{
	hmap<hstr, int> a;
	a["A"] = 1;
	a["B"] = 2;
	a["C"] = 7;
	hmap<hstr, int> b = a;
	HTEST_ASSERT(a == b, "");
	HTEST_ASSERT(!(a != b), "");
	b.clear();
	b["A"] = 5;
	b["D"] = 6;
	HTEST_ASSERT(a["A"] == 1, "");
	HTEST_ASSERT(!a.hasKey("D"), "");
	HTEST_ASSERT(a.tryGet("E", 10) == 10, "");
	HTEST_ASSERT(a.tryGet("E", 5) == 5, "");
	a.insert(b); // does not overwrite already existing keys
	HTEST_ASSERT(a["A"] == 1, "");
	HTEST_ASSERT(a["D"] == 6, "");
	b["E"] = 8;
	HTEST_ASSERT(!a.hasKey("E"), "");
	a.inject(b); // overwrites already existing keys
	HTEST_ASSERT(a["A"] == 5, "");
	HTEST_ASSERT(a["D"] == 6, "");
	HTEST_ASSERT(a["E"] == 8, "");
	HTEST_ASSERT(a.tryGet("E", 10) == 8, "");
}

HTEST_CASE(iteration)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	foreach_m(int, it, a)
	{
		HTEST_ASSERT(it->first == "a" && it->second == 1 || it->first == "abc" && it->second == 2, "");
	}
	foreach_map(hstr, int, it, a)
	{
		HTEST_ASSERT(it->first == "a" && it->second == 1 || it->first == "abc" && it->second == 2, "");
	}
}

HTEST_CASE(comparison)
{
	hmap<hstr, int> a;
	a["a"] = 1;
	a["abc"] = 2;
	hmap<hstr, int> b;
	b["abc"] = 2;
	b["a"] = 1;
	HTEST_ASSERT(a == b, "");
	hmap<hstr, int> c;
	c["a"] = 3;
	c["abc"] = 2;
	HTEST_ASSERT(a != c, "");
	hmap<hstr, int> d;
	d["a2"] = 1;
	d["abc2"] = 2;
	HTEST_ASSERT(a != d, "");
}

HTEST_CASE(random)
{
	hmap<int, int> a;
	a[0] = 10;
	a[1] = 11;
	a[2] = 12;
	a[3] = 13;
	int value;
	int key = a.random(&value);
	HTEST_ASSERT(a.hasKey(key), "");
	HTEST_ASSERT(a.hasValue(value), "");
	hmap<int, int> b = a.random(2);
	HTEST_ASSERT(b.size() == 2, "");
	HTEST_ASSERT(a.hasAllKeys(b.keys()), "");
	HTEST_ASSERT(a.hasAllValues(b.values()), "");
	HTEST_ASSERT(!b.hasAllKeys(a.keys()), "");
	HTEST_ASSERT(!b.hasAllValues(a.values()), "");
}

static bool negative_keys(hstr const& key, int const& value) { return ((int)key < 0); }
static bool positive_values(hstr const& key, int const& value) { return (value >= 0); }
static bool over_9000_keys_values(hstr const& key, int const& value) { return ((int)key > 9000 && value > 9000); }

HTEST_CASE(match)
{
	hmap<hstr, int> a;
	a["0"] = 0;
	a["1"] = 1;
	a["2"] = -2;
	a["-500"] = -1;
	a["-600"] = 2;
	a["9001"] = 9001;
	a["9002"] = -1;
	HTEST_ASSERT(a.matchesAny(&positive_values), "");
	HTEST_ASSERT(a.matchesAny(&negative_keys), "");
	HTEST_ASSERT(!a.matchesAll(&negative_keys), "");
	HTEST_ASSERT(!a.matchesAll(&positive_values), "");
	hmap<hstr, int> c = a.findAll(&negative_keys);
	HTEST_ASSERT(c.size() == 2, "");
	HTEST_ASSERT(c.matchesAny(&negative_keys), "");
	HTEST_ASSERT(c.matchesAny(&positive_values), "");
	HTEST_ASSERT(c.matchesAll(&negative_keys), "");
	HTEST_ASSERT(!c.matchesAll(&positive_values), "");
	c = a.findAll(&positive_values);
	HTEST_ASSERT(c.size() == 4, "");
	HTEST_ASSERT(c.matchesAny(&negative_keys), "");
	HTEST_ASSERT(c.matchesAny(&positive_values), "");
	HTEST_ASSERT(!c.matchesAll(&negative_keys), "");
	HTEST_ASSERT(c.matchesAll(&positive_values), "");
	c = a.findAll(&over_9000_keys_values);
	HTEST_ASSERT(c.size() == 1, "");
	HTEST_ASSERT(!c.matchesAny(&negative_keys), "");
	HTEST_ASSERT(c.matchesAny(&positive_values), "");
	HTEST_ASSERT(c.matchesAll(&positive_values), "");
}

HTEST_CASE(cast)
{
	hmap<hstr, int> a;
	a["1"] = 0;
	a["5"] = -10;
	a["100"] = 22;
	a["-54321"] = -3;
	hmap<int, hstr> b = a.cast<int, hstr>();
	HTEST_ASSERT(b.size() == 4, "");
	harray<int> keys = a.keys().cast<int>();
	HTEST_ASSERT(b.hasAllKeys(keys), "");
	harray<hstr> values = a.values().cast<hstr>();
	HTEST_ASSERT(b.hasAllValues(values), "");
	HTEST_ASSERT(b.hasKey(1), "");
	HTEST_ASSERT(b.hasKey(5), "");
	HTEST_ASSERT(b.hasKey(100), "");
	HTEST_ASSERT(b.hasKey(-54321), "");
	HTEST_ASSERT(b[1] == "0", "");
	HTEST_ASSERT(b[5] == "-10", "");
	HTEST_ASSERT(b[100] == "22", "");
	HTEST_ASSERT(b[-54321] == "-3", "");
}

HTEST_SUITE_END
