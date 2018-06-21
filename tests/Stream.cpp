#define __HTEST_LIB_NAME hltypes
#include "htest.h"
#include "harray.h"
#include "hexception.h"
#include "hstream.h"
#include "hstring.h"

HTEST_CLASS(Stream)
{
	static hstr output;
	
	HTEST_FUNCTION(readWrite)
	{
		hstream s;
		s.write("This is a test.");
		s.rewind();
		hstr text = s.read();
		HTEST_ASSERT(text == "This is a test.", "");
		s.rewind();
		text = s.read(6);
		HTEST_ASSERT(text == "This i", "");
		text = s.read(5);
		HTEST_ASSERT(text == "s a t", "");
	}

	HTEST_FUNCTION(readLine)
	{
		hstream s;
		s.writeLine("This is a test.");
		s.writeLine("This is also a test.");
		s.writeLine("This is another test.");
		s.rewind();
		hstr text = s.read();
		HTEST_ASSERT(text == "This is a test.\nThis is also a test.\nThis is another test.\n", "");
		s.rewind();
		text = s.readLine();
		HTEST_ASSERT(text == "This is a test.", "");
		text = s.readLine();
		HTEST_ASSERT(text == "This is also a test.", "");
		text = s.readLine();
		HTEST_ASSERT(text == "This is another test.", "");
		s.rewind();
		harray<hstr> lines = s.readLines();
		HTEST_ASSERT(lines[0] == "This is a test." && lines[1] == "This is also a test." && lines[2] == "This is another test.", "");
	}

	HTEST_FUNCTION(readDelimiter)
	{
		hstream s;
		s.write(hstr('a', 4090));
		s.write(hstr('b', 10));
		s.rewind();
		hstr text = s.read(hstr('b', 10));
		HTEST_ASSERT(text == hstr('a', 4090), "");
	}

	HTEST_FUNCTION(writef)
	{
		hstream s;
		s.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
		s.rewind();
		hstr text = s.read();
		HTEST_ASSERT(text == "This is a 0 formatted 3.14 file.", "");
	}

	HTEST_FUNCTION(readWriteRaw)
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
		HTEST_ASSERT(str == "Raw t", "");
	}

	HTEST_FUNCTION(seekPositionSize)
	{
		hstream s;
		s.write("This is another test.");
		s.rewind();
		s.seek(4, hseek::Start);
		hstr text = s.read();
		HTEST_ASSERT(text == " is another test.", "");
		s.rewind();
		s.seek(-4, hseek::End);
		int64_t pos = s.position();
		HTEST_ASSERT(pos == 17, "");
		text = s.read();
		HTEST_ASSERT(text == "est.", "");
		s.rewind();
		s.seek(4, hseek::Current);
		pos = s.position();
		HTEST_ASSERT(pos == 4, "");
		s.seek(5, hseek::Current);
		pos = s.position();
		HTEST_ASSERT(pos == 9, "");
		text = s.read(7);
		HTEST_ASSERT(s.size() == 21, "");
		HTEST_ASSERT(text == "nother ", "");
		s.clear();
		HTEST_ASSERT(s.size() == 0, "");
		HTEST_ASSERT(s.position() == 0, "");
	}

	HTEST_FUNCTION(serialization)
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
		HTEST_ASSERT(i == 1234, "");
		HTEST_ASSERT(si == 4321, "");
		HTEST_ASSERT(str == "testing", "");
		HTEST_ASSERT(e == 3.14f, "");
		HTEST_ASSERT(d == 1.23456789999999, "");
		HTEST_ASSERT(!b, "");
	}
}
