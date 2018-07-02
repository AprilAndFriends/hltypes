#define _HTEST_LIB hltypes
#define _HTEST_CLASS Thread
#include <htest/htest.h>
#include "hthread.h"
#include "hmutex.h"


static hstr output;
static hmutex mutex;

//	static void f1(hthread* t)
//	{
//		output += "1 ";
//	}
//
//	static void f2(hthread* t)
//	{
//		output += "2 ";
//	}

static void f3(hthread* t)
{
	hmutex::ScopeLock lock;
	for (int i = 0; i < 10; i++)
	{
		lock.acquire(&mutex);
		output += "1 ";
		lock.release();
		hthread::sleep(10);
	}
}

static void f4(hthread* t)
{
	hmutex::ScopeLock lock;
	for (int i = 0; i < 10; i++)
	{
		lock.acquire(&mutex);
		output += "2 ";
		lock.release();
		hthread::sleep(10);
	}
}

HTEST_SUITE_BEGIN

HTEST_CASE(testThreadingBasic)
{
	output = "";
	hthread t1(&f3);
	hthread t2(&f4);
	t1.start();
	t2.start();
	hthread::sleep(200);
	HTEST_ASSERT(output.count('1') == 10, "basic threading 1");
	HTEST_ASSERT(output.count('2') == 10, "basic threading 2");
}

HTEST_CASE(testThreadingControl)
{
	output = "";
	hthread t1(&f3);
	hthread t2(&f4);
	t1.start();
	t2.start();
	hthread::sleep(20);
	t1.pause();
	hthread::sleep(20);
	t2.pause();
	hthread::sleep(100);
	t1.resume();
	hthread::sleep(20);
	t2.resume();
	t1.pause();
	hthread::sleep(30);
	t1.resume();
	hthread::sleep(200);
	t1.stop();
	t2.stop();
	HTEST_ASSERT(output.count('1') == 10, "threading control 1");
	HTEST_ASSERT(output.count('2') == 10, "threading control 2");
}

HTEST_SUITE_END
