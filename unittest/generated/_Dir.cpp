#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(Dir)
{
	HL_UT_TEST_FUNCTION(staticCreateRemove);
}

HL_UT_RUN_CLASS_BEGIN(Dir)
	HL_UT_RUN_METHOD(Dir, staticCreateRemove)
HL_UT_RUN_CLASS_END
