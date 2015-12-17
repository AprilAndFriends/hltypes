#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(Map)
{
	HL_UT_TEST_FUNCTION(adding);
	HL_UT_TEST_FUNCTION(removing);
	HL_UT_TEST_FUNCTION(structure);
	HL_UT_TEST_FUNCTION(iteration);
	HL_UT_TEST_FUNCTION(comparison);
	HL_UT_TEST_FUNCTION(random);
	HL_UT_TEST_FUNCTION(match);
	HL_UT_TEST_FUNCTION(cast);
}

HL_UT_RUN_CLASS_BEGIN(Map)
	HL_UT_RUN_METHOD(Map, adding)
	HL_UT_RUN_METHOD(Map, removing)
	HL_UT_RUN_METHOD(Map, structure)
	HL_UT_RUN_METHOD(Map, iteration)
	HL_UT_RUN_METHOD(Map, comparison)
	HL_UT_RUN_METHOD(Map, random)
	HL_UT_RUN_METHOD(Map, match)
	HL_UT_RUN_METHOD(Map, cast)
HL_UT_RUN_CLASS_END
