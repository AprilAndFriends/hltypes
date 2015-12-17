#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

HL_UT_TEST_CLASS(Array)
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
	HL_UT_TEST_FUNCTION(random);
	HL_UT_TEST_FUNCTION(join);
	HL_UT_TEST_FUNCTION(matching);
	HL_UT_TEST_FUNCTION(cast);
	HL_UT_TEST_FUNCTION(incorrectNegativeIndex);
	HL_UT_TEST_FUNCTION(correctNegativeIndex);
	HL_UT_TEST_FUNCTION(positiveIndex);
	HL_UT_TEST_FUNCTION(outOfBoundsAssignment);
}

HL_UT_RUN_CLASS_BEGIN(Array)
	HL_UT_RUN_METHOD(Array, adding)
	HL_UT_RUN_METHOD(Array, adding2)
	HL_UT_RUN_METHOD(Array, adding3)
	HL_UT_RUN_METHOD(Array, removing1)
	HL_UT_RUN_METHOD(Array, removing2)
	HL_UT_RUN_METHOD(Array, container)
	HL_UT_RUN_METHOD(Array, comparison)
	HL_UT_RUN_METHOD(Array, operations)
	HL_UT_RUN_METHOD(Array, iteration)
	HL_UT_RUN_METHOD(Array, constructor)
	HL_UT_RUN_METHOD(Array, intersection)
	HL_UT_RUN_METHOD(Array, union_)
	HL_UT_RUN_METHOD(Array, difference)
	HL_UT_RUN_METHOD(Array, random)
	HL_UT_RUN_METHOD(Array, join)
	HL_UT_RUN_METHOD(Array, matching)
	HL_UT_RUN_METHOD(Array, cast)
	HL_UT_RUN_METHOD(Array, incorrectNegativeIndex)
	HL_UT_RUN_METHOD(Array, correctNegativeIndex)
	HL_UT_RUN_METHOD(Array, positiveIndex)
	HL_UT_RUN_METHOD(Array, outOfBoundsAssignment)
HL_UT_RUN_CLASS_END
