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

/******* FILE ******************************************************************************/
TEST(File_read_write)
{
	hstr filename = "test.txt";
	hfile f(filename, hltypes::WRITE);
	f.write("This is a test.");
	f.open(filename, hltypes::READ);
	hstr text = f.read();
	CHECK(text == "This is a test.");
	f.open(filename);
	text = f.read(6);
	CHECK(text == "This i");
	text = f.read(5);
	CHECK(text == "s a t");
	text = hfile::hread(filename, 69);
	CHECK(text == "This is a test.");
	hfile::happend(filename, "22");
	text = hfile::hread(filename);
	CHECK(text == "This is a test.22");
}

TEST(File_writef)
{
	hstr filename = "test.txt";
	hfile f(filename, hltypes::WRITE);
	f.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
	f.open(filename, hltypes::READ);
	hstr text = f.read();
	CHECK(text == "This is a 0 formatted 3.14 file.");
}

TEST(File_read_write_raw)
{
	hstr filename = "raw.txt";
	hfile f(filename, hltypes::WRITE);
	unsigned char a[10] = {'\0'};
	a[0] = 'R';
	a[1] = 'a';
	a[2] = 'w';
	a[3] = ' ';
	a[4] = 't';
	a[5] = 'e';
	a[6] = 's';
	a[7] = 't';
	a[8] = '.';
	f.write_raw(a, 5);
	f.close();
	f.open(filename, hltypes::READ);
	unsigned char b[6] = {'\0'};
	f.read_raw(b, 5);
	hstr str = hstr((char*)b);
	CHECK(str == "Raw t");
}

TEST(File_seek_position_size)
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

TEST(File_serialization)
{
	hstr filename = "test.txt";
	hfile f(filename, hltypes::WRITE);
	f.dump(1234);
	f.dump((short)4321);
	f.dump(hstr("testing"));
	f.dump(3.14f);
	f.dump(1.23456789999999);
	f.dump(false);
	f.close();
	f.open(filename);
	int i = f.load_int();
	short s = f.load_short();
	hstr str = f.load_hstr();
	float e = f.load_float();
	double d = f.load_double();
	bool b = f.load_bool();
	f.close();
	CHECK(i == 1234);
	CHECK(s == 4321);
	CHECK(str == "testing");
	CHECK(e == 3.14f);
	CHECK(d == 1.23456789999999);
	CHECK(!b);
}

TEST(File_static_create_remove)
{
	hstr filename = "test2.txt";
	hfile::remove(filename);
	CHECK(!hfile::exists(filename));
	hfile::create(filename);
	CHECK(hfile::exists(filename));
	hfile::remove(filename);
	CHECK(!hfile::exists(filename));
}

TEST(File_static_rename)
{
	hstr old_filename = "test.txt";
	hstr new_filename = "test2.txt";
	hfile::remove(old_filename);
	hfile::create(old_filename);
	hfile::remove(new_filename);
	CHECK(hfile::exists(old_filename));
	CHECK(!hfile::exists(new_filename));
	hfile::rename(old_filename, new_filename);
	CHECK(!hfile::exists(old_filename));
	CHECK(hfile::exists(new_filename));
	hfile::remove(new_filename);
}

TEST(File_static_copy)
{
	hstr old_filename = "test.txt";
	hstr new_filename = "test2.txt";
	hfile::remove(old_filename);
	hfile::remove(new_filename);
	hfile::hwrite(old_filename, "This is a copy test.");
	hfile::copy(old_filename, new_filename);
	CHECK(hfile::exists(new_filename));
	hstr text = hfile::hread(new_filename);
	CHECK(text == "This is a copy test.");
	hfile f(old_filename, hltypes::WRITE);
	f.dump(1234);
	f.dump((short)4321);
	f.dump(hstr("testing"));
	f.dump(3.14f);
	f.dump(1.23456789999999);
	f.dump(false);
	f.close();
	hfile::remove(new_filename);
	hfile::copy(old_filename, new_filename);
	CHECK(hfile::exists(new_filename));
	f.open(new_filename);
	int i = f.load_int();
	short s = f.load_short();
	hstr str = f.load_hstr();
	float e = f.load_float();
	double d = f.load_double();
	bool b = f.load_bool();
	f.close();
	CHECK(i == 1234);
	CHECK(s == 4321);
	CHECK(str == "testing");
	CHECK(e == 3.14f);
	CHECK(d == 1.23456789999999);
	CHECK(!b);
}

