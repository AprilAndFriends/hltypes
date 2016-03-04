#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(Dir)
{
	HL_UT_TEST_FUNCTION(staticCreateRemove);
	HL_UT_TEST_FUNCTION(staticClear);
	HL_UT_TEST_FUNCTION(staticRename);
	HL_UT_TEST_FUNCTION(staticMove);
	HL_UT_TEST_FUNCTION(staticCopy);
}

HL_UT_RUN_CLASS_BEGIN(Dir)
	HL_UT_RUN_METHOD(Dir, staticCreateRemove)
	HL_UT_RUN_METHOD(Dir, staticClear)
	HL_UT_RUN_METHOD(Dir, staticRename)
	HL_UT_RUN_METHOD(Dir, staticMove)
	HL_UT_RUN_METHOD(Dir, staticCopy)
HL_UT_RUN_CLASS_END
