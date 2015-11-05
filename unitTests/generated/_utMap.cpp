namespace testMap
{
	void testAdding();
	void testRemoving();
	void testStructure();
	void testIteration();
	void testComparison();
	void testRandom();
	void testMatch();
	void testCast();
}

#ifdef __APPLE__
#import "aprilUT.h"

@interface _testMap : XCTestCase

@end

@implementation _testMap

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
	TEST_CLASS(_testMap)
	{
	public:
		TEST_METHOD(testAdding)
		{
			testMap::testAdding();
		}
		TEST_METHOD(testRemoving)
		{
			testMap::testRemoving();
		}
		TEST_METHOD(testStructure)
		{
			testMap::testStructure();
		}
		TEST_METHOD(testIteration)
		{
			testMap::testIteration();
		}
		TEST_METHOD(testComparison)
		{
			testMap::testComparison();
		}
		TEST_METHOD(testRandom)
		{
			testMap::testRandom();
		}
		TEST_METHOD(testMatch)
		{
			testMap::testMatch();
		}
		TEST_METHOD(testCast)
		{
			testMap::testCast();
		}
	};
}
#endif
