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

#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hfile.h>
#include <hltypes/hstring.h>

TEST(File_read_write)
{
	hstr filename = "test.txt";
	hfile f(filename, hfile::WRITE);
	f.write("This is a test.");
	f.open(filename, hfile::READ);
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

TEST(File_read_line)
{
	hstr filename = "test.txt";
	hfile f(filename, hfile::WRITE);
	f.write_line("This is a test.");
	f.write_line("This is also a test.");
	f.write_line("This is another test.");
	f.open(filename, hfile::READ);
	hstr text = f.read();
	CHECK(text == "This is a test.\nThis is also a test.\nThis is another test.\n");
	f.open(filename, hfile::READ);
	text = f.read_line();
	CHECK(text == "This is a test.");
	text = f.read_line();
	CHECK(text == "This is also a test.");
	text = f.read_line();
	CHECK(text == "This is another test.");
	f.open(filename, hfile::READ);
	harray<hstr> lines = f.read_lines();
	CHECK(lines[0] == "This is a test." && lines[1] == "This is also a test." && lines[2] == "This is another test.");
}

TEST(File_read_delimiter)
{
	hstr filename = "test.txt";
	hfile f(filename, hfile::WRITE);
	f.write(hstr('a', 4090));
	f.write(hstr('b', 10));
	f.open(filename, hfile::READ);
	hstr text = f.read(hstr('b', 10));
	CHECK(text == hstr('a', 4090));
}

TEST(File_writef)
{
	hstr filename = "test.txt";
	hfile f(filename, hfile::WRITE);
	f.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
	f.open(filename, hfile::READ);
	hstr text = f.read();
	CHECK(text == "This is a 0 formatted 3.14 file.");
}

TEST(File_read_write_raw)
{
	hstr filename = "raw.txt";
	hfile f(filename, hfile::WRITE);
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
	f.open(filename, hfile::READ);
	unsigned char b[6] = {'\0'};
	f.read_raw(b, 5);
	hstr str = hstr((char*)b);
	CHECK(str == "Raw t");
	f.close();
	hfile::remove(filename);
}

TEST(File_seek_position_size)
{
	hstr filename = "test.txt";
	hfile f;
	f.open(filename, hfile::WRITE);
	f.write("This is another test.");
	f.open(filename, hfile::READ);
	f.seek(4, hfile::START);
	hstr text = f.read();
	CHECK(text == " is another test.");
	f.open(filename, hfile::READ);
	f.seek(-4, hfile::END);
	int pos = f.position();
	CHECK(pos == 17);
	text = f.read();
	CHECK(text == "est.");
	f.open(filename, hfile::READ);
	f.seek(4, hfile::CURRENT);
	pos = f.position();
	CHECK(pos == 4);
	f.seek(5, hfile::CURRENT);
	pos = f.position();
	CHECK(pos == 9);
	text = f.read(7);
	CHECK(f.size() == 21);
	CHECK(text == "nother ");
	CHECK(hfile::hsize(filename) == 21);
}

TEST(File_serialization)
{
	hstr filename = "test.txt/";
	hfile f(filename, hfile::WRITE);
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
	hfile::create(filename);
	CHECK(hfile::exists(filename));
	hfile::remove(filename);
	CHECK(!hfile::exists(filename));
}

TEST(File_static_clear)
{
	hstr filename = "test.txt";
	hfile::hwrite(filename, "This is a clearing test.");
	hstr text = hfile::hread(filename);
	CHECK(text == "This is a clearing test.");
	hfile::clear(filename);
	text = hfile::hread(filename);
	CHECK(text == "");
}

TEST(File_static_rename)
{
	hstr old_filename = "test.txt";
	hstr new_filename = "test2.txt";
	hfile::create(old_filename);
	hfile::remove(new_filename);
	CHECK(hfile::exists(old_filename));
	CHECK(!hfile::exists(new_filename));
	hfile::rename(old_filename, new_filename);
	CHECK(!hfile::exists(old_filename));
	CHECK(hfile::exists(new_filename));
	hfile::remove(new_filename);
}

TEST(File_static_move)
{
	hstr filename = "test.txt";
	hstr path = "..";
	hfile::create(filename);
	hfile::remove(path + "/" + filename);
	CHECK(hfile::exists(filename));
	CHECK(!hfile::exists(path + "/" + filename));
	hfile::move(filename, path);
	CHECK(!hfile::exists(filename));
	CHECK(hfile::exists(path + "\\" + filename));
	hfile::remove(path + "/" + filename);
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
	hfile f(old_filename, hfile::WRITE);
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
	hfile::remove(old_filename);
	hfile::remove(new_filename);
	CHECK(i == 1234);
	CHECK(s == 4321);
	CHECK(str == "testing");
	CHECK(e == 3.14f);
	CHECK(d == 1.23456789999999);
	CHECK(!b);
}

