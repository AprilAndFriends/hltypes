#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "harray.h"
#include "hexception.h"
#include "hstream.h"
#include "hstring.h"

HL_UT_TEST_CLASS(Stream)
{
	static hstr output;
	
	HL_UT_TEST_FUNCTION(readWrite)
	{
		hstream s;
		s.write("This is a test.");
		s.rewind();
		hstr text = s.read();
		HL_UT_ASSERT(text == "This is a test.", "");
		s.rewind();
		text = s.read(6);
		HL_UT_ASSERT(text == "This i", "");
		text = s.read(5);
		HL_UT_ASSERT(text == "s a t", "");
	}

	HL_UT_TEST_FUNCTION(readLine)
	{
		hstream s;
		s.writeLine("This is a test.");
		s.writeLine("This is also a test.");
		s.writeLine("This is another test.");
		s.rewind();
		hstr text = s.read();
		HL_UT_ASSERT(text == "This is a test.\nThis is also a test.\nThis is another test.\n", "");
		s.rewind();
		text = s.readLine();
		HL_UT_ASSERT(text == "This is a test.", "");
		text = s.readLine();
		HL_UT_ASSERT(text == "This is also a test.", "");
		text = s.readLine();
		HL_UT_ASSERT(text == "This is another test.", "");
		s.rewind();
		harray<hstr> lines = s.readLines();
		HL_UT_ASSERT(lines[0] == "This is a test." && lines[1] == "This is also a test." && lines[2] == "This is another test.", "");
	}

	HL_UT_TEST_FUNCTION(readDelimiter)
	{
		hstream s;
		s.write(hstr('a', 4090));
		s.write(hstr('b', 10));
		s.rewind();
		hstr text = s.read(hstr('b', 10));
		HL_UT_ASSERT(text == hstr('a', 4090), "");
	}

	HL_UT_TEST_FUNCTION(writef)
	{
		hstream s;
		s.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
		s.rewind();
		hstr text = s.read();
		HL_UT_ASSERT(text == "This is a 0 formatted 3.14 file.", "");
	}

	HL_UT_TEST_FUNCTION(readWriteRaw)
	{
		hstream s;
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
		s.writeRaw(a, 5);
		s.rewind();
		unsigned char b[6] = { '\0' };
		s.readRaw(b, 5);
		hstr str = hstr((char*)b);
		HL_UT_ASSERT(str == "Raw t", "");
	}

	HL_UT_TEST_FUNCTION(seekPositionSize)
	{
		hstream s;
		s.write("This is another test.");
		s.rewind();
		s.seek(4, hstream::START);
		hstr text = s.read();
		HL_UT_ASSERT(text == " is another test.", "");
		s.rewind();
		s.seek(-4, hstream::END);
		int64_t pos = s.position();
		HL_UT_ASSERT(pos == 17, "");
		text = s.read();
		HL_UT_ASSERT(text == "est.", "");
		s.rewind();
		s.seek(4, hstream::CURRENT);
		pos = s.position();
		HL_UT_ASSERT(pos == 4, "");
		s.seek(5, hstream::CURRENT);
		pos = s.position();
		HL_UT_ASSERT(pos == 9, "");
		text = s.read(7);
		HL_UT_ASSERT(s.size() == 21, "");
		HL_UT_ASSERT(text == "nother ", "");
		s.clear();
		HL_UT_ASSERT(s.size() == 0, "");
		HL_UT_ASSERT(s.position() == 0, "");
	}

	HL_UT_TEST_FUNCTION(serialization)
	{
		hstream s;
		s.dump(1234);
		s.dump((int16_t)4321);
		s.dump(hstr("testing"));
		s.dump(3.14f);
		s.dump(1.23456789999999);
		s.dump(false);
		s.rewind();
		int i = s.loadInt32();
		short si = s.loadInt16();
		hstr str = s.loadString();
		float e = s.loadFloat();
		double d = s.loadDouble();
		bool b = s.loadBool();
		HL_UT_ASSERT(i == 1234, "");
		HL_UT_ASSERT(si == 4321, "");
		HL_UT_ASSERT(str == "testing", "");
		HL_UT_ASSERT(e == 3.14f, "");
		HL_UT_ASSERT(d == 1.23456789999999, "");
		HL_UT_ASSERT(!b, "");
	}
}
