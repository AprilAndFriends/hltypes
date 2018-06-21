#define __HTEST_LIB_NAME hltypes
#include "htest.h"
#include "hdeque.h"

HTEST_CLASS(Deque)
{
	HTEST_FUNCTION(adding)
	{
		hdeque<int> a;
		a.add(1);
		a.addFirst(0);
		a += 7;
		a << 20;
		HTEST_ASSERT(a[0] == 0, "");
		HTEST_ASSERT(a.first() == 0, "");
		HTEST_ASSERT(a[1] == 1, "");
		HTEST_ASSERT(a[2] == 7, "");
		HTEST_ASSERT(a[3] == 20, "");
		HTEST_ASSERT(a.last() == 20, ""); 
		HTEST_ASSERT(a.at(0) == 0, "");
		HTEST_ASSERT(a.at(1) == 1, "");
		HTEST_ASSERT(a.at(2) == 7, "");
		HTEST_ASSERT(a.at(3) == 20, "");
		HTEST_ASSERT(a.size() == 4, "");
	}
	HTEST_FUNCTION(removing)
	{
		hdeque<int> a;
		a.add(1);
		a.add(0);
		a.add(7);
		a.removeFirst();		
		HTEST_ASSERT(a[0] == 0, "");
		a.removeLast();
		HTEST_ASSERT(a[0] == 0, "");
		a.removeLast();
		HTEST_ASSERT(a.size() == 0, "");
	}
	HTEST_FUNCTION(size)
	{
		hdeque<int> a;
		a.add(1);
		a.add(0);
		a.add(7);
		a.add(8);
		a.removeFirst();
		HTEST_ASSERT(a.size() == 3, "");
	}
}
