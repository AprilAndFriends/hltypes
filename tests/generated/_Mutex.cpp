#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(Mutex)
{
	HTEST_FUNCTION(lockRelease);
	HTEST_FUNCTION(massLockRelease);
}

HTEST_RUN_CLASS_BEGIN(Mutex)
	HTEST_RUN_METHOD(Mutex, lockRelease)
	HTEST_RUN_METHOD(Mutex, massLockRelease)
HTEST_RUN_CLASS_END
