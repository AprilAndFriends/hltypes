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
	HL_UT_TEST_FUNCTION(constructor);
	HL_UT_TEST_FUNCTION(intersection);
	HL_UT_TEST_FUNCTION(union_);
	HL_UT_TEST_FUNCTION(difference);
	HL_UT_TEST_FUNCTION(join);
	HL_UT_TEST_FUNCTION(matching);
	HL_UT_TEST_FUNCTION(random);
	HL_UT_TEST_FUNCTION(cast);
	HL_UT_TEST_FUNCTION(incorrectNegativeIndex);
	HL_UT_TEST_FUNCTION(correctNegativeIndex);
	HL_UT_TEST_FUNCTION(positiveIndex);
	HL_UT_TEST_FUNCTION(outOfBoundsAssignment);
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
	HL_UT_RUN_METHOD(List, constructor)
	HL_UT_RUN_METHOD(List, intersection)
	HL_UT_RUN_METHOD(List, union_)
	HL_UT_RUN_METHOD(List, difference)
	HL_UT_RUN_METHOD(List, join)
	HL_UT_RUN_METHOD(List, matching)
	HL_UT_RUN_METHOD(List, random)
	HL_UT_RUN_METHOD(List, cast)
	HL_UT_RUN_METHOD(List, incorrectNegativeIndex)
	HL_UT_RUN_METHOD(List, correctNegativeIndex)
	HL_UT_RUN_METHOD(List, positiveIndex)
	HL_UT_RUN_METHOD(List, outOfBoundsAssignment)
HL_UT_RUN_CLASS_END
