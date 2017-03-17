#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(Mutex)
{
	HL_UT_TEST_FUNCTION(lockRelease);
	HL_UT_TEST_FUNCTION(massLockRelease);
}

HL_UT_RUN_CLASS_BEGIN(Mutex)
	HL_UT_RUN_METHOD(Mutex, lockRelease)
	HL_UT_RUN_METHOD(Mutex, massLockRelease)
HL_UT_RUN_CLASS_END
