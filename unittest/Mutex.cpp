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
	static int test_a = 1;
	static int test_b = 2;
	static int test_c = 5;
	static int test_result = 0;

	hmutex mutex;

	static void thread1(hthread* t)
	{
		mutex.lock();
		test_c = test_a + test_b;
		mutex.unlock();
	}
	static void thread2(hthread* t)
	{
		mutex.lock();
		test_result = test_c + 2;
		mutex.unlock();
	}
	static void thread3(hthread* t)
	{
		mutex.lock();
		test_result = test_result + 1;
		mutex.unlock();
	}	
	HL_UT_TEST_FUNCTION(lockrelease)
	{		
		hthread t1(&thread1);
		hthread t2(&thread2);

		t1.start();
		t2.start();

		hthread::sleep(100);

		t1.join();
		t2.join();

		HL_UT_ASSERT(test_result == 5, "");
	}
	HL_UT_TEST_FUNCTION(mass_lockrelease)
	{
		test_result = 0;

		hthread* t[25];
		for (int i = 0; i < 25; i++)
		{
			t[i] = new hthread(&thread3);
			t[i]->start();
		}

		for (int i = 0; i < 25; i++)
		{
			t[i]->join();
			delete t[i];
		}

		HL_UT_ASSERT(test_result == 25, "");
	}
}
