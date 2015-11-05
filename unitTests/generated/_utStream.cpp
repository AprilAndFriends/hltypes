namespace testStream
{
	void testReadWrite();
	void testReadLine();
	void testReadDelimiter();
	void testWritef();
	void testReadWriteRaw();
	void testSeekPositionSize();
	void testSerialization();
}

#ifdef __APPLE__
#import "aprilUT.h"

@interface _testStream : XCTestCase

@end

@implementation _testStream

- (void)setUp
{
	[super setUp];
	testInstance = self;
}

-(void)tearDown
{
	[super tearDown];
}


@end

#else
#include "aprilUT.h"

namespace test_libhltypes
{
	TEST_CLASS(_testStream)
	{
	public:
		TEST_METHOD(testReadWrite)
		{
			testStream::testReadWrite();
		}
		TEST_METHOD(testReadLine)
		{
			testStream::testReadLine();
		}
		TEST_METHOD(testReadDelimiter)
		{
			testStream::testReadDelimiter();
		}
		TEST_METHOD(testWritef)
		{
			testStream::testWritef();
		}
		TEST_METHOD(testReadWriteRaw)
		{
			testStream::testReadWriteRaw();
		}
		TEST_METHOD(testSeekPositionSize)
		{
			testStream::testSeekPositionSize();
		}
		TEST_METHOD(testSerialization)
		{
			testStream::testSerialization();
		}
	};
}
#endif
