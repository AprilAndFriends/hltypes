/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <stdio.h>
#include <dirent.h>
#ifdef _WIN32
#include <direct.h>
#elif defined(__APPLE__)
#include <sys/stat.h>
#define _mkdir(name) mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define _rmdir(name) rmdir(name)
#endif

// prevents recursive calls of hdir::rename as this function is called via this pointer
int (*d_rename)(const char* old_name, const char* new_name) = rename;

#include "harray.h"
#include "hdir.h"
#include "hfile.h"
#include "hstring.h"

namespace hltypes
{
/******* STATIC ********************************************************/
	
	bool dir::create(chstr dirname)
	{
		if (dirname == "" || hdir::exists(dirname))
		{
			return false;
		}
		harray<hstr> folders = dirname.split("/");
		if (folders.size() > 0)
		{
			hstr path = folders.pop_front();
			_mkdir(path.c_str());
			foreach (hstr, it, folders)
			{
				path += "/" + (*it);
				_mkdir(path.c_str());
			}
		}
		return hdir::exists(dirname);
	}
	
	bool dir::create_new(chstr dirname)
	{
		return (hdir::create(dirname) || hdir::clear(dirname));
	}
	
	bool dir::remove(chstr dirname)
	{
		if (dirname == "" || !dir::exists(dirname))
		{
			return false;
		}
		harray<hstr> directories = hdir::directories(dirname);
		foreach (hstr, it, directories)
		{
			hdir::remove(dirname + "/" + (*it));
		}
		harray<hstr> files = hdir::files(dirname);
		foreach (hstr, it, files)
		{
			hfile::remove(dirname + "/" + (*it));
		}
		_rmdir(dirname.c_str());
		return !dir::exists(dirname);
	}
	
	bool dir::exists(chstr dirname)
	{
		DIR* dir = opendir(dirname.c_str());
		if (dir != NULL)
		{
			closedir(dir);
			return true;
		}
		return false;
	}
	
	bool dir::clear(chstr dirname)
	{
		if (dirname == "" || !dir::exists(dirname))
		{
			return false;
		}
		harray<hstr> directories = hdir::directories(dirname);
		foreach (hstr, it, directories)
		{
			hdir::remove(dirname + "/" + (*it));
		}
		harray<hstr> files = hdir::files(dirname);
		foreach (hstr, it, files)
		{
			hfile::remove(dirname + "/" + (*it));
		}
		return (directories.size() > 0 || files.size() > 0);
	}
	
	bool dir::rename(chstr old_dirname, chstr new_dirname)
	{
		if (!hdir::exists(old_dirname) || hdir::exists(new_dirname))
		{
			return false;
		}
		hdir::create_path(new_dirname);
		return (d_rename(old_dirname.c_str(), new_dirname.c_str()) == 0);
	}
	
	bool dir::move(chstr dirname, chstr path)
	{
		return hdir::rename(dirname, path + "/" + dirname.rsplit("/", 1).pop_back());
	}
	
	bool dir::copy(chstr old_dirname, chstr new_dirname)
	{
		if (!hdir::exists(old_dirname) || hdir::exists(new_dirname))
		{
			return false;
		}
		hdir::create(new_dirname);
		harray<hstr> directories = hdir::directories(old_dirname);
		foreach (hstr, it, directories)
		{
			hdir::copy(old_dirname + "/" + (*it), new_dirname + "/" + (*it));
		}
		harray<hstr> files = hdir::files(old_dirname);
		foreach (hstr, it, files)
		{
			hfile::copy(old_dirname + "/" + (*it), new_dirname + "/" + (*it));
		}
		return true;
	}
	
	bool dir::create_path(chstr path)
	{
		harray<hstr> parts = path.rsplit("/", 1);
		return (parts.size() > 1 && hdir::create(parts[0]));
	}
    
    void prepend_directory(chstr dirname, harray<hstr>& entries)
    {
        foreach(hstr, it, entries)
		{
			(*it) = dirname + "/" + (*it);
		}
    }
	
	harray<hstr> dir::entries(chstr dirname, bool prepend_dir)
	{
		harray<hstr> result;
		if (hdir::exists(dirname))
		{
			DIR* dir = opendir(dirname.c_str());
			struct dirent* entry;
			while (entry = readdir(dir))
			{
				result += entry->d_name;
			}
			closedir(dir);
		}
        if (prepend_dir)
		{
			prepend_directory(dirname, result);
		}
		return result;
	}
	
	harray<hstr> dir::contents(chstr dirname, bool prepend_dir)
	{
		harray<hstr> result = dir::entries(dirname);
		if (result.size() > 0)
		{
			result.remove(".");
			result.remove("..");
			if (prepend_dir)
			{
				prepend_directory(dirname, result);
			}
		}
		return result;
	}
	
	harray<hstr> dir::directories(chstr dirname, bool prepend_dir)
	{
		harray<hstr> contents = dir::contents(dirname);
		harray<hstr> result;
		foreach (hstr, it, contents)
		{
			if (hdir::exists(dirname + "/" + (*it)))
			{
				result += (*it);
			}
		}
        if (prepend_dir)
		{
			prepend_directory(dirname, result);
		}
		return result;
	}
	
	harray<hstr> dir::files(chstr dirname, bool prepend_dir)
	{
		harray<hstr> contents = dir::contents(dirname);
		harray<hstr> result;
		foreach (hstr, it, contents)
		{
			if (!hdir::exists(dirname + "/" + (*it)))
			{
				result += (*it);
			}
		}
        if (prepend_dir)
		{
			prepend_directory(dirname, result);
		}
		return result;
	}
	
}
