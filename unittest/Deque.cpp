#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "hdeque.h"

HL_UT_TEST_CLASS(Deque)
{
	HL_UT_TEST_FUNCTION(adding)
	{
		hdeque<int> a;
		a.add(1);
		a.add(0);
		a += 7;
		a << 20;
		HL_UT_ASSERT(a[0] == 1, "");
		//HL_UT_ASSERT(a.front() == 0, ""); //TODO CHECK
		HL_UT_ASSERT(a[1] == 0, "");
		HL_UT_ASSERT(a[2] == 7, "");
		HL_UT_ASSERT(a[3] == 20, "");
		//HL_UT_ASSERT(a.back() == 20, ""); 
		HL_UT_ASSERT(a.at(0) == 1, "");
		HL_UT_ASSERT(a.at(1) == 0, "");
		HL_UT_ASSERT(a.at(2) == 7, "");
		HL_UT_ASSERT(a.at(3) == 20, "");
		HL_UT_ASSERT(a.size() == 4, "");
	}
	HL_UT_TEST_FUNCTION(removing)
	{
		hdeque<int> a;
		a.add(1);
		a.add(0);
		a.add(7);
		a.removeFirst();		
		HL_UT_ASSERT(a[0] == 0, "");
		a.removeLast();
		HL_UT_ASSERT(a[0] == 0, "");
		a.removeLast();
		HL_UT_ASSERT(a.size() == 0, "");
	}
	HL_UT_TEST_FUNCTION(size)
	{
		hdeque<int> a;
		a.add(1);
		a.add(0);
		a.add(7);
		a.add(8);
		a.removeFirst();
		HL_UT_ASSERT(a.size() == 3, "");
	}
}