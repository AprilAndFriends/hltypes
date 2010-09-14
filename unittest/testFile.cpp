/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <hltypes/exception.h>
#include <hltypes/hfile.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

/******* FILE ******************************************************************************/
TEST(File_ReadWrite)
{
	hstr filename = "test.txt";
	hfile f(filename, hltypes::WRITE);
	f.write("This is a test.");
	f.open(filename, hltypes::READ);
	hstr text = f.read();
	CHECK(text == "This is a test.");
	f.open(filename, hltypes::READ);
	text = f.read(6);
	CHECK(text == "This i");
	text = hfile::hread(filename, 69);
	CHECK(text == "This is a test.");
	hfile::hwrite(filename, "22");
	text = hfile::hread(filename);
	CHECK(text == "This is a test.22");
}

TEST(File_Writef)
{
	hstr filename = "test.txt";
	hfile f(filename, hltypes::WRITE);
	f.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
	f.open(filename, hltypes::READ);
	hstr text = f.read();
	CHECK(text == "This is a 0 formatted 3.14 file.");
}

TEST(File_SeekPositionSize)
{
	hstr filename = "test.txt";
	hfile f;
	f.open(filename, hltypes::WRITE);
	f.write("This is another test.");
	f.open(filename, hltypes::READ);
	f.seek(4, hltypes::START);
	hstr text = f.read();
	CHECK(text == " is another test.");
	f.open(filename, hltypes::READ);
	f.seek(-4, hltypes::END);
	int pos = f.position();
	CHECK(pos == 17);
	text = f.read();
	CHECK(text == "est.");
	f.open(filename, hltypes::READ);
	f.seek(4, hltypes::CURRENT);
	pos = f.position();
	CHECK(pos == 4);
	f.seek(5, hltypes::CURRENT);
	pos = f.position();
	CHECK(pos == 9);
	text = f.read(7);
	CHECK(f.size() == 21);
	CHECK(text == "nother ");
	CHECK(hfile::hsize(filename) == 21);
}

TEST(File_Serialization)
{
	hstr filename = "test.txt";
	hfile f(filename, hltypes::WRITE);
	f.dump(1234);
	f.dump(hstr("testing"));
	f.dump(3.14f);
	f.dump(false);
	f.close();
	f.open(filename);
	int i = f.load_int();
	hstr str = f.load_hstr();
	float e = f.load_float();
	bool b = f.load_bool();
	f.close();
	CHECK(i == 1234);
	CHECK(str == "testing");
	CHECK(hsprintf("%4.2f", e) == "3.14");
	CHECK(!b);
}

/*
TEST(File_Static)
{
	hstr filename = "test2.txt";
	try
	{
		if (hfile::exists(filename))
		{
			hfile::remove(filename);
		}
	}
	catch (const char* a) { }
	CHECK(!hfile::exists(filename));
	hfile::create(filename);
	CHECK(hfile::exists(filename));
	hfile::remove(filename);
	CHECK(!hfile::exists(filename));
}
*/
