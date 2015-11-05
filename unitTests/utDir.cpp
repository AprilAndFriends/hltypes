#include "generated/aprilUT.h"
#include "harray.h"
#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hstring.h"

namespace testDir
{
	void testStaticCreateRemove()
	{
		hstr dirname = "testdir";
		hdir::create(dirname);
		assertTrue(hdir::exists(dirname), "hdir - static create/remove");
		hdir::remove(dirname);
		hstr dirname2 = dirname + "/testdir2";
		hdir::create(dirname2);
		assertTrue(hdir::exists(dirname2), "hdir - static create/remove");
		hfile::create(dirname2 + "/test.txt");
		hdir::remove(dirname);
		assertTrue(!hdir::exists(dirname2), "hdir - static create/remove");
		assertTrue(!hdir::exists(dirname), "hdir - static create/remove");
	}

	void testStaticClear()
	{
		hstr dirname = "testdir";
		hstr dirname2 = dirname + "/testdir2";
		hfile::create(dirname2 + "/test.txt");
		assertTrue(hdir::exists(dirname2), "hdir - static clear");
		hdir::clear(dirname);
		assertTrue(!hdir::exists(dirname2), "hdir - static clear");
		assertTrue(hdir::exists(dirname), "hdir - static clear");
		hdir::remove(dirname);
	}

	void testStaticRename()
	{
		hstr dirname = "testdir";
		hstr dirname2 = "testdir2/testdir3";
		hdir::create(dirname + "/" + dirname2);
		hfile::create(dirname + "/testdir2/test.txt");
		hfile::create(dirname + "/" + dirname2 + "/test.txt");
		assertTrue(hdir::exists(dirname + "/" + dirname2), "hdir - static rename");
		hstr newdir = "testdirX";
		hdir::rename(dirname, newdir);
		assertTrue(!hdir::exists(dirname), "hdir - static rename");
		assertTrue(!hdir::exists(dirname + "/" + dirname2), "hdir - static rename");
		assertTrue(hdir::exists(newdir), "hdir - static rename");
		assertTrue(hdir::exists(newdir + "/" + dirname2), "hdir - static rename");
		hdir::remove(newdir);
	}

	void testStaticMove()
	{
		hstr dirname = "testdir";
		hstr dirname2 = "testdir2/testdir3";
		hdir::create(dirname + "/" + dirname2);
		hfile::create(dirname + "/testdir2/test.txt");
		hfile::create(dirname + "/" + dirname2 + "/test.txt");
		assertTrue(hdir::exists(dirname + "/" + dirname2), "hdir - static move");
		hstr newdir = "..";
		hdir::remove(newdir + "/" + dirname);
		hdir::move(dirname, newdir + "/");
		assertTrue(!hdir::exists(dirname), "hdir - static move");
		assertTrue(!hdir::exists(dirname + "/" + dirname2), "hdir - static move");
		assertTrue(hdir::exists(newdir + "/" + dirname), "hdir - static move");
		assertTrue(hdir::exists(newdir + "/" + dirname + "/" + dirname2), "hdir - static move");
		hdir::remove(newdir + "/" + dirname);
	}

	void testStaticCopy()
	{
		hstr dirname = "testdir";
		hstr dirname2 = "testdir2/testdir3";
		hdir::create(dirname + "/" + dirname2 + "/");
		hfile::create(dirname + "/testdir2/test.txt");
		hfile::create(dirname + "/" + dirname2 + "/test.txt");
		assertTrue(hdir::exists(dirname + "/" + dirname2), "hdir - static copy");
		hstr newdir = "dir";
		hdir::remove(newdir + "/" + dirname + "/");
		hdir::copy(dirname, newdir + "/" + dirname);
		assertTrue(hdir::exists(dirname), "hdir - static copy");
		assertTrue(hdir::exists(dirname + "/" + dirname2), "hdir - static copy");
		assertTrue(hdir::exists(newdir + "/" + dirname), "hdir - static copy");
		assertTrue(hdir::exists(newdir + "/" + dirname + "/" + dirname2), "hdir - static copy");
		hdir::remove(dirname);
		hdir::remove(newdir + "/" + dirname);
	}
}
