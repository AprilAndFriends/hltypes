#include "generated/aprilUT.h"
#include "harray.h"
#include "hexception.h"
#include "hfile.h"
#include "hstring.h"

namespace testFile
{
	void testFileReadWrite()
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.write("This is a test.");
		f.open(filename, hfile::READ);
		hstr text = f.read();
		assertTrue(text == "This is a test.", "file read/write");
		f.open(filename);
		text = f.read(6);
		assertTrue(text == "This i", "file read/write");
		text = f.read(5);
		assertTrue(text == "s a t", "file read/write");
		text = hfile::hread(filename, 69);
		assertTrue(text == "This is a test.", "hfile - read/write");
		hfile::happend(filename, "22");
		text = hfile::hread(filename);
		assertTrue(text == "This is a test.22", "hfile - read/write");
	}

	void testFileReadLine()
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.writeLine("This is a test.");
		f.writeLine("This is also a test.");
		f.writeLine("This is another test.");
		f.open(filename, hfile::READ);
		hstr text = f.read();
		assertTrue(text == "This is a test.\nThis is also a test.\nThis is another test.\n", "hfile - read line");
		f.open(filename, hfile::READ);
		text = f.readLine();
		assertTrue(text == "This is a test.", "hfile - read line");
		text = f.readLine();
		assertTrue(text == "This is also a test.", "hfile - read line");
		text = f.readLine();
		assertTrue(text == "This is another test.", "hfile - read line");
		f.open(filename, hfile::READ);
		harray<hstr> lines = f.readLines();
		assertTrue(lines[0] == "This is a test." && lines[1] == "This is also a test." && lines[2] == "This is another test.", "hfile - read line");
	}

	void testFileReadDelimiter()
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.write(hstr('a', 4090));
		f.write(hstr('b', 10));
		f.open(filename, hfile::READ);
		hstr text = f.read(hstr('b', 10));
		assertTrue(text == hstr('a', 4090), "hfile - read delimiter");
	}

	void testFileWritef()
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
		f.open(filename, hfile::READ);
		hstr text = f.read();
		assertTrue(text == "This is a 0 formatted 3.14 file.", "hfile - test writef");
	}

	void testFileReadWriteRaw()
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
		assertTrue(str == "Raw t", "hfile - test read/write raw");
		f.close();
		hfile::remove(filename);
	}

	void testFileSeekPositionSize()
	{
		hstr filename = "test.txt";
		hfile f;
		f.open(filename, hfile::WRITE);
		f.write("This is another test.");
		f.open(filename, hfile::READ);
		f.seek(4, hfile::START);
		hstr text = f.read();
		assertTrue(text == " is another test.", "hfile - test seek/position/size");
		f.open(filename, hfile::READ);
		f.seek((int64_t)-4, hfile::END);
		int pos = (int)f.position();
		assertTrue(pos == 17, "hfile - test seek/position/size");
		text = f.read();
		assertTrue(text == "est.", "hfile - test seek/position/size");
		f.open(filename, hfile::READ);
		f.seek((int64_t)4, hfile::CURRENT);
		pos = (int)f.position();
		assertTrue(pos == 4, "hfile - test seek/position/size");
		f.seek((int64_t)5, hfile::CURRENT);
		pos = (int)f.position();
		assertTrue(pos == 9, "hfile - test seek/position/size");
		text = f.read(7);
		assertTrue(f.size() == (int64_t)21, "hfile - test seek/position/size");
		assertTrue(text == "nother ", "hfile - test seek/position/size");		
	}
	void testFileSerialization()
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
		assertTrue(i == 1234, "hfile - test serialization");
		assertTrue(s == 4321, "hfile - test serialization");
		assertTrue(str == "testing", "hfile - test serialization");
		assertTrue(e == 3.14f, "hfile - test serialization");
		assertTrue(d == 1.23456789999999, "hfile - test serialization");
		assertTrue(!b, "hfile - test serialization");
	}
	void testFileStaticClear()
	{
		hstr filename = "test.txt";
		hfile::hwrite(filename, "This is a clearing test.");
		hstr text = hfile::hread(filename);
		assertTrue(text == "This is a clearing test.", "hfile - test static clear");
		hfile::clear(filename);
		text = hfile::hread(filename);
		assertTrue(text == "", "hfile - test static clear");
	}

	void testFileStaticRename()
	{
		hstr old_filename = "test.txt";
		hstr new_filename = "test2.txt";
		hfile::create(old_filename);
		hfile::remove(new_filename);
		assertTrue(hfile::exists(old_filename), "hfile - test static rename");
		assertTrue(!hfile::exists(new_filename), "hfile - test static rename");
		hfile::rename(old_filename, new_filename);
		assertTrue(!hfile::exists(old_filename), "hfile - test static rename");
		assertTrue(hfile::exists(new_filename), "hfile - test static rename");
		hfile::remove(new_filename);
	}

	void testFileStaticMove()
	{
		hstr filename = "test.txt";
		hstr path = "..";
		hfile::create(filename);
		hfile::remove(path + "/" + filename);
		assertTrue(hfile::exists(filename), "hfile - test static move");
		assertTrue(!hfile::exists(path + "/" + filename), "hfile - test static move");
		hfile::move(filename, path);
		assertTrue(!hfile::exists(filename), "hfile - test static move");
		assertTrue(hfile::exists(path + "\\" + filename), "hfile - test static move");
		hfile::remove(path + "/" + filename);
	}

	void testFileStaticCopy()
	{
		hstr old_filename = "test.txt";
		hstr new_filename = "test2.txt";
		hfile::remove(old_filename);
		hfile::remove(new_filename);
		hfile::hwrite(old_filename, "This is a copy test.");
		hfile::copy(old_filename, new_filename);
		assertTrue(hfile::exists(new_filename), "hfile - test static copy");
		hstr text = hfile::hread(new_filename);
		assertTrue(text == "This is a copy test.", "hfile - test static copy");
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
		assertTrue(hfile::exists(new_filename), "hfile - test static copy");
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
		assertTrue(i == 1234, "hfile - test static copy");
		assertTrue(s == 4321, "hfile - test static copy");
		assertTrue(str == "testing", "hfile - test static copy");
		assertTrue(e == 3.14f, "hfile - test static copy");
		assertTrue(d == 1.23456789999999, "hfile - test static copy");
		assertTrue(!b, "hfile - test static copy");
	}
}
