#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "hmutex.h"
#include "hthread.h"

HL_UT_TEST_CLASS(Mutex)
{
	static int test1 = 1;
	static int test2 = 2;
	static int test3 = 5;
	static int testResult1 = 0;
	static int testResult2 = 0;

	hmutex mutex1;
	hmutex mutex2;
	hmutex mutex3;

	static void thread1(hthread* t)
	{
		mutex1.lock();
		test3 = test1 + test2;
		mutex1.unlock();
	}
	static void thread2(hthread* t)
	{
		mutex2.lock();
		testResult1 = test3 + 2;
		mutex2.unlock();
	}
	static void thread3(hthread* t)
	{
		mutex3.lock();
		testResult2 = testResult2 + 1;
		mutex3.unlock();
	}	
	HL_UT_TEST_FUNCTION(lockRelease)
	{		
		testResult1 = 0;
		hthread t1(&thread1);
		hthread t2(&thread2);

		t1.start();
		t2.start();

		hthread::sleep(100);

		t1.join();
		t2.join();

		HL_UT_ASSERT(testResult1 == 5, "");
	}
	HL_UT_TEST_FUNCTION(massLockRelease)
	{
		testResult2 = 0;

		hthread* t[10];
		for (int i = 0; i < 10; i++)
		{
			t[i] = new hthread(&thread3);
			t[i]->start();
		}

		for (int i = 0; i < 10; i++)
		{
			t[i]->join();
			delete t[i];
		}

		HL_UT_ASSERT(testResult2 == 10, "");
	}
}
