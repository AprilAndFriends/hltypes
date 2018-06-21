#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(List)
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
	HTEST_FUNCTION(join);
	HTEST_FUNCTION(matching);
	HTEST_FUNCTION(random);
	HTEST_FUNCTION(cast);
	HTEST_FUNCTION(incorrectNegativeIndex);
	HTEST_FUNCTION(correctNegativeIndex);
	HTEST_FUNCTION(positiveIndex);
	HTEST_FUNCTION(outOfBoundsAssignment);
}

HTEST_RUN_CLASS_BEGIN(List)
	HTEST_RUN_METHOD(List, adding)
	HTEST_RUN_METHOD(List, adding2)
	HTEST_RUN_METHOD(List, adding3)
	HTEST_RUN_METHOD(List, removing1)
	HTEST_RUN_METHOD(List, removing2)
	HTEST_RUN_METHOD(List, container)
	HTEST_RUN_METHOD(List, comparison)
	HTEST_RUN_METHOD(List, operations)
	HTEST_RUN_METHOD(List, iteration)
	HTEST_RUN_METHOD(List, constructor)
	HTEST_RUN_METHOD(List, intersection)
	HTEST_RUN_METHOD(List, union_)
	HTEST_RUN_METHOD(List, difference)
	HTEST_RUN_METHOD(List, join)
	HTEST_RUN_METHOD(List, matching)
	HTEST_RUN_METHOD(List, random)
	HTEST_RUN_METHOD(List, cast)
	HTEST_RUN_METHOD(List, incorrectNegativeIndex)
	HTEST_RUN_METHOD(List, correctNegativeIndex)
	HTEST_RUN_METHOD(List, positiveIndex)
	HTEST_RUN_METHOD(List, outOfBoundsAssignment)
HTEST_RUN_CLASS_END
