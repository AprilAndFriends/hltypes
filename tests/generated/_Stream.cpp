#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(Stream)
{
	HTEST_FUNCTION(readWrite);
	HTEST_FUNCTION(readLine);
	HTEST_FUNCTION(readDelimiter);
	HTEST_FUNCTION(writef);
	HTEST_FUNCTION(readWriteRaw);
	HTEST_FUNCTION(seekPositionSize);
	HTEST_FUNCTION(serialization);
}

HTEST_RUN_CLASS_BEGIN(Stream)
	HTEST_RUN_METHOD(Stream, readWrite)
	HTEST_RUN_METHOD(Stream, readLine)
	HTEST_RUN_METHOD(Stream, readDelimiter)
	HTEST_RUN_METHOD(Stream, writef)
	HTEST_RUN_METHOD(Stream, readWriteRaw)
	HTEST_RUN_METHOD(Stream, seekPositionSize)
	HTEST_RUN_METHOD(Stream, serialization)
HTEST_RUN_CLASS_END
