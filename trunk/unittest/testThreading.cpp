/// @file
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <hltypes/hmutex.h>
#include <hltypes/hstring.h>
#include <hltypes/hthread.h>

hstr output;
hmutex mutex;

void f1()
{
	output += "1 ";
}

void f2()
{
	output += "2 ";
}

void f3()
{
	for (int i = 0; i < 10; i++)
	{
		mutex.lock();
		output += "1 ";
		//printf("1 ");
		mutex.unlock();
		hthread::sleep(10);
	}
}

void f4()
{
	for (int i = 0; i < 10; i++)
	{
		mutex.lock();
		output += "2 ";
		//printf("2 ");
		mutex.unlock();
		hthread::sleep(10);
	}
}

/******* UTIL ******************************************************************************/
TEST(Threading_manual)
{
	output = "";
	hthread* t1 = new hthread(&f1);
	hthread* t2 = new hthread(&f2);
	t1->execute();
	t2->execute();
	t1->execute();
	CHECK(output == "1 2 1 ");
	t2->execute();
	CHECK(output == "1 2 1 2 ");
	delete t1;
	delete t2;
}

TEST(Threading_basic)
{
	output = "";
	hthread* t1 = new hthread(&f3);
	hthread* t2 = new hthread(&f4);
	t1->start();
	t2->start();
	hthread::sleep(200);
	CHECK(output.count('1') == 10);
	CHECK(output.count('2') == 10);
	delete t1;
	delete t2;
}

TEST(Threading_control)
{
	output = "";
	hthread* t1 = new hthread(&f3);
	hthread* t2 = new hthread(&f4);
	t1->start();
	t2->start();
	hthread::sleep(20);
	t1->pause();
	hthread::sleep(20);
	t2->pause();
	hthread::sleep(100);
	t1->resume();
	hthread::sleep(20);
	t2->resume();
	t1->pause();
	hthread::sleep(30);
	t1->resume();
	hthread::sleep(200);
	t1->stop();
	t2->stop();
	
	CHECK(output.count('1') == 10);
	CHECK(output.count('2') == 10);
	delete t1;
	delete t2;
}

