#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(Thread)
{
	HL_UT_TEST_FUNCTION(testThreadingManual);
	HL_UT_TEST_FUNCTION(testThreadingBasic);
	HL_UT_TEST_FUNCTION(testThreadingControl);
}

HL_UT_RUN_CLASS_BEGIN(Thread)
	HL_UT_RUN_METHOD(Thread, testThreadingManual)
	HL_UT_RUN_METHOD(Thread, testThreadingBasic)
	HL_UT_RUN_METHOD(Thread, testThreadingControl)
HL_UT_RUN_CLASS_END
