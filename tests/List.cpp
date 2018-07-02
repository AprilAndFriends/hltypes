#define _HTEST_LIB hltypes
#define _HTEST_CLASS List
#include <htest/htest.h>
#include "hlist.h"

HTEST_SUITE_BEGIN

HTEST_CASE(adding)
{
	hlist<int> a;
	a.add(1);
	a.addFirst(0);
	a.insertAt(1, 2);
	a.add(5);
	a.add(15);
	a += 7;
	a << 20;
	HTEST_ASSERT(a[0] == 0, "");
	HTEST_ASSERT(a.first() == 0, "");
	HTEST_ASSERT(a[1] == 2, "");
	HTEST_ASSERT(a[2] == 1, "");
	HTEST_ASSERT(a[3] == 5, "");
	HTEST_ASSERT(a[4] == 15, "");
	HTEST_ASSERT(a[5] == 7, "");
	HTEST_ASSERT(a[6] == 20, "");
	HTEST_ASSERT(a.last() == 20, "");
	HTEST_ASSERT(a.at(0) == 0, "");
	HTEST_ASSERT(a.at(1) == 2, "");
	HTEST_ASSERT(a.at(2) == 1, "");
	HTEST_ASSERT(a.at(3) == 5, "");
	HTEST_ASSERT(a.at(4) == 15, "");
	HTEST_ASSERT(a.at(5) == 7, "");
	HTEST_ASSERT(a.at(6) == 20, "");
	HTEST_ASSERT(a.size() == 7, "");
}

HTEST_CASE(adding2)
{
	hlist<int> a;
	a += 0;
	a += 1;
	hlist<int> b;
	b.insertAt(0, a);
	HTEST_ASSERT(b.size() == 2, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[1], "");
	b.add(a, 1, 1);
	HTEST_ASSERT(b.size() == 3, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[1], "");
	HTEST_ASSERT(b[2] == a[1], "");
	b.clear();
	b.add(a);
	HTEST_ASSERT(b.size() == 2, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[1], "");
	b.insertAt(1, a);
	HTEST_ASSERT(b.size() == 4, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[0], "");
	HTEST_ASSERT(b[2] == a[1], "");
	HTEST_ASSERT(b[3] == a[1], "");
	b.addFirst(a, 1);
	HTEST_ASSERT(b.size() == 5, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[0], "");
	HTEST_ASSERT(b[2] == a[0], "");
	HTEST_ASSERT(b[3] == a[1], "");
	HTEST_ASSERT(b[4] == a[1], "");
	b.clear();
	b += a;
	HTEST_ASSERT(b.size() == 2, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[1], "");
	b << a;
	HTEST_ASSERT(b.size() == 4, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[1], "");
	HTEST_ASSERT(b[2] == a[0], "");
	HTEST_ASSERT(b[3] == a[1], "");
}

HTEST_CASE(adding3)
{
	int a[2] = { 0, 1 };
	hlist<int> b;
	b.insertAt(0, a, 2);
	HTEST_ASSERT(b.size() == 2, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[1], "");
	b.add(a, 1);
	HTEST_ASSERT(b.size() == 3, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[1], "");
	HTEST_ASSERT(b[2] == a[0], "");
	b.clear();
	b.add(a, 2);
	HTEST_ASSERT(b.size() == 2, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[1], "");
	b.insertAt(1, a, 2);
	HTEST_ASSERT(b.size() == 4, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[0], "");
	HTEST_ASSERT(b[2] == a[1], "");
	HTEST_ASSERT(b[3] == a[1], "");
	b.addFirst(a, 1);
	HTEST_ASSERT(b.size() == 5, "");
	HTEST_ASSERT(b[0] == a[0], "");
	HTEST_ASSERT(b[1] == a[0], "");
	HTEST_ASSERT(b[2] == a[0], "");
	HTEST_ASSERT(b[3] == a[1], "");
	HTEST_ASSERT(b[4] == a[1], "");
}

HTEST_CASE(removing1)
{
	hlist<int> a;
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
	int b = a.removeAt(2);
	HTEST_ASSERT(b == 2, "");
	b = a.removeAt(1);
	HTEST_ASSERT(b == 1, "");
	b = a.removeFirst();
	HTEST_ASSERT(b == 0, "");
	b = a.removeLast();
	HTEST_ASSERT(b == 10, "");
	HTEST_ASSERT(a.size() == 7, "");
	a -= 4;
	HTEST_ASSERT(a.size() == 6, "");
	hlist<int> c = a.removeFirst(2);
	HTEST_ASSERT(a.size() == 4, "");
	HTEST_ASSERT(c.size() == 2, "");
	HTEST_ASSERT(c[0] == 3, "");
	HTEST_ASSERT(c[1] == 5, "");
	c = a.removeLast(3);
	HTEST_ASSERT(a.size() == 1, "");
	HTEST_ASSERT(c.size() == 3, "");
	HTEST_ASSERT(c[0] == 7, "");
	HTEST_ASSERT(c[1] == 8, "");
	HTEST_ASSERT(c[2] == 9, "");
	c.removeAt(1, 2);
	HTEST_ASSERT(c.size() == 1, "");
	HTEST_ASSERT(c[0] == 7, "");
}

HTEST_CASE(removing2)
{
	hlist<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	a += 4;
	a += 5;
	a += 6;
	hlist<int> b(a);
	hlist<int> c;
	c += 3;
	c += 4;
	c += 5;
	a -= c;
	HTEST_ASSERT(a.size() == 4, "");
	HTEST_ASSERT(a[0] == 0, "");
	HTEST_ASSERT(a[1] == 1, "");
	HTEST_ASSERT(a[2] == 2, "");
	HTEST_ASSERT(a[3] == 6, "");
	b.remove(c);
	HTEST_ASSERT(a == b, "");
}

HTEST_CASE(container)
{
	hlist<int> a;
	hlist<int> indexes;
	a.add(0);
	a.add(1);
	a.add(2);
	a.add(2);
	a.add(3);
	HTEST_ASSERT(a.indexOf(5) == -1, "");
	HTEST_ASSERT(a.indexOf(3) == 4, "");
	HTEST_ASSERT(a.indexOf(2) == 2, "");
	indexes = a.indicesOf(2);
	HTEST_ASSERT(indexes.size() == 2, "");
	HTEST_ASSERT(indexes[0] == 2, "");
	HTEST_ASSERT(indexes[1] == 3, "");
	indexes = a.indicesOf(3);
	HTEST_ASSERT(indexes.size() == 1, "");
	HTEST_ASSERT(indexes[0] == 4, "");
	indexes = a.indicesOf(9);
	HTEST_ASSERT(indexes.size() == 0, "");
	HTEST_ASSERT(a.has(2) == true, "");
	HTEST_ASSERT(a.has(8) == false, "");
	HTEST_ASSERT(a.count(0) == 1, "");
	HTEST_ASSERT(a.count(2) == 2, "");
	HTEST_ASSERT(a.count(5) == 0, "");
}

HTEST_CASE(comparison)
{
	hlist<int> a;
	a += 0;
	a += 1;
	a += 2;
	hlist<int> b;
	b += 1;
	b += 2;
	b += 3;
	HTEST_ASSERT(a.equals(b) == false, "");
	HTEST_ASSERT((a == b) == a.equals(b), "");
	a.removeAt(0);
	b.removeAt(2);
	HTEST_ASSERT(a.equals(b) == true, "");
	HTEST_ASSERT((a == b) == a.equals(b), "");
	a += a.removeAt(0);
	HTEST_ASSERT(a.equals(b) == false, "");
}

HTEST_CASE(operations)
{
	hlist<int> a;
	a += 5;
	a += 1;
	a += 2;
	a += 2;
	a += 2;
	hlist<int> b;
	b += 2;
	b += 2;
	b += 2;
	b += 1;
	b += 5;
	HTEST_ASSERT(a == b.reversed(), "reversed()");
	b.reverse();
	HTEST_ASSERT(a == b, "reverse()");
	a.removeDuplicates();
	HTEST_ASSERT(a.size() == 3, "size()");
	HTEST_ASSERT(a[0] == 5, "");
	HTEST_ASSERT(a[1] == 1, "");
	HTEST_ASSERT(a[2] == 2, "");
	b.removedDuplicates();
	HTEST_ASSERT(a != b, "removedDuplicates()");
	b.clear();
	b += 5;
	b += 1;
	b += 2;
	HTEST_ASSERT(a == b, "clear()");
#ifndef __APPLE__ // stable_sort fails to compile on Mac for some reason, disabling this test for now
	a.sort();
	HTEST_ASSERT(a == b.sorted(), "sorted()");
	HTEST_ASSERT(a != b, "");
	b.sort();
	HTEST_ASSERT(a == b, "sort()");
#endif
	HTEST_ASSERT(a.min() == 1, "");
	HTEST_ASSERT(b.max() == 5, "");
}

HTEST_CASE(iteration)
{
	hlist<int> a;
	a += 5;
	a += 6;
	a += 2;
	a += 0;
	int i = 0;
	foreach_l(int, it, a)
	{
		HTEST_ASSERT((*it) == a[i], "");
		i++;
	}
	i = 0;
	foreach_lr(int, it, a)
	{
		HTEST_ASSERT((*it) == a[a.size() - 1 - i], "");
		i++;
	}
}

HTEST_CASE(constructor)
{
	hlist<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	hlist<int> b(a);
	HTEST_ASSERT(a == b, "");
	hlist<int> c = a;
	HTEST_ASSERT(a == c, "");
}

HTEST_CASE(intersection)
{
	hlist<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	hlist<int> b;
	b += 2;
	b += 3;
	b += 4;
	b += 5;
	hlist<int> c = a.intersected(b);
	HTEST_ASSERT(c.size() == 2, "");
	HTEST_ASSERT(c[0] == 2, "");
	HTEST_ASSERT(c[1] == 3, "");
	HTEST_ASSERT(c == (a & b), "");
	a.intersect(b);
	HTEST_ASSERT(a == c, "");
}

HTEST_CASE(union_)
{
	hlist<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	hlist<int> b;
	b += 2;
	b += 3;
	b += 4;
	b += 5;
	hlist<int> c = a.united(b);
	HTEST_ASSERT(c.size() == 6, "");
	HTEST_ASSERT(c[0] == 0, "");
	HTEST_ASSERT(c[1] == 1, "");
	HTEST_ASSERT(c[2] == 2, "");
	HTEST_ASSERT(c[3] == 3, "");
	HTEST_ASSERT(c[4] == 4, "");
	HTEST_ASSERT(c[5] == 5, "");
	HTEST_ASSERT(c == (a | b), "");
	a.unite(b);
	HTEST_ASSERT(a == c, "");
	a.unite(4);
	HTEST_ASSERT(a == c, "");
	a.unite(999);
	HTEST_ASSERT(a != c, "");
}

HTEST_CASE(difference)
{
	hlist<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 2;
	a += 3;
	a += 3;
	a += 4;
	hlist<int> b;
	b += 2;
	b += 2;
	b += 3;
	b += 4;
	b += 4;
	b += 5;
	hlist<int> c = a.differentiated(b);
	HTEST_ASSERT(c.size() == 2, "");
	HTEST_ASSERT(c[0] == 0, "");
	HTEST_ASSERT(c[1] == 1, "");
	HTEST_ASSERT(c == (a / b), "");
	a.differentiate(b);
	HTEST_ASSERT(a == c, "");
	hlist<hstr> a1;
	a1 += "a";
	a1 += "a";
	hlist<hstr> a2;
	a2 += "a";
	a2 += "a";
	HTEST_ASSERT(a1 == a2, "");
	HTEST_ASSERT((a1 / a2).size() == 0, "");
}

HTEST_CASE(join)
{
	hlist<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	hstr b = a.joined(",");
	HTEST_ASSERT(b == "0,1,2,3", "");
	b = a.joined(2);
	HTEST_ASSERT(b == "0212223", "");
	b = a.joined(1.5f);
	HTEST_ASSERT(b == "01.511.521.53", "");
}
static bool negative(int const& i) { return (i < 0); }
static bool positive(int const& i) { return (i >= 0); }
static bool over_9000(int const& i) { return (i > 9000); }

HTEST_CASE(matching)
{
	hlist<int> a;
	a += 0;
	a += -1;
	a += 2;
	a += -3;
	HTEST_ASSERT(a.matchesAny(&positive) == true, "");
	HTEST_ASSERT(a.matchesAny(&negative) == true, "");
	HTEST_ASSERT(a.matchesAll(&negative) == false, "");
	HTEST_ASSERT(a.matchesAll(&positive) == false, "");
	HTEST_ASSERT(a.findFirst(&negative) != NULL, "");
	HTEST_ASSERT(*a.findFirst(&negative) == -1, "");
	HTEST_ASSERT(a.findFirst(&positive) != NULL, "");
	HTEST_ASSERT(*a.findFirst(&positive) == 0, "");
	HTEST_ASSERT(a.findFirst(&over_9000) == NULL, "");
	hlist<int> c = a.findAll(&negative);
	HTEST_ASSERT(c.size() == 2 && c[0] == -1 && c[1] == -3, "");
	HTEST_ASSERT(c.matchesAny(&negative) == true, "");
	HTEST_ASSERT(c.matchesAny(&positive) == false, "");
	HTEST_ASSERT(c.matchesAll(&negative) == true, "");
	HTEST_ASSERT(c.matchesAll(&positive) == false, "");
}

//todo - fix the random function for lists in hltypes
HTEST_CASE(random)
{
	hlist<int> a;
	a += 0;
	a += 1;
	a += 2;
	a += 3;
	int i = a.random();
	HTEST_ASSERT(a.has(i), "");
	hlist<int> b = a.random(2);
	HTEST_ASSERT(b.size() == 2, "");
	HTEST_ASSERT(a.has(b), "");
	HTEST_ASSERT(!b.has(a), "");
}

HTEST_CASE(cast)
{
	hlist<int> a;
	a += 0;
	a += -1;
	a += 2;
	a += -3;
	hlist<hstr> b = a.cast<hstr>();
	HTEST_ASSERT(b.size() == 4, "");
	HTEST_ASSERT(b[0] == "0", "");
	HTEST_ASSERT(b[1] == "-1", "");
	HTEST_ASSERT(b[2] == "2", "");
	HTEST_ASSERT(b[3] == "-3", "");
}

HTEST_CASE(incorrectNegativeIndex)
{
	hlist<int> test;
	int value;

	try
	{
		value = test[-1];
	}
	catch (hexception)
	{
		HTEST_ASSERT(true, "");
		return;
	}
	catch (std::exception& e)
	{
		printf("ERROR: hlist indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
		HTEST_ASSERT(false, "");
		return;
	}

	printf("ERROR: negative out of bounds hlist indexing test didn't throw any exceptions!\n");
	HTEST_ASSERT(false, "");
}

HTEST_CASE(correctNegativeIndex)
{
	hlist<int> test;
	test += 1;
	test += 2;
	test += 3;
	int value;

	try
	{
		value = test[-1];
	}
	catch (hexception& e)
	{
		printf("ERROR: hltypes exception throw where it should've returned last value: %s\n", e.getMessage().cStr());
		HTEST_ASSERT(false, "");
		return;
	}
	catch (std::exception& e)
	{
		printf("ERROR: hlist indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
		HTEST_ASSERT(false, "");
		return;
	}

	HTEST_ASSERT(value == 3, "");
}

HTEST_CASE(positiveIndex)
{
	hlist<int> test;
	int value;
	test += 1;
	test += 2;
	test += 3;

	try
	{
		for (int i = 0; i < 4; i++)
		{
			value = test[i];
		}
	}
	catch (hexception)
	{
		HTEST_ASSERT(true, "");
		return;
	}
	catch (std::exception& e)
	{
		printf("ERROR: hlist indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
		HTEST_ASSERT(false, "");
		return;
	}

	printf("ERROR: positive array out of bounds hlist indexing test didn't throw any exceptions!\n");
	HTEST_ASSERT(false, "");
}

HTEST_CASE(outOfBoundsAssignment)
{
	hlist<int> test;
	test += 1;
	test += 2;
	test += 3;

	try
	{
		test[4] = 5;
	}
	catch (hexception)
	{
		HTEST_ASSERT(true, "");
		return;
	}
	catch (std::exception& e)
	{
		printf("ERROR: hlist indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
		HTEST_ASSERT(false, "");
		return;
	}

	printf("ERROR: array out of bounds hlist asignment test didn't throw any exceptions!\n");
	HTEST_ASSERT(false, "");
}

HTEST_SUITE_END
