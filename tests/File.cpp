#define _HTEST_LIB hltypes
#define _HTEST_CLASS File
#include <htest/htest.h>
#include "harray.h"
#include "hexception.h"
#include "hfile.h"
#include "hstring.h"

HTEST_SUITE_BEGIN

HTEST_CASE_WITH_DATA(readWrite)
{
	hstr filename = tempDir + "/test.txt";
	hfile f;
	f.open(filename, hfaccess::Write);
	f.write("This is a test.");
	f.open(filename, hfaccess::Read);
	hstr text = f.read();
	HTEST_ASSERT(text == "This is a test.", "");
	f.open(filename);
	text = f.read(6);
	HTEST_ASSERT(text == "This i", "read()");
	text = f.read(5);
	HTEST_ASSERT(text == "s a t", "read()");
	text = hfile::hread(filename, 69);
	HTEST_ASSERT(text == "This is a test.", "read");
	hfile::happend(filename, "22");
	text = hfile::hread(filename);
	HTEST_ASSERT(text == "This is a test.22", "write()");
}

HTEST_CASE_WITH_DATA(readLine)
{
	hstr filename = tempDir + "/test.txt";
	hfile f;
	f.open(filename, hfaccess::Write);
	f.writeLine("This is a test.");
	f.writeLine("This is also a test.");
	f.writeLine("This is another test.");
	f.open(filename, hfaccess::Read);
	hstr text = f.read();
	HTEST_ASSERT(text == "This is a test.\nThis is also a test.\nThis is another test.\n", "");
	f.open(filename, hfaccess::Read);
	text = f.readLine();
	HTEST_ASSERT(text == "This is a test.", "");
	text = f.readLine();
	HTEST_ASSERT(text == "This is also a test.", "");
	text = f.readLine();
	HTEST_ASSERT(text == "This is another test.", "");
	f.open(filename, hfaccess::Read);
	harray<hstr> lines = f.readLines();
	HTEST_ASSERT(lines[0] == "This is a test." && lines[1] == "This is also a test." && lines[2] == "This is another test.", "");
}

HTEST_CASE_WITH_DATA(readDelimiter)
{
	hstr filename = tempDir + "/test.txt";
	hfile f;
	f.open(filename, hfaccess::Write);
	f.write(hstr('a', 4090));
	f.write(hstr('b', 10));
	f.open(filename, hfaccess::Read);
	hstr text = f.read(hstr('b', 10));
	HTEST_ASSERT(text == hstr('a', 4090), "");
}

HTEST_CASE_WITH_DATA(writef)
{
	hstr filename = tempDir + "/test.txt";
	hfile f;
	f.open(filename, hfaccess::Write);
	f.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
	f.open(filename, hfaccess::Read);
	hstr text = f.read();
	HTEST_ASSERT(text == "This is a 0 formatted 3.14 file.", "");
}

HTEST_CASE_WITH_DATA(readWriteRaw)
{
	hstr filename = tempDir + "/raw.txt";
	hfile f;
	f.open(filename, hfaccess::Write);
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
	f.open(filename, hfaccess::Read);
	unsigned char b[6] = { '\0' };
	f.readRaw(b, 5);
	hstr str = hstr((char*)b);
	HTEST_ASSERT(str == "Raw t", "");
	f.close();
	hfile::remove(filename);
}

HTEST_CASE_WITH_DATA(seekPositionSize)
{
	hstr filename = tempDir + "/test.txt";
	hfile f;
	f.open(filename, hfaccess::Write);
	f.write("This is another test.");
	f.open(filename, hfaccess::Read);
	f.seek(4, hseek::Start);
	hstr text = f.read();
	HTEST_ASSERT(text == " is another test.", "seek()");
	f.open(filename, hfaccess::Read);
	f.seek((int64_t)-4, hseek::End);
	int pos = (int)f.position();
	HTEST_ASSERT(pos == 17, "position()");
	text = f.read();
	HTEST_ASSERT(text == "est.", "");
	f.open(filename, hfaccess::Read);
	f.seek((int64_t)4, hseek::Current);
	pos = (int)f.position();
	HTEST_ASSERT(pos == 4, "");
	f.seek((int64_t)5, hseek::Current);
	pos = (int)f.position();
	HTEST_ASSERT(pos == 9, "");
	text = f.read(7);
	HTEST_ASSERT(f.size() == (int64_t)21, "size()");
	HTEST_ASSERT(text == "nother ", "");
}
HTEST_CASE_WITH_DATA(serialization)
{
	hstr filename = tempDir + "/test.txt/";
	hfile f;
	f.open(filename, hfaccess::Write);
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
	HTEST_ASSERT(i == 1234, "int32");
	HTEST_ASSERT(s == 4321, "int16");
	HTEST_ASSERT(str == "testing", "string");
	HTEST_ASSERT(e == 3.14f, "float");
	HTEST_ASSERT(d == 1.23456789999999, "double");
	HTEST_ASSERT(!b, "bool");
}
HTEST_CASE_WITH_DATA(staticClear)
{
	hstr filename = tempDir + "/test.txt";
	hfile::hwrite(filename, "This is a clearing test.");
	hstr text = hfile::hread(filename);
	HTEST_ASSERT(text == "This is a clearing test.", "");
	hfile::clear(filename);
	text = hfile::hread(filename);
	HTEST_ASSERT(text == "", "");
}

HTEST_CASE_WITH_DATA(staticRename)
{
	hstr oldFilename = tempDir + "/test.txt";
	hstr newFilename = tempDir + "/test2.txt";
	hfile::create(oldFilename);
	hfile::remove(newFilename);
	HTEST_ASSERT(hfile::exists(oldFilename), "");
	HTEST_ASSERT(!hfile::exists(newFilename), "");
	hfile::rename(oldFilename, newFilename);
	HTEST_ASSERT(!hfile::exists(oldFilename), "");
	HTEST_ASSERT(hfile::exists(newFilename), "");
	hfile::remove(newFilename);
}

HTEST_CASE_WITH_DATA(staticMove)
{
	hstr filename = "test.txt";
	hstr filepath = tempDir + "/" + filename;
	hstr path = tempDir + "/..";
	hfile::create(filepath);
	hfile::remove(path + "/" + filename);
	HTEST_ASSERT(hfile::exists(filepath), "");
	HTEST_ASSERT(!hfile::exists(path + "/" + filename), "");
	hfile::move(filepath, path);
	HTEST_ASSERT(!hfile::exists(filepath), "");
	HTEST_ASSERT(hfile::exists(path + "\\" + filename), "");
	hfile::remove(path + "/" + filename);
}

HTEST_CASE_WITH_DATA(staticCopy)
{
	hstr oldFilename = tempDir + "/test.txt";
	hstr newFilename = tempDir + "/test2.txt";
	hfile::remove(oldFilename);
	hfile::remove(newFilename);
	hfile::hwrite(oldFilename, "This is a copy test.");
	hfile::copy(oldFilename, newFilename);
	HTEST_ASSERT(hfile::exists(newFilename), "");
	hstr text = hfile::hread(newFilename);
	HTEST_ASSERT(text == "This is a copy test.", "");
	hfile f;
	f.open(oldFilename, hfaccess::Write);
	f.dump(1234);
	f.dump((short)4321);
	f.dump(hstr("testing"));
	f.dump(3.14f);
	f.dump(1.23456789999999);
	f.dump(false);
	f.close();
	hfile::remove(newFilename);
	hfile::copy(oldFilename, newFilename);
	HTEST_ASSERT(hfile::exists(newFilename), "");
	f.open(newFilename);
	int i = f.loadInt32();
	short s = f.loadInt16();
	hstr str = f.loadString();
	float e = f.loadFloat();
	double d = f.loadDouble();
	bool b = f.loadBool();
	f.close();
	hfile::remove(oldFilename);
	hfile::remove(newFilename);
	HTEST_ASSERT(i == 1234, "");
	HTEST_ASSERT(s == 4321, "");
	HTEST_ASSERT(str == "testing", "");
	HTEST_ASSERT(e == 3.14f, "");
	HTEST_ASSERT(d == 1.23456789999999, "");
	HTEST_ASSERT(!b, "");
}

HTEST_SUITE_END
