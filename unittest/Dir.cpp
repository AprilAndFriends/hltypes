#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "harray.h"
#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hstring.h"

HL_UT_TEST_CLASS(Dir)
{
	HL_UT_TEST_FUNCTION(staticCreateRemove)
	{
		hstr dirname = "testdir";
		hdir::create(dirname);
		HL_UT_ASSERT(hdir::exists(dirname), "hdir - static create/remove");
		hdir::remove(dirname);
		hstr dirname2 = dirname + "/testdir2";
		hdir::create(dirname2);
		HL_UT_ASSERT(hdir::exists(dirname2), "hdir - static create/remove");
		hfile::create(dirname2 + "/test.txt");
		hdir::remove(dirname);
		HL_UT_ASSERT(!hdir::exists(dirname2), "hdir - static create/remove");
		HL_UT_ASSERT(!hdir::exists(dirname), "hdir - static create/remove");
	}

	HL_UT_TEST_FUNCTION(staticClear)
	{
		hstr dirname = "testdir";
		hstr dirname2 = dirname + "/testdir2";
		hfile::create(dirname2 + "/test.txt");
		HL_UT_ASSERT(hdir::exists(dirname2), "hdir - static clear");
		hdir::clear(dirname);
		HL_UT_ASSERT(!hdir::exists(dirname2), "hdir - static clear");
		HL_UT_ASSERT(hdir::exists(dirname), "hdir - static clear");
		hdir::remove(dirname);
	}

	HL_UT_TEST_FUNCTION(staticRename)
	{
		hstr dirname = "testdir";
		hstr dirname2 = "testdir2/testdir3";
		hdir::create(dirname + "/" + dirname2);
		hfile::create(dirname + "/testdir2/test.txt");
		hfile::create(dirname + "/" + dirname2 + "/test.txt");
		HL_UT_ASSERT(hdir::exists(dirname + "/" + dirname2), "hdir - static rename");
		hstr newdir = "testdirX";
		hdir::rename(dirname, newdir);
		HL_UT_ASSERT(!hdir::exists(dirname), "hdir - static rename");
		HL_UT_ASSERT(!hdir::exists(dirname + "/" + dirname2), "hdir - static rename");
		HL_UT_ASSERT(hdir::exists(newdir), "hdir - static rename");
		HL_UT_ASSERT(hdir::exists(newdir + "/" + dirname2), "hdir - static rename");
		hdir::remove(newdir);
	}

	HL_UT_TEST_FUNCTION(staticMove)
	{
		hstr dirname = "testdir";
		hstr dirname2 = "testdir2/testdir3";
		hdir::create(dirname + "/" + dirname2);
		hfile::create(dirname + "/testdir2/test.txt");
		hfile::create(dirname + "/" + dirname2 + "/test.txt");
		HL_UT_ASSERT(hdir::exists(dirname + "/" + dirname2), "hdir - static move");
		hstr newdir = "..";
		hdir::remove(newdir + "/" + dirname);
		hdir::move(dirname, newdir + "/");
		HL_UT_ASSERT(!hdir::exists(dirname), "hdir - static move");
		HL_UT_ASSERT(!hdir::exists(dirname + "/" + dirname2), "hdir - static move");
		HL_UT_ASSERT(hdir::exists(newdir + "/" + dirname), "hdir - static move");
		HL_UT_ASSERT(hdir::exists(newdir + "/" + dirname + "/" + dirname2), "hdir - static move");
		hdir::remove(newdir + "/" + dirname);
	}

	HL_UT_TEST_FUNCTION(staticCopy)
	{
		hstr dirname = "testdir";
		hstr dirname2 = "testdir2/testdir3";
		hdir::create(dirname + "/" + dirname2 + "/");
		hfile::create(dirname + "/testdir2/test.txt");
		hfile::create(dirname + "/" + dirname2 + "/test.txt");
		HL_UT_ASSERT(hdir::exists(dirname + "/" + dirname2), "hdir - static copy");
		hstr newdir = "dir";
		hdir::remove(newdir + "/" + dirname + "/");
		hdir::copy(dirname, newdir + "/" + dirname);
		HL_UT_ASSERT(hdir::exists(dirname), "hdir - static copy");
		HL_UT_ASSERT(hdir::exists(dirname + "/" + dirname2), "hdir - static copy");
		HL_UT_ASSERT(hdir::exists(newdir + "/" + dirname), "hdir - static copy");
		HL_UT_ASSERT(hdir::exists(newdir + "/" + dirname + "/" + dirname2), "hdir - static copy");
		hdir::remove(dirname);
		hdir::remove(newdir + "/" + dirname);
	}
}
