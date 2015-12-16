#ifndef __HL_TEST_LIB_NAME
#pragma message("WARNING! __HL_TEST_LIB_NAME not defined!")
#endif

#ifdef __APPLE__

#import <XCTest/XCTest.h>

extern XCTestCase* testInstance;
#define assertTrue(expression, msg) _XCTPrimitiveAssertTrue(testInstance, (expression), @msg)

#define HL_TEST_CLASS_BEGIN(classe) TEST_CLASS(_test ## classe) \
	@interface _test ## classe \
	@end \
	- (void)setUp \
	{ \
		[super setUp]; \
		testInstance = self; \
	} \
	-(void)tearDown \
	{ \
		[super tearDown]; \
	}
#define HL_TEST_CLASS_END \
	@end

#define HL_TEST_METHOD(classe, name) \
	- (void)__HL_TEST_LIB_NAME ## _ ## classe ## _ ## name) \
	{ \
		__HL_TEST_LIB_NAME ## _unittest_ ## classe::assignment(); \
	}

#else

#define __REDUCE(x) x

#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const wchar_t* GetWC(const char* c);
#define assertTrue(expression, msg) Assert::IsTrue((expression), GetWC(msg))

#define HL_TEST_CLASS_BEGIN(classe) \
	namespace test_ ## __HL_TEST_LIB_NAME \
	{ \
		TEST_CLASS(_test_ ## classe) \
		{ \
		public:
#define HL_TEST_CLASS_END \
		}; \
	}

#define HL_TEST_METHOD(classe, name) \
	TEST_METHOD(__REDUCE(__HL_TEST_LIB_NAME) ## _ ## classe ## _ ## name) \
	{ \
		__HL_TEST_LIB_NAME ## _unittest_ ## classe::assignment(); \
	}

#endif

#define HL_TEST_NAMESPACE(classe) namespace __HL_TEST_LIB_NAME ## _unittest_ ## classe
