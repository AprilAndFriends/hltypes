namespace testDir
{
	void testStaticCreateRemove();
	void testStaticClear();
	void testStaticRename();
	void testStaticMove();
	void testStaticCopy();
}

#ifdef __APPLE__
#import "aprilUT.h"

@interface _testDir : XCTestCase

@end

@implementation _testDir

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
	TEST_CLASS(_testDir)
	{
	public:
		TEST_METHOD(testStaticCreateRemove)
		{
			testDir::testStaticCreateRemove();
		}
		TEST_METHOD(testStaticClear)
		{
			testDir::testStaticClear();
		}
		TEST_METHOD(testStaticRename)
		{
			testDir::testStaticRename();
		}
		TEST_METHOD(testStaticMove)
		{
			testDir::testStaticMove();
		}
		TEST_METHOD(testStaticCopy)
		{
			testDir::testStaticCopy();
		}
	};
}
#endif
