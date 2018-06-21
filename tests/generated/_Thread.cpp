#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(Thread)
{
	HTEST_FUNCTION(testThreadingBasic);
	HTEST_FUNCTION(testThreadingControl);
}

HTEST_RUN_CLASS_BEGIN(Thread)
	HTEST_RUN_METHOD(Thread, testThreadingBasic)
	HTEST_RUN_METHOD(Thread, testThreadingControl)
HTEST_RUN_CLASS_END
