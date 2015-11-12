namespace testArray
{
	void testAdding1();
	void testAdding2();
	void testAdding3();
	void testRemoving1();
	void testRemoving2();
	void testContainer();
	void testComparison();
	void testOperations();
	void testIterations();
	void testConstructor();
	void testIntersection();
	void testUnion();
	void testDifference();
	void testRandom();
	void testJoin();
	void testMatch();
	void testCast();
	void testIncorrectNegativeIndex();
	void testCorrectNegativeIndex();
	void testPositiveIndex();
	void testOutOfBoundsAssignment();
}

#ifdef __APPLE__
#import "aprilUT.h"

@interface _testArray : XCTestCase

@end

@implementation _testArray

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
	TEST_CLASS(_testArray)
	{
	public:
		TEST_METHOD(testAdding1)
		{
			testArray::testAdding1();
		}	
		TEST_METHOD(testAdding2)
		{
			testArray::testAdding2();
		}
		TEST_METHOD(testAdding3)
		{
			testArray::testAdding3();
		}
		TEST_METHOD(testRemoving1)
		{
			testArray::testRemoving1();
		}
		TEST_METHOD(testRemoving2)
		{
			testArray::testRemoving2();
		}
		TEST_METHOD(testContainer)
		{
			testArray::testContainer();
		}
		TEST_METHOD(testComparison)
		{
			testArray::testComparison();
		}
		TEST_METHOD(testOperations)
		{
			testArray::testOperations();
		}
		TEST_METHOD(testIterations)
		{
			testArray::testIterations();
		}
		TEST_METHOD(testConstructor)
		{
			testArray::testConstructor();
		}
		TEST_METHOD(testIntersection)
		{
			testArray::testIntersection();
		}
		TEST_METHOD(testUnion)
		{
			testArray::testUnion();
		}
		TEST_METHOD(testDifference)
		{
			testArray::testDifference();
		}
		TEST_METHOD(testRandom)
		{
			testArray::testRandom();
		}
		TEST_METHOD(testJoin)
		{
			testArray::testJoin();
		}
		TEST_METHOD(testMatch)
		{
			testArray::testMatch();
		}
		TEST_METHOD(testCast)
		{
			testArray::testCast();
		}
		TEST_METHOD(testIncorrectNegativeIndex)
		{
			testArray::testIncorrectNegativeIndex();
		}
		TEST_METHOD(testCorrectNegativeIndex)
		{
			testArray::testCorrectNegativeIndex();
		}
		TEST_METHOD(testPositiveIndex)
		{
			testArray::testPositiveIndex();
		}
		TEST_METHOD(testOutOfBoundsAssignment)
		{
			testArray::testOutOfBoundsAssignment();
		}
	};
}
#endif
