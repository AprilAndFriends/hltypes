namespace testThread
{
	void testThreadingManual();
	void testThreadingBasic();
	void testThreadingControl();
}

#ifdef __APPLE__
#import "aprilUT.h"

@interface _testThread : XCTestCase

@end

@implementation _testThread

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
	TEST_CLASS(_testThread)
	{
	public:
		TEST_METHOD(testThreadingManual)
		{
			testThread::testThreadingManual();
		}
		TEST_METHOD(testThreadingBasic)
		{
			testThread::testThreadingBasic();
		}
		TEST_METHOD(testThreadingControl)
		{
			testThread::testThreadingControl();
		}
	};
}
#endif
