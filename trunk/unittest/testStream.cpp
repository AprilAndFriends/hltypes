/// @file
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <hltypes/harray.h>
#include <hltypes/hexception.h>
#include <hltypes/hstream.h>
#include <hltypes/hstring.h>

TEST(Stream_read_write)
{
	hstream s;
	s.write("This is a test.");
	s.rewind();
	hstr text = s.read();
	CHECK(text == "This is a test.");
	s.rewind();
	text = s.read(6);
	CHECK(text == "This i");
	text = s.read(5);
	CHECK(text == "s a t");
}

TEST(Stream_read_line)
{
	hstream s;
	s.writeLine("This is a test.");
	s.writeLine("This is also a test.");
	s.writeLine("This is another test.");
	s.rewind();
	hstr text = s.read();
	CHECK(text == "This is a test.\nThis is also a test.\nThis is another test.\n");
	s.rewind();
	text = s.readLine();
	CHECK(text == "This is a test.");
	text = s.readLine();
	CHECK(text == "This is also a test.");
	text = s.readLine();
	CHECK(text == "This is another test.");
	s.rewind();
	harray<hstr> lines = s.readLines();
	CHECK(lines[0] == "This is a test." && lines[1] == "This is also a test." && lines[2] == "This is another test.");
}

TEST(Stream_read_delimiter)
{
	hstream s;
	s.write(hstr('a', 4090));
	s.write(hstr('b', 10));
	s.rewind();
	hstr text = s.read(hstr('b', 10));
	CHECK(text == hstr('a', 4090));
}

TEST(Stream_writef)
{
	hstream s;
	s.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
	s.rewind();
	hstr text = s.read();
	CHECK(text == "This is a 0 formatted 3.14 file.");
}

TEST(Stream_read_write_raw)
{
	hstream s;
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
	s.writeRaw(a, 5);
	s.rewind();
	unsigned char b[6] = {'\0'};
	s.readRaw(b, 5);
	hstr str = hstr((char*)b);
	CHECK(str == "Raw t");
}

TEST(Stream_seek_position_size)
{
	hstream s;
	s.write("This is another test.");
	s.rewind();
	s.seek(4, hstream::START);
	hstr text = s.read();
	CHECK(text == " is another test.");
	s.rewind();
	s.seek(-4, hstream::END);
	int pos = s.position();
	CHECK(pos == 17);
	text = s.read();
	CHECK(text == "est.");
	s.rewind();
	s.seek(4, hstream::CURRENT);
	pos = s.position();
	CHECK(pos == 4);
	s.seek(5, hstream::CURRENT);
	pos = s.position();
	CHECK(pos == 9);
	text = s.read(7);
	CHECK(s.size() == 21);
	CHECK(text == "nother ");
	s.clear();
	CHECK(s.size() == 0);
	CHECK(s.position() == 0);
}

TEST(Stream_serialization)
{
	hstream s;
	s.dump(1234);
	s.dump((short)4321);
	s.dump(hstr("testing"));
	s.dump(3.14f);
	s.dump(1.23456789999999);
	s.dump(false);
	s.rewind();
	int i = s.load_int();
	short si = s.load_short();
	hstr str = s.load_string();
	float e = s.load_float();
	double d = s.load_double();
	bool b = s.load_bool();
	CHECK(i == 1234);
	CHECK(si == 4321);
	CHECK(str == "testing");
	CHECK(e == 3.14f);
	CHECK(d == 1.23456789999999);
	CHECK(!b);
}

