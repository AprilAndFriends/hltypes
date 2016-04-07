#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(Deque)
{
	HL_UT_TEST_FUNCTION(adding);
}

HL_UT_RUN_CLASS_BEGIN(Deque)
	HL_UT_RUN_METHOD(Deque, adding)
HL_UT_RUN_CLASS_END
