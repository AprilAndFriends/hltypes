#define _HTEST_LIB hltypes
#define _HTEST_CLASS Dir

#include <htest/htest.h>

#include "harray.h"
#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hstring.h"

#include "hlog.h"

HTEST_SUITE_BEGIN

HTEST_CASE_WITH_DATA(staticCreateRemove)
{
	hstr dirname = tempDir + "/testdir";
	hdir::create(dirname);
	HTEST_ASSERT(hdir::exists(dirname), "create 1");
	hdir::remove(dirname);
	hstr dirname2 = dirname + "/testdir2";
	hdir::create(dirname2);
	HTEST_ASSERT(hdir::exists(dirname2), "create 2");
	hfile::create(dirname2 + "/test.txt");
	hdir::remove(dirname);
	HTEST_ASSERT(!hdir::exists(dirname2), "remove 2");
	HTEST_ASSERT(!hdir::exists(dirname), "remove 1");
}

HTEST_CASE_WITH_DATA(staticClear)
{
	hstr dirname = tempDir + "/testdir";
	hstr dirname2 = dirname + "/testdir2";
	hfile::create(dirname2 + "/test.txt");
	HTEST_ASSERT(hdir::exists(dirname2), "create");
	hdir::clear(dirname);
	HTEST_ASSERT(!hdir::exists(dirname2), "clear 1");
	HTEST_ASSERT(hdir::exists(dirname), "clear 2");
	hdir::remove(dirname);
}

HTEST_CASE_WITH_DATA(staticRename)
{
	hstr dirname = tempDir + "/testdir";
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2);
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	HTEST_ASSERT(hdir::exists(dirname + "/" + dirname2), "create");
	hstr newdir = tempDir + "/testdirX";
	hdir::rename(dirname, newdir);
	HTEST_ASSERT(!hdir::exists(dirname), "exists 1");
	HTEST_ASSERT(!hdir::exists(dirname + "/" + dirname2), "exists 2");
	HTEST_ASSERT(hdir::exists(newdir), "exists 3");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirname2), "exists 4");
	hdir::remove(newdir);
}

HTEST_CASE_WITH_DATA(staticMove)
{
	hstr dirTemplate = "testdir";
	hstr dirname = tempDir + "/" + dirTemplate;
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2);
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	HTEST_ASSERT(hdir::exists(dirname + "/" + dirname2), "create");
	hstr newdir = tempDir + "/..";
	hdir::remove(newdir + "/" + dirTemplate);
	hdir::move(dirname, newdir + "/");
	HTEST_ASSERT(!hdir::exists(dirname), "exists 1");
	HTEST_ASSERT(!hdir::exists(dirname + "/" + dirname2), "exists 2");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirTemplate), "exists 3");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirTemplate + "/" + dirname2), "exists 4");
	hdir::remove(newdir + "/" + dirTemplate);
}

HTEST_CASE_WITH_DATA(staticCopy)
{
	hstr dirTemplate = "testdir";
	hstr dirname = tempDir + "/" + dirTemplate;
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2 + "/");
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	HTEST_ASSERT(hdir::exists(dirname + "/" + dirname2), "create");
	hstr newdir = tempDir + "/dir";
	hdir::remove(newdir + "/" + dirTemplate + "/");
	hdir::copy(dirname, newdir + "/" + dirTemplate);
	HTEST_ASSERT(hdir::exists(dirname), "exists 1");
	HTEST_ASSERT(hdir::exists(dirname + "/" + dirname2), "exists 2");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirTemplate), "exists 3");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirTemplate + "/" + dirname2), "exists 4");
	hdir::remove(dirname);
	hdir::remove(newdir + "/" + dirTemplate);
}

HTEST_SUITE_END
