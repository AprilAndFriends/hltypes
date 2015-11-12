#include "generated/aprilUT.h"
#include "harray.h"
#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hstring.h"

namespace testArray
{
	void testAdding1()
	{
		harray<int> a;
		a.add(1);
		a.addFirst(0);
		a.insertAt(1, 2);
		a.add(5);
		a.add(15);
		a += 7;
		a << 20;
		assertTrue(a[0] == 0, "harray - adding");
		assertTrue(a.first() == 0, "harray - adding");		
		assertTrue(a[1] == 2, "harray - adding");
		assertTrue(a[2] == 1, "harray - adding");
		assertTrue(a[3] == 5, "harray - adding");
		assertTrue(a[4] == 15, "harray - adding");
		assertTrue(a[5] == 7, "harray - adding");
		assertTrue(a[6] == 20, "harray - adding");
		assertTrue(a.last() == 20, "harray - adding");
		assertTrue(a.at(0) == 0, "harray - adding");
		assertTrue(a.at(1) == 2, "harray - adding");
		assertTrue(a.at(2) == 1, "harray - adding");
		assertTrue(a.at(3) == 5, "harray - adding");
		assertTrue(a.at(4) == 15, "harray - adding");
		assertTrue(a.at(5) == 7, "harray - adding");
		assertTrue(a.at(6) == 20, "harray - adding");
		assertTrue(a.size() == 7, "harray - adding");
	}

	void testAdding2()
	{
		harray<int> a;
		a += 0;
		a += 1;
		harray<int> b;
		b.insertAt(0, a);
		assertTrue(b.size() == 2, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[1], "harray - adding");
		b.add(a, 1, 1);
		assertTrue(b.size() == 3, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[1], "harray - adding");
		assertTrue(b[2] == a[1], "harray - adding");
		b.clear();
		b.add(a);
		assertTrue(b.size() == 2, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[1], "harray - adding");
		b.insertAt(1, a);
		assertTrue(b.size() == 4, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[0], "harray - adding");
		assertTrue(b[2] == a[1], "harray - adding");
		assertTrue(b[3] == a[1], "harray - adding");
		b.addFirst(a, 1);
		assertTrue(b.size() == 5, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[0], "harray - adding");
		assertTrue(b[2] == a[0], "harray - adding");
		assertTrue(b[3] == a[1], "harray - adding");
		assertTrue(b[4] == a[1], "harray - adding");
		b.clear();
		b += a;
		assertTrue(b.size() == 2, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[1], "harray - adding");
		b << a;
		assertTrue(b.size() == 4, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[1], "harray - adding");
		assertTrue(b[2] == a[0], "harray - adding");
		assertTrue(b[3] == a[1], "harray - adding");
	}

	void testAdding3()
	{
		int a[2] = { 0, 1 };
		harray<int> b;
		b.insertAt(0, a, 2);
		assertTrue(b.size() == 2, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[1], "harray - adding");
		b.add(a, 1);
		assertTrue(b.size() == 3, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[1], "harray - adding");
		assertTrue(b[2] == a[0], "harray - adding");
		b.clear();
		b.add(a, 2);
		assertTrue(b.size() == 2, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[1], "harray - adding");
		b.insertAt(1, a, 2);
		assertTrue(b.size() == 4, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[0], "harray - adding");
		assertTrue(b[2] == a[1], "harray - adding");
		assertTrue(b[3] == a[1], "harray - adding");
		b.addFirst(a, 1);
		assertTrue(b.size() == 5, "harray - adding");
		assertTrue(b[0] == a[0], "harray - adding");
		assertTrue(b[1] == a[0], "harray - adding");
		assertTrue(b[2] == a[0], "harray - adding");
		assertTrue(b[3] == a[1], "harray - adding");
		assertTrue(b[4] == a[1], "harray - adding");
	}

	void testRemoving1()
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
		assertTrue(b == 2, "harray - removing");
		b = a.removeAt(1);
		assertTrue(b == 1, "harray - removing");
		b = a.removeFirst();
		assertTrue(b == 0, "harray - removing");
		b = a.removeLast();
		assertTrue(b == 10, "harray - removing");
		assertTrue(a.size() == 7, "harray - removing");
		a -= 4;
		assertTrue(a.size() == 6, "harray - removing");
		harray<int> c = a.removeFirst(2);
		assertTrue(a.size() == 4, "harray - removing");
		assertTrue(c.size() == 2, "harray - removing");
		assertTrue(c[0] == 3, "harray - removing");
		assertTrue(c[1] == 5, "harray - removing");
		c = a.removeLast(3);
		assertTrue(a.size() == 1, "harray - removing");
		assertTrue(c.size() == 3, "harray - removing");
		assertTrue(c[0] == 7, "harray - removing");
		assertTrue(c[1] == 8, "harray - removing");
		assertTrue(c[2] == 9, "harray - removing");
		c.removeAt(1, 2);
		assertTrue(c.size() == 1, "harray - removing");
		assertTrue(c[0] == 7, "harray - removing");
	}

	void testRemoving2()
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
		assertTrue(a.size() == 4, "harray - removing");
		assertTrue(a[0] == 0, "harray - removing");
		assertTrue(a[1] == 1, "harray - removing");
		assertTrue(a[2] == 2, "harray - removing");
		assertTrue(a[3] == 6, "harray - removing");
		b.remove(c);
		assertTrue(a == b, "harray - removing");
	}

	void testContainer()
	{
		harray<int> a;
		harray<int> indexes;
		a.add(0);
		a.add(1);
		a.add(2);
		a.add(2);
		a.add(3);
		assertTrue(a.indexOf(5) == -1, "harray - container");
		assertTrue(a.indexOf(3) == 4, "harray - container");
		assertTrue(a.indexOf(2) == 2, "harray - container");
		indexes = a.indicesOf(2);
		assertTrue(indexes.size() == 2, "harray - container");
		assertTrue(indexes[0] == 2, "harray - container");
		assertTrue(indexes[1] == 3, "harray - container");
		indexes = a.indicesOf(3);
		assertTrue(indexes.size() == 1, "harray - container");
		assertTrue(indexes[0] == 4, "harray - container");
		indexes = a.indicesOf(9);
		assertTrue(indexes.size() == 0, "harray - container");
		assertTrue(a.has(2) == true, "harray - container");
		assertTrue(a.has(8) == false, "harray - container");
		assertTrue(a.count(0) == 1, "harray - container");
		assertTrue(a.count(2) == 2, "harray - container");
		assertTrue(a.count(5) == 0, "harray - container");
	}

	void testComparison()
	{
		harray<int> a;
		a += 0;
		a += 1;
		a += 2;
		harray<int> b;
		b += 1;
		b += 2;
		b += 3;
		assertTrue(a.equals(b) == false, "harray - comparison");
		assertTrue((a == b) == a.equals(b), "harray - comparison");
		a.removeAt(0);
		b.removeAt(2);
		assertTrue(a.equals(b) == true, "harray - comparison");
		assertTrue((a == b) == a.equals(b), "harray - comparison");
		a += a.removeAt(0);
		assertTrue(a.equals(b) == false, "harray - comparison");
	}

	void testOperations()
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
		assertTrue(a == b.reversed(), "harray - operations");
		b.reverse();
		assertTrue(a == b, "harray - operations");
		a.removeDuplicates();
		assertTrue(a.size() == 3, "harray - operations");
		assertTrue(a[0] == 5, "harray - operations");
		assertTrue(a[1] == 1, "harray - operations");
		assertTrue(a[2] == 2, "harray - operations");
		b.removedDuplicates();
		assertTrue(a != b, "harray - operations");
		b.clear();
		b += 5;
		b += 1;
		b += 2;
		assertTrue(a == b, "harray - operations");
		a.sort();
		assertTrue(a == b.sorted(), "harray - operations");
		assertTrue(a != b, "harray - operations");
		b.sort();
		assertTrue(a == b, "harray - operations");
		assertTrue(a.min() == 1, "harray - operations");
		assertTrue(b.max() == 5, "harray - operations");
	}

	void testIterations()
	{
		harray<int> a;
		a += 5;
		a += 6;
		a += 2;
		a += 0;
		int i = 0;
		foreach(int, it, a)
		{
			assertTrue((*it) == a[i], "harray - iterations");
			i++;
		}
		i = 0;
		foreach_r(int, it, a)
		{
			assertTrue((*it) == a[a.size() - 1 - i], "harray - iterations");
			i++;
		}
	}

	void testConstructor()
	{
		harray<int> a;
		a += 0;
		a += 1;
		a += 2;
		a += 3;
		harray<int> b(a);
		assertTrue(a == b, "harray - constructor");
		harray<int> c = a;
		assertTrue(a == c, "harray - constructor");
	}

	void testIntersection()
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
		assertTrue(c.size() == 2, "harray - intersection");
		assertTrue(c[0] == 2, "harray - intersection");
		assertTrue(c[1] == 3, "harray - intersection");
		assertTrue(c == (a & b), "harray - intersection");
		a.intersect(b);
		assertTrue(a == c, "harray - intersection");
	}

	void testUnion()
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
		assertTrue(c.size() == 6, "harray - union");
		assertTrue(c[0] == 0, "harray - union");
		assertTrue(c[1] == 1, "harray - union");
		assertTrue(c[2] == 2, "harray - union");
		assertTrue(c[3] == 3, "harray - union");
		assertTrue(c[4] == 4, "harray - union");
		assertTrue(c[5] == 5, "harray - union");
		assertTrue(c == (a | b), "harray - union");
		a.unite(b);
		assertTrue(a == c, "harray - union");
		a.unite(4);
		assertTrue(a == c, "harray - union");
		a.unite(999);
		assertTrue(a != c, "harray - union");
	}

	void testDifference()
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
		assertTrue(c.size() == 2, "harray - difference");
		assertTrue(c[0] == 0, "harray - difference");
		assertTrue(c[1] == 1, "harray - difference");
		assertTrue(c == (a / b), "harray - difference");
		a.differentiate(b);
		assertTrue(a == c, "harray - difference");
		harray<hstr> a1;
		a1 += "a";
		a1 += "a";
		harray<hstr> a2;
		a2 += "a";
		a2 += "a";
		assertTrue(a1 == a2, "harray - difference");
		assertTrue((a1 / a2).size() == 0, "harray - difference");
	}

	void testRandom()
	{
		harray<int> a;
		a += 0;
		a += 1;
		a += 2;
		a += 3;
		int i = a.random();
		assertTrue(a.has(i), "harray - random");
		harray<int> b = a.random(2);
		assertTrue(b.size() == 2, "harray - random");
		assertTrue(a.has(b), "harray - random");
		assertTrue(!b.has(a), "harray - random");
	}

	void testJoin()
	{
		harray<int> a;
		a += 0;
		a += 1;
		a += 2;
		a += 3;
		hstr b = a.joined(",");
		assertTrue(b == "0,1,2,3", "harray - join");
		b = a.joined(2);
		assertTrue(b == "0212223", "harray - join");
		b = a.joined(1.5f);
		assertTrue(b == "01.50000011.50000021.5000003", "harray - join");
	}

	bool negative(int i) { return (i < 0); }
	bool positive(int i) { return (i >= 0); }
	bool over_9000(int i) { return (i > 9000); }

	void testMatch()
	{
		harray<int> a;
		a += 0;
		a += -1;
		a += 2;
		a += -3;
		assertTrue(a.matchesAny(&positive) == true, "harray - match");
		assertTrue(a.matchesAny(&negative) == true, "harray - match");
		assertTrue(a.matchesAll(&negative) == false, "harray - match");
		assertTrue(a.matchesAll(&positive) == false, "harray - match");
		assertTrue(a.findFirst(&negative) != NULL, "harray - match");
		assertTrue(*a.findFirst(&negative) == -1, "harray - match");
		assertTrue(a.findFirst(&positive) != NULL, "harray - match");
		assertTrue(*a.findFirst(&positive) == 0, "harray - match");
		assertTrue(a.findFirst(&over_9000) == NULL, "harray - match");
		harray<int> c = a.findAll(&negative);
		assertTrue(c.size() == 2 && c[0] == -1 && c[1] == -3, "harray - match");
		assertTrue(c.matchesAny(&negative) == true, "harray - match");
		assertTrue(c.matchesAny(&positive) == false, "harray - match");
		assertTrue(c.matchesAll(&negative) == true, "harray - match");
		assertTrue(c.matchesAll(&positive) == false, "harray - match");
	}

	void testCast()
	{
		harray<int> a;
		a += 0;
		a += -1;
		a += 2;
		a += -3;
		harray<hstr> b = a.cast<hstr>();
		assertTrue(b.size() == 4, "harray - cast");
		assertTrue(b[0] == "0", "harray - cast");
		assertTrue(b[1] == "-1", "harray - cast");
		assertTrue(b[2] == "2", "harray - cast");
		assertTrue(b[3] == "-3", "harray - cast");
	}

	void testIncorrectNegativeIndex()
	{
		harray<int> test;
		int value;

		try
		{
			value = test[-1];
		}
		catch (hexception)
		{
			assertTrue(true, "harray - incorrect negative index exception");
			return;
		}
		catch (std::exception& e)
		{
			printf("ERROR: harray indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
			assertTrue(false, "harray - incorrect negative index exception");
			return;
		}

		printf("ERROR: negative out of bounds harray indexing test didn't throw any exceptions!\n");
		assertTrue(false, "harray - incorrect negative index exception");
	}

	void testCorrectNegativeIndex()
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
			assertTrue(false, "harray - correct negative index exception");
			return;
		}
		catch (std::exception& e)
		{
			printf("ERROR: harray indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
			assertTrue(false, "harray - correct negative index exception");
			return;
		}

		assertTrue(value == 3, "harray - correct negative index exception");
	}

	void testPositiveIndex()
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
			assertTrue(true, "harray - positive index exception");
			return;
		}
		catch (std::exception& e)
		{
			printf("ERROR: harray indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
			assertTrue(false, "harray - positive index exception");
			return;
		}

		printf("ERROR: positive array out of bounds harray indexing test didn't throw any exceptions!\n");
		assertTrue(false, "harray - positive index exception");
	}

	void testOutOfBoundsAssignment()
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
			assertTrue(true, "harray - out of bounds assignment exception");
			return;
		}
		catch (std::exception& e)
		{
			printf("ERROR: harray indexing resulted in STL exception instead of hltypes exception! msg: %s\n", e.what());
			assertTrue(false, "harray - out of bounds assignment exception");
			return;
		}

		printf("ERROR: array out of bounds harray asignment test didn't throw any exceptions!\n");
		assertTrue(false, "harray - out of bounds assignment exception");
	}
}
