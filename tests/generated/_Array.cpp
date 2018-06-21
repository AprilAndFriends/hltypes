#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(Array)
{
	HTEST_FUNCTION(adding);
	HTEST_FUNCTION(adding2);
	HTEST_FUNCTION(adding3);
	HTEST_FUNCTION(removing1);
	HTEST_FUNCTION(removing2);
	HTEST_FUNCTION(container);
	HTEST_FUNCTION(comparison);
	HTEST_FUNCTION(operations);
	HTEST_FUNCTION(iteration);
	HTEST_FUNCTION(constructor);
	HTEST_FUNCTION(intersection);
	HTEST_FUNCTION(union_);
	HTEST_FUNCTION(difference);
	HTEST_FUNCTION(random);
	HTEST_FUNCTION(randomize);
	HTEST_FUNCTION(join);
	HTEST_FUNCTION(matching);
	HTEST_FUNCTION(cast);
	HTEST_FUNCTION(incorrectNegativeIndex);
	HTEST_FUNCTION(correctNegativeIndex);
	HTEST_FUNCTION(positiveIndex);
	HTEST_FUNCTION(outOfBoundsAssignment);
}

HTEST_RUN_CLASS_BEGIN(Array)
	HTEST_RUN_METHOD(Array, adding)
	HTEST_RUN_METHOD(Array, adding2)
	HTEST_RUN_METHOD(Array, adding3)
	HTEST_RUN_METHOD(Array, removing1)
	HTEST_RUN_METHOD(Array, removing2)
	HTEST_RUN_METHOD(Array, container)
	HTEST_RUN_METHOD(Array, comparison)
	HTEST_RUN_METHOD(Array, operations)
	HTEST_RUN_METHOD(Array, iteration)
	HTEST_RUN_METHOD(Array, constructor)
	HTEST_RUN_METHOD(Array, intersection)
	HTEST_RUN_METHOD(Array, union_)
	HTEST_RUN_METHOD(Array, difference)
	HTEST_RUN_METHOD(Array, random)
	HTEST_RUN_METHOD(Array, randomize)
	HTEST_RUN_METHOD(Array, join)
	HTEST_RUN_METHOD(Array, matching)
	HTEST_RUN_METHOD(Array, cast)
	HTEST_RUN_METHOD(Array, incorrectNegativeIndex)
	HTEST_RUN_METHOD(Array, correctNegativeIndex)
	HTEST_RUN_METHOD(Array, positiveIndex)
	HTEST_RUN_METHOD(Array, outOfBoundsAssignment)
HTEST_RUN_CLASS_END
