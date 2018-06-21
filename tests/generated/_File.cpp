#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(File)
{
	HTEST_FUNCTION(readWrite);
	HTEST_FUNCTION(readLine);
	HTEST_FUNCTION(readDelimiter);
	HTEST_FUNCTION(writef);
	HTEST_FUNCTION(readWriteRaw);
	HTEST_FUNCTION(seekPositionSize);
	HTEST_FUNCTION(serialization);
	HTEST_FUNCTION(staticClear);
	HTEST_FUNCTION(staticRename);
	HTEST_FUNCTION(staticMove);
	HTEST_FUNCTION(staticCopy);
}

HTEST_RUN_CLASS_BEGIN(File)
	HTEST_RUN_METHOD(File, readWrite)
	HTEST_RUN_METHOD(File, readLine)
	HTEST_RUN_METHOD(File, readDelimiter)
	HTEST_RUN_METHOD(File, writef)
	HTEST_RUN_METHOD(File, readWriteRaw)
	HTEST_RUN_METHOD(File, seekPositionSize)
	HTEST_RUN_METHOD(File, serialization)
	HTEST_RUN_METHOD(File, staticClear)
	HTEST_RUN_METHOD(File, staticRename)
	HTEST_RUN_METHOD(File, staticMove)
	HTEST_RUN_METHOD(File, staticCopy)
HTEST_RUN_CLASS_END
