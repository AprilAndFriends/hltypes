namespace testFile
{
	void testFileReadWrite();
	void testFileReadLine();
	void testFileReadDelimiter();
	void testFileWritef();
	void testFileReadWriteRaw();
	void testFileSeekPositionSize();
	void testFileSerialization();
	void testFileStaticClear();
	void testFileStaticRename();
	void testFileStaticMove();
	void testFileStaticCopy();
}

#ifdef __APPLE__
#import "aprilUT.h"

@interface _testFile : XCTestCase

@end

@implementation _testFile

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
	TEST_CLASS(_testFile)
	{
	public:
		TEST_METHOD(testFileReadWrite)
		{
			testFile::testFileReadWrite();
		}
		TEST_METHOD(testFileReadLine)
		{
			testFile::testFileReadLine();
		}
		TEST_METHOD(testFileReadDelimiter)
		{
			testFile::testFileReadDelimiter();
		}
		TEST_METHOD(testFileWritef)
		{
			testFile::testFileWritef();
		}
		TEST_METHOD(testFileReadWriteRaw)
		{
			testFile::testFileReadWriteRaw();
		}
		TEST_METHOD(testFileSeekPositionSize)
		{
			testFile::testFileSeekPositionSize();
		}
		TEST_METHOD(testFileSerialization)
		{
			testFile::testFileSerialization();
		}
		TEST_METHOD(testFileStaticClear)
		{
			testFile::testFileStaticClear();
		}
		TEST_METHOD(testFileStaticRename)
		{
			testFile::testFileStaticRename();
		}
		TEST_METHOD(testFileStaticMove)
		{
			testFile::testFileStaticMove();
		}
		TEST_METHOD(testFileStaticCopy)
		{
			testFile::testFileStaticCopy();
		}
	};
}
#endif
