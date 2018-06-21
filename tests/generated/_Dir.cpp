#define __HTEST_LIB_NAME hltypes
#include "htest.h"

HTEST_CLASS(Dir)
{
	HTEST_FUNCTION(staticCreateRemove);
	HTEST_FUNCTION(staticClear);
	HTEST_FUNCTION(staticRename);
	HTEST_FUNCTION(staticMove);
	HTEST_FUNCTION(staticCopy);
}

HTEST_RUN_CLASS_BEGIN(Dir)
	HTEST_RUN_METHOD(Dir, staticCreateRemove)
	HTEST_RUN_METHOD(Dir, staticClear)
	HTEST_RUN_METHOD(Dir, staticRename)
	HTEST_RUN_METHOD(Dir, staticMove)
	HTEST_RUN_METHOD(Dir, staticCopy)
HTEST_RUN_CLASS_END
