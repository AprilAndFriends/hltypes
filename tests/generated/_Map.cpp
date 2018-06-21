#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(Map)
{
	HTEST_FUNCTION(adding);
	HTEST_FUNCTION(removing);
	HTEST_FUNCTION(structure);
	HTEST_FUNCTION(iteration);
	HTEST_FUNCTION(comparison);
	HTEST_FUNCTION(random);
	HTEST_FUNCTION(match);
	HTEST_FUNCTION(cast);
}

HTEST_RUN_CLASS_BEGIN(Map)
	HTEST_RUN_METHOD(Map, adding)
	HTEST_RUN_METHOD(Map, removing)
	HTEST_RUN_METHOD(Map, structure)
	HTEST_RUN_METHOD(Map, iteration)
	HTEST_RUN_METHOD(Map, comparison)
	HTEST_RUN_METHOD(Map, random)
	HTEST_RUN_METHOD(Map, match)
	HTEST_RUN_METHOD(Map, cast)
HTEST_RUN_CLASS_END
