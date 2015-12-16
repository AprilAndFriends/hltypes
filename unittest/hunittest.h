#ifndef HL_UT_HUNITTEST_H
#define HL_UT_HUNITTEST_H

#ifndef __HL_UT_LIB_NAME
#pragma message("WARNING! __HL_UT_LIB_NAME not defined!")
#endif

#define __EXPAND(x) x

#ifdef __APPLE__

#import <XCTest/XCTest.h>

extern XCTestCase* testInstance;
#define HL_UT_ASSERT(expression, msg) _XCTPrimitiveAssertTrue(testInstance, (expression), @msg)

#define HL_UT_CLASS_BEGIN(classe) \
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
#define HL_UT_CLASS_END \
	@end

#define HL_UT_RUN_METHOD(classe, name) \
	- (void)__HL_UT_LIB_NAME ## _ ## classe ## _ ## name) \
	{ \
		__HL_UT_LIB_NAME ## _unittest_ ## classe::assignment(); \
	}

#else

#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const wchar_t* GetWC(const char* c);
#define HL_UT_ASSERT(expression, msg) Assert::IsTrue((expression), GetWC(msg))

#define HL_UT_RUN_CLASS_BEGIN(classe) \
	namespace __EXPAND(__HL_UT_LIB_NAME) ## _hunittest \
	{ \
		TEST_CLASS(classe) \
		{ \
		public:
#define HL_UT_RUN_CLASS_END \
		}; \
	}

#define HL_UT_RUN_METHOD(classe, name) \
	TEST_METHOD(__EXPAND(__HL_UT_LIB_NAME) ## _ ## classe ## _ ## name) \
	{ \
		__EXPAND(__HL_UT_LIB_NAME) ## _hunittest_ ## classe::name(); \
	}

#endif

#define HL_UT_TEST_CLASS(classe) namespace __EXPAND(__HL_UT_LIB_NAME) ## _hunittest_ ## classe
#define HL_UT_TEST_FUNCTION(name) void name()

#endif
