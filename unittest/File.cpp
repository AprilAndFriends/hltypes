#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "harray.h"
#include "hexception.h"
#include "hfile.h"
#include "hstring.h"

HL_UT_TEST_CLASS(File)
{
	HL_UT_TEST_FUNCTION(readWrite)
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.write("This is a test.");
		f.open(filename, hfile::READ);
		hstr text = f.read();
		HL_UT_ASSERT(text == "This is a test.", "");
		f.open(filename);
		text = f.read(6);
		HL_UT_ASSERT(text == "This i", "read()");
		text = f.read(5);
		HL_UT_ASSERT(text == "s a t", "read()");
		text = hfile::hread(filename, 69);
		HL_UT_ASSERT(text == "This is a test.", "read");
		hfile::happend(filename, "22");
		text = hfile::hread(filename);
		HL_UT_ASSERT(text == "This is a test.22", "write()");
	}

	HL_UT_TEST_FUNCTION(readLine)
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.writeLine("This is a test.");
		f.writeLine("This is also a test.");
		f.writeLine("This is another test.");
		f.open(filename, hfile::READ);
		hstr text = f.read();
		HL_UT_ASSERT(text == "This is a test.\nThis is also a test.\nThis is another test.\n", "");
		f.open(filename, hfile::READ);
		text = f.readLine();
		HL_UT_ASSERT(text == "This is a test.", "");
		text = f.readLine();
		HL_UT_ASSERT(text == "This is also a test.", "");
		text = f.readLine();
		HL_UT_ASSERT(text == "This is another test.", "");
		f.open(filename, hfile::READ);
		harray<hstr> lines = f.readLines();
		HL_UT_ASSERT(lines[0] == "This is a test." && lines[1] == "This is also a test." && lines[2] == "This is another test.", "");
	}

	HL_UT_TEST_FUNCTION(readDelimiter)
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.write(hstr('a', 4090));
		f.write(hstr('b', 10));
		f.open(filename, hfile::READ);
		hstr text = f.read(hstr('b', 10));
		HL_UT_ASSERT(text == hstr('a', 4090), "");
	}

	HL_UT_TEST_FUNCTION(writef)
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
		f.open(filename, hfile::READ);
		hstr text = f.read();
		HL_UT_ASSERT(text == "This is a 0 formatted 3.14 file.", "");
	}

	HL_UT_TEST_FUNCTION(readWriteRaw)
	{
		hstr filename = "raw.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		unsigned char a[10] = { '\0' };
		a[0] = 'R';
		a[1] = 'a';
		a[2] = 'w';
		a[3] = ' ';
		a[4] = 't';
		a[5] = 'e';
		a[6] = 's';
		a[7] = 't';
		a[8] = '.';
		f.writeRaw(a, 5);
		f.close();
		f.open(filename, hfile::READ);
		unsigned char b[6] = { '\0' };
		f.readRaw(b, 5);
		hstr str = hstr((char*)b);
		HL_UT_ASSERT(str == "Raw t", "");
		f.close();
		hfile::remove(filename);
	}

	HL_UT_TEST_FUNCTION(seekPositionSize)
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.write("This is another test.");
		f.open(filename, hfile::READ);
		f.seek(4, hfile::START);
		hstr text = f.read();
		HL_UT_ASSERT(text == " is another test.", "seek()");
		f.open(filename, hfile::READ);
		f.seek((int64_t)-4, hfile::END);
		int pos = (int)f.position();
		HL_UT_ASSERT(pos == 17, "position()");
		text = f.read();
		HL_UT_ASSERT(text == "est.", "");
		f.open(filename, hfile::READ);
		f.seek((int64_t)4, hfile::CURRENT);
		pos = (int)f.position();
		HL_UT_ASSERT(pos == 4, "");
		f.seek((int64_t)5, hfile::CURRENT);
		pos = (int)f.position();
		HL_UT_ASSERT(pos == 9, "");
		text = f.read(7);
		HL_UT_ASSERT(f.size() == (int64_t)21, "size()");
		HL_UT_ASSERT(text == "nother ", "");		
	}
	HL_UT_TEST_FUNCTION(serialization)
	{
		hstr filename = "test.txt/";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.dump(1234);
		f.dump((short)4321);
		f.dump(hstr("testing"));
		f.dump(3.14f);
		f.dump(1.23456789999999);
		f.dump(false);
		f.close();
		f.open(filename);
		int i = f.loadInt32();
		short s = f.loadInt16();
		hstr str = f.loadString();
		float e = f.loadFloat();
		double d = f.loadDouble();
		bool b = f.loadBool();
		f.close();
		HL_UT_ASSERT(i == 1234, "int32");
		HL_UT_ASSERT(s == 4321, "int16");
		HL_UT_ASSERT(str == "testing", "string");
		HL_UT_ASSERT(e == 3.14f, "float");
		HL_UT_ASSERT(d == 1.23456789999999, "double");
		HL_UT_ASSERT(!b, "bool");
	}
	HL_UT_TEST_FUNCTION(staticClear)
	{
		hstr filename = "test.txt";
		hfile::hwrite(filename, "This is a clearing test.");
		hstr text = hfile::hread(filename);
		HL_UT_ASSERT(text == "This is a clearing test.", "");
		hfile::clear(filename);
		text = hfile::hread(filename);
		HL_UT_ASSERT(text == "", "");
	}

	HL_UT_TEST_FUNCTION(staticRename)
	{
		hstr old_filename = "test.txt";
		hstr new_filename = "test2.txt";
		hfile::create(old_filename);
		hfile::remove(new_filename);
		HL_UT_ASSERT(hfile::exists(old_filename), "");
		HL_UT_ASSERT(!hfile::exists(new_filename), "");
		hfile::rename(old_filename, new_filename);
		HL_UT_ASSERT(!hfile::exists(old_filename), "");
		HL_UT_ASSERT(hfile::exists(new_filename), "");
		hfile::remove(new_filename);
	}

	HL_UT_TEST_FUNCTION(staticMove)
	{
		hstr filename = "test.txt";
		hstr path = "..";
		hfile::create(filename);
		hfile::remove(path + "/" + filename);
		HL_UT_ASSERT(hfile::exists(filename), "");
		HL_UT_ASSERT(!hfile::exists(path + "/" + filename), "");
		hfile::move(filename, path);
		HL_UT_ASSERT(!hfile::exists(filename), "");
		HL_UT_ASSERT(hfile::exists(path + "\\" + filename), "");
		hfile::remove(path + "/" + filename);
	}

	HL_UT_TEST_FUNCTION(staticCopy)
	{
		hstr old_filename = "test.txt";
		hstr new_filename = "test2.txt";
		hfile::remove(old_filename);
		hfile::remove(new_filename);
		hfile::hwrite(old_filename, "This is a copy test.");
		hfile::copy(old_filename, new_filename);
		HL_UT_ASSERT(hfile::exists(new_filename), "");
		hstr text = hfile::hread(new_filename);
		HL_UT_ASSERT(text == "This is a copy test.", "");
		hfile f;
		f.open(old_filename, hfile::WRITE);
		f.dump(1234);
		f.dump((short)4321);
		f.dump(hstr("testing"));
		f.dump(3.14f);
		f.dump(1.23456789999999);
		f.dump(false);
		f.close();
		hfile::remove(new_filename);
		hfile::copy(old_filename, new_filename);
		HL_UT_ASSERT(hfile::exists(new_filename), "");
		f.open(new_filename);
		int i = f.loadInt32();
		short s = f.loadInt16();
		hstr str = f.loadString();
		float e = f.loadFloat();
		double d = f.loadDouble();
		bool b = f.loadBool();
		f.close();
		hfile::remove(old_filename);
		hfile::remove(new_filename);
		HL_UT_ASSERT(i == 1234, "");
		HL_UT_ASSERT(s == 4321, "");
		HL_UT_ASSERT(str == "testing", "");
		HL_UT_ASSERT(e == 3.14f, "");
		HL_UT_ASSERT(d == 1.23456789999999, "");
		HL_UT_ASSERT(!b, "");
	}
}
