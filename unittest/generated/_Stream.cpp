#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(Stream)
{
	HL_UT_TEST_FUNCTION(readWrite);
	HL_UT_TEST_FUNCTION(readLine);
	HL_UT_TEST_FUNCTION(readDelimiter);
	HL_UT_TEST_FUNCTION(writef);
	HL_UT_TEST_FUNCTION(readWriteRaw);
	HL_UT_TEST_FUNCTION(seekPositionSize);
	HL_UT_TEST_FUNCTION(serialization);
}

HL_UT_RUN_CLASS_BEGIN(Stream)
	HL_UT_RUN_METHOD(Stream, readWrite)
	HL_UT_RUN_METHOD(Stream, readLine)
	HL_UT_RUN_METHOD(Stream, readDelimiter)
	HL_UT_RUN_METHOD(Stream, writef)
	HL_UT_RUN_METHOD(Stream, readWriteRaw)
	HL_UT_RUN_METHOD(Stream, seekPositionSize)
	HL_UT_RUN_METHOD(Stream, serialization)
HL_UT_RUN_CLASS_END
