#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "hthread.h"
#include "hmutex.h"

HL_UT_TEST_CLASS(Thread)
{
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
	
	HL_UT_TEST_FUNCTION(testThreadingBasic)
	{
		output = "";
		hthread t1(&f3);
		hthread t2(&f4);
		t1.start();
		t2.start();
		hthread::sleep(200);
		HL_UT_ASSERT(output.count('1') == 10, "basic threading 1");
		HL_UT_ASSERT(output.count('2') == 10, "basic threading 2");
	}

	HL_UT_TEST_FUNCTION(testThreadingControl)
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
		HL_UT_ASSERT(output.count('1') == 10, "threading control 1");
		HL_UT_ASSERT(output.count('2') == 10, "threading control 2");
	}

}
