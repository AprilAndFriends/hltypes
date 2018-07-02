#define _HTEST_LIB hltypes
#define _HTEST_CLASS Mutex
#include "htest.h"

#include "hmutex.h"
#include "hthread.h"

static int test1 = 1;
static int test2 = 2;
static int test3 = 5;
static int testResult1 = 0;
static int testResult2 = 0;

hmutex mutex1;
hmutex mutex2;
hmutex mutex3;

static void _thread1(hthread* t)
{
	hmutex::ScopeLock lock(&mutex1);
	test3 = test1 + test2;
}

static void _thread2(hthread* t)
{
	hmutex::ScopeLock lock(&mutex2);
	testResult1 = test3 + 2;
}

static void _thread3(hthread* t)
{
	hmutex::ScopeLock lock(&mutex3);
	testResult2 = testResult2 + 1;
}

HTEST_SUITE_BEGIN

HTEST_CASE(lockRelease)
{
	testResult1 = 0;
	hthread t1(&_thread1);
	hthread t2(&_thread2);

	t1.start();
	t2.start();
	
	hthread::sleep(500.0f);

	t1.join();
	t2.join();

	HTEST_ASSERT(testResult1 == 5, "lockRelease");
}

HTEST_CASE(massLockRelease)
{
	testResult2 = 0;

	hthread* t[10];
	for (int i = 0; i < 10; ++i)
	{
		t[i] = new hthread(&_thread3);
		t[i]->start();
	}

	hthread::sleep(500.0f);

	for (int i = 0; i < 10; ++i)
	{
		t[i]->join();
		delete t[i];
	}

	HTEST_ASSERT(testResult2 == 10, hstr(testResult2).cStr());
}

HTEST_SUITE_END
