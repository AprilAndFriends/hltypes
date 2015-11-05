#include "generated/aprilUT.h"
#include "hthread.h"
#include "hmutex.h"

namespace testThread
{
	hstr output;
	hmutex mutex;

	void f1(hthread* t)
	{
		output += "1 ";
	}

	void f2(hthread* t)
	{
		output += "2 ";
	}

	void f3(hthread* t)
	{
		for (int i = 0; i < 10; i++)
		{
			mutex.lock();
			output += "1 ";
			mutex.unlock();
			hthread::sleep(10);
		}
	}

	void f4(hthread* t)
	{
		for (int i = 0; i < 10; i++)
		{
			mutex.lock();
			output += "2 ";
			mutex.unlock();
			hthread::sleep(10);
		}
	}

	void testThreadingManual()
	{
		output = "";
		hthread t1(&f1);
		hthread t2(&f2);
		t1.execute();
		t2.execute();
		t1.execute();
		assertTrue(output == "1 2 1 ", "hthread - manual threading");
		t2.execute();
		assertTrue(output == "1 2 1 2 ", "hthread - manual threading");
	}

	void testThreadingBasic()
	{
		output = "";
		hthread t1(&f3);
		hthread t2(&f4);
		t1.start();
		t2.start();
		hthread::sleep(200);
		assertTrue(output.count('1') == 10, "hthread - basic threading");
		assertTrue(output.count('2') == 10, "hthread - basic threading");
	}

	void testThreadingControl()
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
		assertTrue(output.count('1') == 10, "hthread - threading control");
		assertTrue(output.count('2') == 10, "hthread - threading control");
	}
}
