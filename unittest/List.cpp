#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "hlist.h"

HL_UT_TEST_CLASS(List)
{
	HL_UT_TEST_FUNCTION(adding)
	{
		hlist<int> a;
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
		hlist<int> a;
		a += 0;
		a += 1;
		hlist<int> b;
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
		hlist<int> b;
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
		hlist<int> c = a.removeFirst(2);
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
		hlist<int> a;
		hlist<int> indexes;
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
		hlist<int> a;
		a += 0;
		a += 1;
		a += 2;
		hlist<int> b;
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
		hlist<int> a;
		a += 5;
		a += 6;
		a += 2;
		a += 0;
		int i = 0;
		foreach_l(int, it, a)
		{
			HL_UT_ASSERT((*it) == a[i], "");
			i++;
		}
		i = 0;
		foreach_lr(int, it, a)
		{
			HL_UT_ASSERT((*it) == a[a.size() - 1 - i], "");
			i++;
		}
	}
}