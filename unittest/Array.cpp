#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "harray.h"

HL_UT_TEST_CLASS(Array)
{
	HL_UT_TEST_FUNCTION(adding)
	{
		harray<int> a;
		a.add(1);
		a.addFirst(0);
		a.insertAt(1, 2);
		a.add(5);
		a.add(15);
		a += 7;
		a << 20;
		HL_UT_ASSERT(a[0] == 0, "");
		HL_UT_ASSERT(a.first() == 0, "");
		HL_UT_ASSERT(a[1] == 2, "");
		HL_UT_ASSERT(a[2] == 1, "");
		HL_UT_ASSERT(a[3] == 5, "");
		HL_UT_ASSERT(a[4] == 15, "");
		HL_UT_ASSERT(a[5] == 7, "");
		HL_UT_ASSERT(a[6] == 20, "");
		HL_UT_ASSERT(a.last() == 20, "");
		HL_UT_ASSERT(a.at(0) == 0, "");
		HL_UT_ASSERT(a.at(1) == 2, "");
		HL_UT_ASSERT(a.at(2) == 1, "");
		HL_UT_ASSERT(a.at(3) == 5, "");
		HL_UT_ASSERT(a.at(4) == 15, "");
		HL_UT_ASSERT(a.at(5) == 7, "");
		HL_UT_ASSERT(a.at(6) == 20, "");
		HL_UT_ASSERT(a.size() == 7, "");
	}

	HL_UT_TEST_FUNCTION(adding2)
	{
		harray<int> a;
		a += 0;
		a += 1;
		harray<int> b;
		b.insertAt(0, a);
		HL_UT_ASSERT(b.size() == 2, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[1], "");
		b.add(a, 1, 1);
		HL_UT_ASSERT(b.size() == 3, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[1], "");
		HL_UT_ASSERT(b[2] == a[1], "");
		b.clear();
		b.add(a);
		HL_UT_ASSERT(b.size() == 2, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[1], "");
		b.insertAt(1, a);
		HL_UT_ASSERT(b.size() == 4, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[0], "");
		HL_UT_ASSERT(b[2] == a[1], "");
		HL_UT_ASSERT(b[3] == a[1], "");
		b.addFirst(a, 1);
		HL_UT_ASSERT(b.size() == 5, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[0], "");
		HL_UT_ASSERT(b[2] == a[0], "");
		HL_UT_ASSERT(b[3] == a[1], "");
		HL_UT_ASSERT(b[4] == a[1], "");
		b.clear();
		b += a;
		HL_UT_ASSERT(b.size() == 2, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[1], "");
		b << a;
		HL_UT_ASSERT(b.size() == 4, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[1], "");
		HL_UT_ASSERT(b[2] == a[0], "");
		HL_UT_ASSERT(b[3] == a[1], "");
	}

	HL_UT_TEST_FUNCTION(adding3)
	{
		int a[2] = { 0, 1 };
		harray<int> b;
		b.insertAt(0, a, 2);
		HL_UT_ASSERT(b.size() == 2, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[1], "");
		b.add(a, 1);
		HL_UT_ASSERT(b.size() == 3, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[1], "");
		HL_UT_ASSERT(b[2] == a[0], "");
		b.clear();
		b.add(a, 2);
		HL_UT_ASSERT(b.size() == 2, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[1], "");
		b.insertAt(1, a, 2);
		HL_UT_ASSERT(b.size() == 4, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[0], "");
		HL_UT_ASSERT(b[2] == a[1], "");
		HL_UT_ASSERT(b[3] == a[1], "");
		b.addFirst(a, 1);
		HL_UT_ASSERT(b.size() == 5, "");
		HL_UT_ASSERT(b[0] == a[0], "");
		HL_UT_ASSERT(b[1] == a[0], "");
		HL_UT_ASSERT(b[2] == a[0], "");
		HL_UT_ASSERT(b[3] == a[1], "");
		HL_UT_ASSERT(b[4] == a[1], "");
	}

	HL_UT_TEST_FUNCTION(removing1)
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
		int b = a.removeAt(2);
		HL_UT_ASSERT(b == 2, "");
		b = a.removeAt(1);
		HL_UT_ASSERT(b == 1, "");
		b = a.removeFirst();
		HL_UT_ASSERT(b == 0, "");
		b = a.removeLast();
		HL_UT_ASSERT(b == 10, "");
		HL_UT_ASSERT(a.size() == 7, "");
		a -= 4;
		HL_UT_ASSERT(a.size() == 6, "");
		harray<int> c = a.removeFirst(2);
		HL_UT_ASSERT(a.size() == 4, "");
		HL_UT_ASSERT(c.size() == 2, "");
		HL_UT_ASSERT(c[0] == 3, "");
		HL_UT_ASSERT(c[1] == 5, "");
		c = a.removeLast(3);
		HL_UT_ASSERT(a.size() == 1, "");
		HL_UT_ASSERT(c.size() == 3, "");
		HL_UT_ASSERT(c[0] == 7, "");
		HL_UT_ASSERT(c[1] == 8, "");
		HL_UT_ASSERT(c[2] == 9, "");
		c.removeAt(1, 2);
		HL_UT_ASSERT(c.size() == 1, "");
		HL_UT_ASSERT(c[0] == 7, "");
	}

	HL_UT_TEST_FUNCTION(removing2)
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
		HL_UT_ASSERT(a.size() == 4, "");
		HL_UT_ASSERT(a[0] == 0, "");
		HL_UT_ASSERT(a[1] == 1, "");
		HL_UT_ASSERT(a[2] == 2, "");
		HL_UT_ASSERT(a[3] == 6, "");
		b.remove(c);
		HL_UT_ASSERT(a == b, "");
	}

	HL_UT_TEST_FUNCTION(container)
	{
		harray<int> a;
		harray<int> indexes;
		a.add(0);
		a.add(1);
		a.add(2);
		a.add(2);
		a.add(3);
		HL_UT_ASSERT(a.indexOf(5) == -1, "");
		HL_UT_ASSERT(a.indexOf(3) == 4, "");
		HL_UT_ASSERT(a.indexOf(2) == 2, "");
		indexes = a.indicesOf(2);
		HL_UT_ASSERT(indexes.size() == 2, "");
		HL_UT_ASSERT(indexes[0] == 2, "");
		HL_UT_ASSERT(indexes[1] == 3, "");
		indexes = a.indicesOf(3);
		HL_UT_ASSERT(indexes.size() == 1, "");
		HL_UT_ASSERT(indexes[0] == 4, "");
		indexes = a.indicesOf(9);
		HL_UT_ASSERT(indexes.size() == 0, "");
		HL_UT_ASSERT(a.has(2) == true, "");
		HL_UT_ASSERT(a.has(8) == false, "");
		HL_UT_ASSERT(a.count(0) == 1, "");
		HL_UT_ASSERT(a.count(2) == 2, "");
		HL_UT_ASSERT(a.count(5) == 0, "");
	}

	HL_UT_TEST_FUNCTION(comparison)
	{
		harray<int> a;
		a += 0;
		a += 1;
		a += 2;
		harray<int> b;
		b += 1;
		b += 2;
		b += 3;
		HL_UT_ASSERT(a.equals(b) == false, "");
		HL_UT_ASSERT((a == b) == a.equals(b), "");
		a.removeAt(0);
		b.removeAt(2);
		HL_UT_ASSERT(a.equals(b) == true, "");
		HL_UT_ASSERT((a == b) == a.equals(b), "");
		a += a.removeAt(0);
		HL_UT_ASSERT(a.equals(b) == false, "");
	}

	HL_UT_TEST_FUNCTION(operations)
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
		HL_UT_ASSERT(a == b.reversed(), "reversed()");
		b.reverse();
		HL_UT_ASSERT(a == b, "reverse()");
		a.removeDuplicates();
		HL_UT_ASSERT(a.size() == 3, "size()");
		HL_UT_ASSERT(a[0] == 5, "");
		HL_UT_ASSERT(a[1] == 1, "");
		HL_UT_ASSERT(a[2] == 2, "");
		b.removedDuplicates();
		HL_UT_ASSERT(a != b, "removedDuplicates()");
		b.clear();
		b += 5;
		b += 1;
		b += 2;
		HL_UT_ASSERT(a == b, "clear()");
		a.sort();
		HL_UT_ASSERT(a == b.sorted(), "sorted()");
		HL_UT_ASSERT(a != b, "");
		b.sort();
		HL_UT_ASSERT(a == b, "sort()");
		HL_UT_ASSERT(a.min() == 1, "");
		HL_UT_ASSERT(b.max() == 5, "");
	}

	HL_UT_TEST_FUNCTION(iteration)
	{
		harray<int> a;
		a += 5;
		a += 6;
		a += 2;
		a += 0;
		int i = 0;
		foreach(int, it, a)
		{
			HL_UT_ASSERT((*it) == a[i], "");
			i++;
		}
		i = 0;
		foreach_r(int, it, a)
		{
			HL_UT_ASSERT((*it) == a[a.size() - 1 - i], "");
			i++;
		}
	}

	HL_UT_TEST_FUNCTION(constructor)
	{
		harray<int> a;
		a += 0;
		a += 1;
		a += 2;
		a += 3;
		harray<int> b(a);
		HL_UT_ASSERT(a == b, "");
		harray<int> c = a;
		HL_UT_ASSERT(a == c, "");
	}

	HL_UT_TEST_FUNCTION(intersection)
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
		HL_UT_ASSERT(c.size() == 2, "");
		HL_UT_ASSERT(c[0] == 2, "");
		HL_UT_ASSERT(c[1] == 3, "");
		HL_UT_ASSERT(c == (a & b), "");
		a.intersect(b);
		HL_UT_ASSERT(a == c, "");
	}

	HL_UT_TEST_FUNCTION(union_)
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
		HL_UT_ASSERT(c.size() == 6, "");
		HL_UT_ASSERT(c[0] == 0, "");
		HL_UT_ASSERT(c[1] == 1, "");
		HL_UT_ASSERT(c[2] == 2, "");
		HL_UT_ASSERT(c[3] == 3, "");
		HL_UT_ASSERT(c[4] == 4, "");
		HL_UT_ASSERT(c[5] == 5, "");
		HL_UT_ASSERT(c == (a | b), "");
		a.unite(b);
		HL_UT_ASSERT(a == c, "");
		a.unite(4);
		HL_UT_ASSERT(a == c, "");
		a.unite(999);
		HL_UT_ASSERT(a != c, "");
	}

	HL_UT_TEST_FUNCTION(difference)
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
		HL_UT_ASSERT(c.size() == 2, "");
		HL_UT_ASSERT(c[0] == 0, "");
		HL_UT_ASSERT(c[1] == 1, "");
		HL_UT_ASSERT(c == (a / b), "");
		a.differentiate(b);
		HL_UT_ASSERT(a == c, "");
		harray<hstr> a1;
		a1 += "a";
		a1 += "a";
		harray<hstr> a2;
		a2 += "a";
		a2 += "a";
		HL_UT_ASSERT(a1 == a2, "");
		HL_UT_ASSERT((a1 / a2).size() == 0, "");
	}

	HL_UT_TEST_FUNCTION(random)
	{
		harray<int> a;
		a += 0;
		a += 1;
		a += 2;
		a += 3;
		int i = a.random();
		HL_UT_ASSERT(a.has(i), "");
		harray<int> b = a.random(2);
		HL_UT_ASSERT(b.size() == 2, "");
		HL_UT_ASSERT(a.has(b), "");
		HL_UT_ASSERT(!b.has(a), "");
	}

	HL_UT_TEST_FUNCTION(join)
	{
		harray<int> a;
		a += 0;
		a += 1;
		a += 2;
		a += 3;
		hstr b = a.joined(",");
		HL_UT_ASSERT(b == "0,1,2,3", "");
		b = a.joined(2);
		HL_UT_ASSERT(b == "0212223", "");
		b = a.joined(1.5f);
		HL_UT_ASSERT(b == "01.50000011.50000021.5000003", "");
	}

	static bool negative(int i) { return (i < 0); }
	static bool positive(int i) { return (i >= 0); }
	static bool over_9000(int i) { return (i > 9000); }

	HL_UT_TEST_FUNCTION(matching)
	{
		harray<int> a;
		a += 0;
		a += -1;
		a += 2;
		a += -3;
		HL_UT_ASSERT(a.matchesAny(&positive) == true, "");
		HL_UT_ASSERT(a.matchesAny(&negative) == true, "");
		HL_UT_ASSERT(a.matchesAll(&negative) == false, "");
		HL_UT_ASSERT(a.matchesAll(&positive) == false, "");
		HL_UT_ASSERT(a.findFirst(&negative) != NULL, "");
		HL_UT_ASSERT(*a.findFirst(&negative) == -1, "");
		HL_UT_ASSERT(a.findFirst(&positive) != NULL, "");
		HL_UT_ASSERT(*a.findFirst(&positive) == 0, "");
		HL_UT_ASSERT(a.findFirst(&over_9000) == NULL, "");
		harray<int> c = a.findAll(&negative);
		HL_UT_ASSERT(c.size() == 2 && c[0] == -1 && c[1] == -3, "");
		HL_UT_ASSERT(c.matchesAny(&negative) == true, "");
		HL_UT_ASSERT(c.matchesAny(&positive) == false, "");
		HL_UT_ASSERT(c.matchesAll(&negative) == true, "");
		HL_UT_ASSERT(c.matchesAll(&positive) == false, "");
	}

	HL_UT_TEST_FUNCTION(cast)
	{
		harray<int> a;
		a += 0;
		a += -1;
		a += 2;
		a += -3;
		harray<hstr> b = a.cast<hstr>();
		HL_UT_ASSERT(b.size() == 4, "");
		HL_UT_ASSERT(b[0] == "0", "");
		HL_UT_ASSERT(b[1] == "-1", "");
		HL_UT_ASSERT(b[2] == "2", "");
		HL_UT_ASSERT(b[3] == "-3", "");
	}

	HL_UT_TEST_FUNCTION(incorrectNegativeIndex)
	{
		harray<int> test;
		int value;

		try
		{
			value = test[-1];
		}
		catch (hexception)
		{
			HL_UT_ASSERT(true, "");
			return;
		}
		catch (std::exception& e)
		{
			printf("ERROR: harray indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
			HL_UT_ASSERT(false, "");
			return;
		}

		printf("ERROR: negative out of bounds harray indexing test didn't throw any exceptions!\n");
		HL_UT_ASSERT(false, "");
	}

	HL_UT_TEST_FUNCTION(correctNegativeIndex)
	{
		harray<int> test;
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
			HL_UT_ASSERT(false, "");
			return;
		}
		catch (std::exception& e)
		{
			printf("ERROR: harray indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
			HL_UT_ASSERT(false, "");
			return;
		}

		HL_UT_ASSERT(value == 3, "");
	}

	HL_UT_TEST_FUNCTION(positiveIndex)
	{
		harray<int> test;
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
			HL_UT_ASSERT(true, "");
			return;
		}
		catch (std::exception& e)
		{
			printf("ERROR: harray indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
			HL_UT_ASSERT(false, "");
			return;
		}

		printf("ERROR: positive array out of bounds harray indexing test didn't throw any exceptions!\n");
		HL_UT_ASSERT(false, "");
	}

	HL_UT_TEST_FUNCTION(outOfBoundsAssignment)
	{
		harray<int> test;
		test += 1;
		test += 2;
		test += 3;

		try
		{
			test[4] = 5;
		}
		catch (hexception)
		{
			HL_UT_ASSERT(true, "");
			return;
		}
		catch (std::exception& e)
		{
			printf("ERROR: harray indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
			HL_UT_ASSERT(false, "");
			return;
		}

		printf("ERROR: array out of bounds harray asignment test didn't throw any exceptions!\n");
		HL_UT_ASSERT(false, "");
	}
}