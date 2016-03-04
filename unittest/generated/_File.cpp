#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(File)
{
	HL_UT_TEST_FUNCTION(readWrite);
	HL_UT_TEST_FUNCTION(readLine);
	HL_UT_TEST_FUNCTION(readDelimiter);
	HL_UT_TEST_FUNCTION(writef);
	HL_UT_TEST_FUNCTION(readWriteRaw);
	HL_UT_TEST_FUNCTION(seekPositionSize);
	HL_UT_TEST_FUNCTION(serialization);
	HL_UT_TEST_FUNCTION(staticClear);
	HL_UT_TEST_FUNCTION(staticRename);
	HL_UT_TEST_FUNCTION(staticMove);
	HL_UT_TEST_FUNCTION(staticCopy);
}

HL_UT_RUN_CLASS_BEGIN(File)
	HL_UT_RUN_METHOD(File, readWrite)
	HL_UT_RUN_METHOD(File, readLine)
	HL_UT_RUN_METHOD(File, readDelimiter)
	HL_UT_RUN_METHOD(File, writef)
	HL_UT_RUN_METHOD(File, readWriteRaw)
	HL_UT_RUN_METHOD(File, seekPositionSize)
	HL_UT_RUN_METHOD(File, serialization)
	HL_UT_RUN_METHOD(File, staticClear)
	HL_UT_RUN_METHOD(File, staticRename)
	HL_UT_RUN_METHOD(File, staticMove)
	HL_UT_RUN_METHOD(File, staticCopy)
HL_UT_RUN_CLASS_END
