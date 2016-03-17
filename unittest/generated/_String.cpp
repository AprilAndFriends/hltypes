#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(String)
{
	HL_UT_TEST_FUNCTION(floatingPoint);
	HL_UT_TEST_FUNCTION(integer);
	HL_UT_TEST_FUNCTION(boolean);
	HL_UT_TEST_FUNCTION(stdstrCompatibility);
	HL_UT_TEST_FUNCTION(cstrCompatibility);
	HL_UT_TEST_FUNCTION(replace);
	HL_UT_TEST_FUNCTION(startsWith);
	HL_UT_TEST_FUNCTION(endsWith);
	HL_UT_TEST_FUNCTION(split1);
	HL_UT_TEST_FUNCTION(split2);
	HL_UT_TEST_FUNCTION(split3);
	HL_UT_TEST_FUNCTION(rsplit1);
	HL_UT_TEST_FUNCTION(rsplit2);
	HL_UT_TEST_FUNCTION(rsplit3);
	HL_UT_TEST_FUNCTION(substrOperator);
	HL_UT_TEST_FUNCTION(count);
	HL_UT_TEST_FUNCTION(caps);
	HL_UT_TEST_FUNCTION(reverse);
	HL_UT_TEST_FUNCTION(isNumeric);
	HL_UT_TEST_FUNCTION(trim);
	HL_UT_TEST_FUNCTION(formatting);
}

HL_UT_RUN_CLASS_BEGIN(String)
	HL_UT_RUN_METHOD(String, floatingPoint)
	HL_UT_RUN_METHOD(String, integer)
	HL_UT_RUN_METHOD(String, boolean)
	HL_UT_RUN_METHOD(String, stdstrCompatibility)
	HL_UT_RUN_METHOD(String, cstrCompatibility)
	HL_UT_RUN_METHOD(String, replace)
	HL_UT_RUN_METHOD(String, startsWith)
	HL_UT_RUN_METHOD(String, endsWith)
	HL_UT_RUN_METHOD(String, split1)
	HL_UT_RUN_METHOD(String, split2)
	HL_UT_RUN_METHOD(String, split3)
	HL_UT_RUN_METHOD(String, rsplit1)
	HL_UT_RUN_METHOD(String, rsplit2)
	HL_UT_RUN_METHOD(String, rsplit3)
	HL_UT_RUN_METHOD(String, substrOperator)
	HL_UT_RUN_METHOD(String, count)
	HL_UT_RUN_METHOD(String, caps)
	HL_UT_RUN_METHOD(String, reverse)
	HL_UT_RUN_METHOD(String, isNumeric)
	HL_UT_RUN_METHOD(String, trim)
	HL_UT_RUN_METHOD(String, formatting)
HL_UT_RUN_CLASS_END
