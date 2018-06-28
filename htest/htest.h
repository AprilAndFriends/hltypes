#ifndef HLTYPES_HTEST_H
#define HLTYPES_HTEST_H

#ifndef __HTEST_LIB_NAME
#pragma message("WARNING! __HTEST_LIB_NAME not defined!")
#endif

#include <hltypes/hstring.h>
#include <hltypes/hdir.h>

#define __EXPAND(x) x

#ifdef __APPLE__

#import <Foundation/NSString.h>
#import <XCTest/XCTest.h>

extern XCTestCase* testInstance;
#define HTEST_ASSERT(expression, msg) _XCTPrimitiveAssertTrue(testInstance, (expression), @#msg)

#define HTEST_RUN_CLASS_BEGIN(classe) \
	@interface _test ## classe: XCTestCase \
	@end \
	@implementation _test ## classe \
	- (void)setUp \
	{ \
		[super setUp]; \
		testInstance = self; \
	} \
	-(void)tearDown \
	{ \
		[super tearDown]; \
	}
#define HTEST_RUN_CLASS_END \
	@end

#define HTEST_RUN_METHOD(classe, name) \
	- (void) test_ ## classe ## _ ## name \
	{ \
		try\
		{\
			hstr dataDir = [[[NSBundle bundleForClass:[self class]] resourcePath] UTF8String];\
			hstr tempDir = hdir::joinPath([NSTemporaryDirectory() UTF8String], "htest");\
			hdir::createNew(tempDir);\
			_htest_ ## classe::name(dataDir, tempDir);\
		}\
		catch (hexception& e)\
		{\
			_XCTFailureHandler(self, YES, __FILE__, __LINE__, @"Unhandled Exception", @"%s", e.getMessage().cStr()); \
		}\
	}
#define HTEST_CLASS(classe) namespace _htest_ ## classe
#define HTEST_FUNCTION(name) void name(chstr dataDir, chstr tempDir)

// NEW

#define HTEST_SUITE_BEGIN(classe) \
	@interface test ## classe: XCTestCase \
	@end \
	@implementation test ## classe \
	- (void)setUp \
	{ \
		[super setUp]; \
		testInstance = self; \
	} \
	-(void)tearDown \
	{ \
		[super tearDown]; \
	}

#define HTEST_SUITE_END @end

#define HTEST_CASE(name) \
	- (void) test_ ## name \
	{ \
		try\
		{\
			hstr dataDir = [[[NSBundle bundleForClass:[self class]] resourcePath] UTF8String];\
			hstr tempDir = hdir::joinPath([NSTemporaryDirectory() UTF8String], "htest");\
			hdir::createNew(tempDir);\
			[self _test_ ## name:dataDir tempDir:tempDir];\
		}\
		catch (hexception& e)\
		{\
			_XCTFailureHandler(self, YES, __FILE__, __LINE__, @"Unhandled Exception", @"%s", e.getMessage().cStr()); \
		}\
	}\
	- (void) _test_ ## name:(chstr) dataDir tempDir:(chstr) tempDir

#else

#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const wchar_t* GetWC(const char* c);
#define HTEST_ASSERT(expression, msg) Assert::IsTrue((expression), GetWC(msg))

#define HTEST_RUN_CLASS_BEGIN(classe) \
	namespace __EXPAND(__HTEST_LIB_NAME) ## _htest \
	{ \
		TEST_CLASS(classe) \
		{ \
		public:
#define HTEST_RUN_CLASS_END \
		}; \
	}

#define HTEST_RUN_METHOD(classe, name) \
	TEST_METHOD(__EXPAND(__HTEST_LIB_NAME) ## _ ## classe ## _ ## name) \
	{ \
		__EXPAND(__HTEST_LIB_NAME) ## _htest_ ## classe::name(); \
	}

#define HTEST_CLASS(classe) namespace __EXPAND(__HTEST_LIB_NAME) ## _htest_ ## classe
#define HTEST_FUNCTION(name) void name()
#endif


#endif
