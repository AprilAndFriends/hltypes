#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(String)
{
	HTEST_FUNCTION(floatingPoint);
	HTEST_FUNCTION(integer);
	HTEST_FUNCTION(boolean);
	HTEST_FUNCTION(cstrCompatibility);
	HTEST_FUNCTION(lowered);
	HTEST_FUNCTION(uppered);
	HTEST_FUNCTION(reverse);
	HTEST_FUNCTION(trim);
	HTEST_FUNCTION(replace);
	HTEST_FUNCTION(insertAt);
	HTEST_FUNCTION(split1);
	HTEST_FUNCTION(split2);
	HTEST_FUNCTION(split3);
	HTEST_FUNCTION(split4);
	HTEST_FUNCTION(rsplit1);
	HTEST_FUNCTION(rsplit2);
	HTEST_FUNCTION(rsplit3);
	HTEST_FUNCTION(rsplit4);
	HTEST_FUNCTION(indexOf);
	HTEST_FUNCTION(rindexOf);
	HTEST_FUNCTION(indexOfAny);
	HTEST_FUNCTION(rindexOfAny);
	HTEST_FUNCTION(count);
	HTEST_FUNCTION(startsWith);
	HTEST_FUNCTION(endsWith);
	HTEST_FUNCTION(contains);
	HTEST_FUNCTION(isNumeric);
	HTEST_FUNCTION(substrOperator);
	HTEST_FUNCTION(hex);
	HTEST_FUNCTION(formatting);
}

HTEST_RUN_CLASS_BEGIN(String)
	HTEST_RUN_METHOD(String, floatingPoint)
	HTEST_RUN_METHOD(String, integer)
	HTEST_RUN_METHOD(String, boolean)
	HTEST_RUN_METHOD(String, cstrCompatibility)
	HTEST_RUN_METHOD(String, lowered)
	HTEST_RUN_METHOD(String, uppered)
	HTEST_RUN_METHOD(String, reverse)
	HTEST_RUN_METHOD(String, trim)
	HTEST_RUN_METHOD(String, replace)
	HTEST_RUN_METHOD(String, insertAt)
	HTEST_RUN_METHOD(String, split1)
	HTEST_RUN_METHOD(String, split2)
	HTEST_RUN_METHOD(String, split3)
	HTEST_RUN_METHOD(String, split4)
	HTEST_RUN_METHOD(String, rsplit1)
	HTEST_RUN_METHOD(String, rsplit2)
	HTEST_RUN_METHOD(String, rsplit3)
	HTEST_RUN_METHOD(String, rsplit4)
	HTEST_RUN_METHOD(String, indexOf)
	HTEST_RUN_METHOD(String, rindexOf)
	HTEST_RUN_METHOD(String, indexOfAny)
	HTEST_RUN_METHOD(String, rindexOfAny)
	HTEST_RUN_METHOD(String, count)
	HTEST_RUN_METHOD(String, startsWith)
	HTEST_RUN_METHOD(String, endsWith)
	HTEST_RUN_METHOD(String, contains)
	HTEST_RUN_METHOD(String, isNumeric)
	HTEST_RUN_METHOD(String, substrOperator)
	HTEST_RUN_METHOD(String, hex)
	HTEST_RUN_METHOD(String, formatting)
HTEST_RUN_CLASS_END
