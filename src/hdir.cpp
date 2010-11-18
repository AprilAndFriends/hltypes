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
#ifdef _WIN32
#include "msvc_dirent.h"
#include <direct.h>
#else
#include <dirent.h>
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
#include "util.h"

namespace hltypes
{
/******* STATIC ********************************************************/
	
	static bool hmkdir(chstr path)
	{
		/*
		wchar_t wpath[1025] = {'\0'};
		utf8_to_unicode(path, wpath);
		_wmkdir(wpath);
		*/
		_mkdir(path.c_str());
	}
	
	static bool hremove(chstr dirname)
	{
		/*
		wchar_t wdirname[1025] = {'\0'};
		utf8_to_unicode(dirname, wdirname);
		_wremove(wdirname);
		*/
		remove(dirname.c_str());
	}
	
	static bool hrmdir(chstr dirname)
	{
		/*
		wchar_t wdirname[1025] = {'\0'};
		utf8_to_unicode(dirname, wdirname);
		_wrmdir(wdirname);
		*/
		_rmdir(dirname.c_str());
	}
	
	bool dir::create(chstr dirname)
	{
		hstr name = normalize_path(dirname);
		if (name == "" || hdir::exists(name))
		{
			return false;
		}
		harray<hstr> folders = name.split("/");
		if (folders.size() > 0)
		{
			hstr path = folders.pop_front();
			hmkdir(path);
			foreach (hstr, it, folders)
			{
				path += "/" + (*it);
				hmkdir(path);
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
		hstr name = normalize_path(dirname);
		if (name == "" || !dir::exists(name))
		{
			return false;
		}
		harray<hstr> directories = hdir::directories(name);
		foreach (hstr, it, directories)
		{
			hdir::remove(name + "/" + (*it));
		}
		harray<hstr> files = hdir::files(name);
		foreach (hstr, it, files)
		{
			hfile::remove(name + "/" + (*it));
		}
		hrmdir(name);
		return !dir::exists(name);
	}
	
	bool dir::exists(chstr dirname)
	{
		hstr name = normalize_path(dirname);
		DIR* dir = opendir(name.c_str());
		if (dir != NULL)
		{
			closedir(dir);
			return true;
		}
		return false;
	}
	
	bool dir::clear(chstr dirname)
	{
		hstr name = normalize_path(dirname);
		if (name == "" || !dir::exists(name))
		{
			return false;
		}
		harray<hstr> directories = hdir::directories(name);
		foreach (hstr, it, directories)
		{
			hdir::remove(name + "/" + (*it));
		}
		harray<hstr> files = hdir::files(name);
		foreach (hstr, it, files)
		{
			hfile::remove(name + "/" + (*it));
		}
		return (directories.size() > 0 || files.size() > 0);
	}
	
	bool dir::rename(chstr old_dirname, chstr new_dirname)
	{
		hstr old_name = normalize_path(old_dirname);
		hstr new_name = normalize_path(new_dirname);
		if (!hdir::exists(old_name) || hdir::exists(new_name))
		{
			return false;
		}
		hdir::create_path(new_name);
		return (d_rename(old_name.c_str(), new_name.c_str()) == 0);
	}
	
	bool dir::move(chstr dirname, chstr path)
	{
		hstr name = normalize_path(dirname);
		hstr path_name = normalize_path(path);
		return hdir::rename(name, path_name + "/" + name.rsplit("/", 1).pop_back());
	}
	
	bool dir::copy(chstr old_dirname, chstr new_dirname)
	{
		hstr old_name = normalize_path(old_dirname);
		hstr new_name = normalize_path(new_dirname);
		if (!hdir::exists(old_name) || hdir::exists(new_name))
		{
			return false;
		}
		hdir::create(new_name);
		harray<hstr> directories = hdir::directories(old_name);
		foreach (hstr, it, directories)
		{
			hdir::copy(old_name + "/" + (*it), new_name + "/" + (*it));
		}
		harray<hstr> files = hdir::files(old_name);
		foreach (hstr, it, files)
		{
			hfile::copy(old_name + "/" + (*it), new_name + "/" + (*it));
		}
		return true;
	}
	
	bool dir::create_path(chstr path)
	{
		hstr path_name = normalize_path(path);
		harray<hstr> parts = path_name.rsplit("/", 1);
		return (parts.size() > 1 && hdir::create(parts[0]));
	}
    
    void prepend_directory(chstr dirname, harray<hstr>& entries)
    {
		if (dirname != "")
		{
			foreach (hstr, it, entries)
			{
				(*it) = dirname + "/" + (*it);
			}
		}
    }
	
	harray<hstr> dir::entries(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		harray<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = opendir(name.c_str());
			struct dirent* entry;
			while (entry = readdir(dir))
			{
				result += entry->d_name;
			}
			closedir(dir);
		}
        if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	harray<hstr> dir::contents(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		harray<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = opendir(name.c_str());
			struct dirent* entry;
			while (entry = readdir(dir))
			{
				result += entry->d_name;
			}
			result.remove(".");
			result.remove("..");
			closedir(dir);
		}
        if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	harray<hstr> dir::directories(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		hstr current;
		harray<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = opendir(name.c_str());
			struct dirent* entry;
			while (entry = readdir(dir))
			{
				current = name + "/" + entry->d_name;
				if (hdir::exists(current))
				{
					result += entry->d_name;
				}
			}
			result.remove(".");
			result.remove("..");
			closedir(dir);
		}
        if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	harray<hstr> dir::files(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		hstr current;
		harray<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = opendir(name.c_str());
			struct dirent* entry;
			while (entry = readdir(dir))
			{
				current = name + "/" + entry->d_name;
				if (hfile::exists(current))
				{
					result += entry->d_name;
				}
			}
			closedir(dir);
		}
        if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
}
