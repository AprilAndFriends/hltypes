#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(Deque)
{
	HTEST_FUNCTION(adding);
	HTEST_FUNCTION(removing);
	HTEST_FUNCTION(size);
}

HTEST_RUN_CLASS_BEGIN(Deque)
	HTEST_RUN_METHOD(Deque, adding)
	HTEST_RUN_METHOD(Deque, removing)
	HTEST_RUN_METHOD(Deque, size)
HTEST_RUN_CLASS_END
