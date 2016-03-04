#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(List)
{
	HL_UT_TEST_FUNCTION(adding);
	HL_UT_TEST_FUNCTION(adding2);
	HL_UT_TEST_FUNCTION(adding3);
	HL_UT_TEST_FUNCTION(removing1);
	HL_UT_TEST_FUNCTION(removing2);
	HL_UT_TEST_FUNCTION(container);
	HL_UT_TEST_FUNCTION(comparison);
	HL_UT_TEST_FUNCTION(operations);
	HL_UT_TEST_FUNCTION(iteration);
}

HL_UT_RUN_CLASS_BEGIN(List)
	HL_UT_RUN_METHOD(List, adding)
	HL_UT_RUN_METHOD(List, adding2)
	HL_UT_RUN_METHOD(List, adding3)
	HL_UT_RUN_METHOD(List, removing1)
	HL_UT_RUN_METHOD(List, removing2)
	HL_UT_RUN_METHOD(List, container)
	HL_UT_RUN_METHOD(List, comparison)
	HL_UT_RUN_METHOD(List, operations)
	HL_UT_RUN_METHOD(List, iteration)
HL_UT_RUN_CLASS_END
