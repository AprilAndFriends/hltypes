#include "generated/aprilUT.h"

#include "harray.h"
#include "hexception.h"
#include "hstream.h"
#include "hstring.h"

namespace testStream
{
	hstr output;
	
	void testReadWrite()
	{
		hstream s;
		s.write("This is a test.");
		s.rewind();
		hstr text = s.read();
		assertTrue(text == "This is a test.", "hstream - read/write");
		s.rewind();
		text = s.read(6);
		assertTrue(text == "This i", "hstream - read/write");
		text = s.read(5);
		assertTrue(text == "s a t", "hstream - read/write");
	}

	void testReadLine()
	{
		hstream s;
		s.writeLine("This is a test.");
		s.writeLine("This is also a test.");
		s.writeLine("This is another test.");
		s.rewind();
		hstr text = s.read();
		assertTrue(text == "This is a test.\nThis is also a test.\nThis is another test.\n", "hstream - read line");
		s.rewind();
		text = s.readLine();
		assertTrue(text == "This is a test.", "hstream - read line");
		text = s.readLine();
		assertTrue(text == "This is also a test.", "hstream - read line");
		text = s.readLine();
		assertTrue(text == "This is another test.", "hstream - read line");
		s.rewind();
		harray<hstr> lines = s.readLines();
		assertTrue(lines[0] == "This is a test." && lines[1] == "This is also a test." && lines[2] == "This is another test.", "hstream - read line");
	}

	void testReadDelimiter()
	{
		hstream s;
		s.write(hstr('a', 4090));
		s.write(hstr('b', 10));
		s.rewind();
		hstr text = s.read(hstr('b', 10));
		assertTrue(text == hstr('a', 4090), "hstream - read delimiter");
	}

	void testWritef()
	{
		hstream s;
		s.writef("This is a %d %s %4.2f %s.", 0, "formatted", 3.14f, "file");
		s.rewind();
		hstr text = s.read();
		assertTrue(text == "This is a 0 formatted 3.14 file.", "hstream - writef");
	}

	void testReadWriteRaw()
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
		assertTrue(str == "Raw t", "hstream - read/write raw");
	}

	void testSeekPositionSize()
	{
		hstream s;
		s.write("This is another test.");
		s.rewind();
		s.seek(4, hstream::START);
		hstr text = s.read();
		assertTrue(text == " is another test.", "hstream - seek/position/size");
		s.rewind();
		s.seek(-4, hstream::END);
		int pos = s.position();
		assertTrue(pos == 17, "hstream - seek/position/size");
		text = s.read();
		assertTrue(text == "est.", "hstream - seek/position/size");
		s.rewind();
		s.seek(4, hstream::CURRENT);
		pos = s.position();
		assertTrue(pos == 4, "hstream - seek/position/size");
		s.seek(5, hstream::CURRENT);
		pos = s.position();
		assertTrue(pos == 9, "hstream - seek/position/size");
		text = s.read(7);
		assertTrue(s.size() == 21, "hstream - seek/position/size");
		assertTrue(text == "nother ", "hstream - seek/position/size");
		s.clear();
		assertTrue(s.size() == 0, "hstream - seek/position/size");
		assertTrue(s.position() == 0, "hstream - seek/position/size");
	}

	void testSerialization()
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
		assertTrue(i == 1234, "hstream - serialization");
		assertTrue(si == 4321, "hstream - serialization");
		assertTrue(str == "testing", "hstream - serialization");
		assertTrue(e == 3.14f, "hstream - serialization");
		assertTrue(d == 1.23456789999999, "hstream - serialization");
		assertTrue(!b, "hstream - serialization");
	}
}
