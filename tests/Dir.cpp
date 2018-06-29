#define _HTEST_LIB hltypes
#define _HTEST_CLASS Dir
#include "htest.h"
#include "harray.h"
#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hstring.h"

HTEST_SUITE_BEGIN

HTEST_CASE(staticCreateRemove)
{
	hstr dirname = "testdir";
	hdir::create(dirname);
	HTEST_ASSERT(hdir::exists(dirname), "create");
	hdir::remove(dirname);
	hstr dirname2 = dirname + "/testdir2";
	hdir::create(dirname2);
	HTEST_ASSERT(hdir::exists(dirname2), "create");
	hfile::create(dirname2 + "/test.txt");
	hdir::remove(dirname);
	HTEST_ASSERT(!hdir::exists(dirname2), "remove");
	HTEST_ASSERT(!hdir::exists(dirname), "remove");
}

HTEST_CASE(staticClear)
{
	hstr dirname = "testdir";
	hstr dirname2 = dirname + "/testdir2";
	hfile::create(dirname2 + "/test.txt");
	HTEST_ASSERT(hdir::exists(dirname2), "");
	hdir::clear(dirname);
	HTEST_ASSERT(!hdir::exists(dirname2), "");
	HTEST_ASSERT(hdir::exists(dirname), "");
	hdir::remove(dirname);
}

HTEST_CASE(staticRename)
{
	hstr dirname = "testdir";
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2);
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	HTEST_ASSERT(hdir::exists(dirname + "/" + dirname2), "");
	hstr newdir = "testdirX";
	hdir::rename(dirname, newdir);
	HTEST_ASSERT(!hdir::exists(dirname), "");
	HTEST_ASSERT(!hdir::exists(dirname + "/" + dirname2), "");
	HTEST_ASSERT(hdir::exists(newdir), "");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirname2), "");
	hdir::remove(newdir);
}

HTEST_CASE(staticMove)
{
	hstr dirname = "testdir";
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2);
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	HTEST_ASSERT(hdir::exists(dirname + "/" + dirname2), "");
	hstr newdir = "..";
	hdir::remove(newdir + "/" + dirname);
	hdir::move(dirname, newdir + "/");
	HTEST_ASSERT(!hdir::exists(dirname), "");
	HTEST_ASSERT(!hdir::exists(dirname + "/" + dirname2), "");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirname), "");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirname + "/" + dirname2), "");
	hdir::remove(newdir + "/" + dirname);
}

HTEST_CASE(staticCopy)
{
	hstr dirname = "testdir";
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2 + "/");
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	HTEST_ASSERT(hdir::exists(dirname + "/" + dirname2), "");
	hstr newdir = "dir";
	hdir::remove(newdir + "/" + dirname + "/");
	hdir::copy(dirname, newdir + "/" + dirname);
	HTEST_ASSERT(hdir::exists(dirname), "");
	HTEST_ASSERT(hdir::exists(dirname + "/" + dirname2), "");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirname), "");
	HTEST_ASSERT(hdir::exists(newdir + "/" + dirname + "/" + dirname2), "");
	hdir::remove(dirname);
	hdir::remove(newdir + "/" + dirname);
}

HTEST_SUITE_END
