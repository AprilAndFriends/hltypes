/// @file
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hdir.h>
#include <hltypes/hfile.h>
#include <hltypes/hstring.h>

TEST(Dir_static_create_remove)
{
	hstr dirname = "testdir";
	hdir::create(dirname);
	CHECK(hdir::exists(dirname));
	hdir::remove(dirname);
	hstr dirname2 = dirname + "/testdir2";
	hdir::create(dirname2);
	CHECK(hdir::exists(dirname2));
	hfile::create(dirname2 + "/test.txt");
	hdir::remove(dirname);
	CHECK(!hdir::exists(dirname2));
	CHECK(!hdir::exists(dirname));
}

TEST(Dir_static_entries)
{
	hstr dirname = "testdir";
	hdir::create(dirname + "/dir1");
	hdir::create(dirname + "/dir2");
	hfile::create(dirname + "/file1.txt");
	hfile::create(dirname + "/file2.txt");
	harray<hstr> data = hdir::entries(dirname);
	CHECK(data.size() == 6);
	CHECK(data.contains("."));
	CHECK(data.contains(".."));
	CHECK(data.contains("dir1"));
	CHECK(data.contains("dir2"));
	CHECK(data.contains("file1.txt"));
	CHECK(data.contains("file2.txt"));
	data = hdir::entries(dirname, true);
	CHECK(data.size() == 6);
	CHECK(data.contains(dirname + "/."));
	CHECK(data.contains(dirname + "/.."));
	CHECK(data.contains(dirname + "/dir1"));
	CHECK(data.contains(dirname + "/dir2"));
	CHECK(data.contains(dirname + "/file1.txt"));
	CHECK(data.contains(dirname + "/file2.txt"));
	data = hdir::contents(dirname);
	CHECK(data.size() == 4);
	CHECK(data.contains("dir1"));
	CHECK(data.contains("dir2"));
	CHECK(data.contains("file1.txt"));
	CHECK(data.contains("file2.txt"));
	data = hdir::contents(dirname, true);
	CHECK(data.size() == 4);
	CHECK(data.contains(dirname + "/dir1"));
	CHECK(data.contains(dirname + "/dir2"));
	CHECK(data.contains(dirname + "/file1.txt"));
	CHECK(data.contains(dirname + "/file2.txt"));
	data = hdir::directories(dirname);
	CHECK(data.size() == 2);
	CHECK(data.contains("dir1"));
	CHECK(data.contains("dir2"));
	data = hdir::directories(dirname, true);
	CHECK(data.size() == 2);
	CHECK(data.contains(dirname + "/dir1"));
	CHECK(data.contains(dirname + "/dir2"));
	data = hdir::files(dirname, true);
	CHECK(data.size() == 2);
	CHECK(data.contains(dirname + "/file1.txt"));
	CHECK(data.contains(dirname + "/file2.txt"));
	data = hdir::files(dirname);
	CHECK(data.size() == 2);
	CHECK(data.contains("file1.txt"));
	CHECK(data.contains("file2.txt"));
}

TEST(Dir_static_clear)
{
	hstr dirname = "testdir";
	hstr dirname2 = dirname + "/testdir2";
	hfile::create(dirname2 + "/test.txt");
	CHECK(hdir::exists(dirname2));
	hdir::clear(dirname);
	CHECK(!hdir::exists(dirname2));
	CHECK(hdir::exists(dirname));
	hdir::remove(dirname);
}

TEST(Dir_static_rename)
{
	hstr dirname = "testdir";
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2);
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	CHECK(hdir::exists(dirname + "/" + dirname2));
	hstr newdir = "testdirX";
	hdir::rename(dirname, newdir);
	CHECK(!hdir::exists(dirname));
	CHECK(!hdir::exists(dirname + "/" + dirname2));
	CHECK(hdir::exists(newdir));
	CHECK(hdir::exists(newdir + "/" + dirname2));
	hdir::remove(newdir);
}

TEST(Dir_static_move)
{
	hstr dirname = "testdir";
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2);
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	CHECK(hdir::exists(dirname + "/" + dirname2));
	hstr newdir = "..";
	hdir::remove(newdir + "/" + dirname);
	hdir::move(dirname, newdir + "/");
	CHECK(!hdir::exists(dirname));
	CHECK(!hdir::exists(dirname + "/" + dirname2));
	CHECK(hdir::exists(newdir + "/" + dirname));
	CHECK(hdir::exists(newdir + "/" + dirname + "/" + dirname2));
	hdir::remove(newdir + "/" + dirname);
}

TEST(Dir_static_copy)
{
	hstr dirname = "testdir";
	hstr dirname2 = "testdir2/testdir3";
	hdir::create(dirname + "/" + dirname2 + "/");
	hfile::create(dirname + "/testdir2/test.txt");
	hfile::create(dirname + "/" + dirname2 + "/test.txt");
	CHECK(hdir::exists(dirname + "/" + dirname2));
	hstr newdir = "dir";
	hdir::remove(newdir + "/" + dirname + "/");
	hdir::copy(dirname, newdir + "/" + dirname);
	CHECK(hdir::exists(dirname));
	CHECK(hdir::exists(dirname + "/" + dirname2));
	CHECK(hdir::exists(newdir + "/" + dirname));
	CHECK(hdir::exists(newdir + "/" + dirname + "/" + dirname2));
	hdir::remove(dirname);
	hdir::remove(newdir + "/" + dirname);
}

