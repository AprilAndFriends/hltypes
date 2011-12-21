/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>
#ifdef _WIN32
#include "msvc_dirent.h"
#include <direct.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define _mkdir(name) mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define _rmdir(name) rmdir(name)
#define _chdir(name) chdir(name)
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
	
	static hstr convert_to_native_path(hstr path)
	{
#ifndef NO_FS_TREE
		return path;
#else
		hstr npath = path.ltrim('.').ltrim('.').ltrim('/');
		npath = npath.replace("/","___");
		return npath;
#endif
	}

	static bool hmkdir(chstr path)
	{
		/*
		wchar_t* wpath = utf8_to_unicode(path);
		bool result =_wmkdir(wpath)
		delete wpath;
		return result;

		*/
#ifndef NO_FS_TREE
		return (_mkdir(path.c_str()) != 0);
#else
		return true;
#endif
	}
	
	static bool hremove(chstr dirname)
	{
		/*
		wchar_t* wdirname = utf8_to_unicode(dirname);
		bool result =_wremove(wdirname)
		delete wdirname;
		return result;
		*/
#ifndef NO_FS_TREE
		return (remove(dirname.c_str()) != 0);
#else
		return true;
#endif
	}
	
	static bool hrmdir(chstr dirname)
	{
		/*
		wchar_t* wdirname = utf8_to_unicode(dirname);
		bool result = _wrmdir(wdirname);
		return result;
		*/
#ifndef NO_FS_TREE
		return (_rmdir(dirname.c_str()) != 0);
#else
		return true;
#endif
	}
	
	bool Dir::create(chstr dirname)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		if (name == "" || hdir::exists(name))
		{
			return false;
		}
		Array<hstr> folders = name.split("/", -1, false);
		if (folders.size() > 0)
		{
			hstr path = folders.pop_first();
			hmkdir(path);
			foreach (hstr, it, folders)
			{
				path += "/" + (*it);
				hmkdir(path);
			}
		}
		return hdir::exists(dirname);
#else
		return true;
#endif
	}
	
	bool Dir::create_new(chstr dirname)
	{
#ifndef NO_FS_TREE
		return (hdir::create(dirname) || hdir::clear(dirname));
#else
		return true;
#endif
	}
	
	bool Dir::remove(chstr dirname)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		if (name == "" || !hdir::exists(name))
		{
			return false;
		}
		Array<hstr> directories = hdir::directories(name);
		foreach (hstr, it, directories)
		{
			hdir::remove(name + "/" + (*it));
		}
		Array<hstr> files = hdir::files(name);
		foreach (hstr, it, files)
		{
			hfile::remove(name + "/" + (*it));
		}
		hrmdir(name);
		return hdir::exists(name);
#else
		// TODO delete all files starting with dirname where all '/' are replaced by '___'
		return true;
#endif
	}
	
	bool Dir::exists(chstr dirname)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		DIR* dir = opendir(name.c_str());
		if (dir != NULL)
		{
			closedir(dir);
			return true;
		}
		return false;
#else
		// TODO return true if there are any files starting with dirname with replaced '/' for '___'
		return false;
#endif
	}
	
	bool Dir::clear(chstr dirname)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		if (name == "" || !hdir::exists(name))
		{
			return false;
		}
		Array<hstr> directories = hdir::directories(name);
		foreach (hstr, it, directories)
		{
			hdir::remove(name + "/" + (*it));
		}
		Array<hstr> files = hdir::files(name);
		foreach (hstr, it, files)
		{
			hfile::remove(name + "/" + (*it));
		}
		return (directories.size() > 0 || files.size() > 0);
#else
		// TODO
		return true;
#endif
	}
	
	bool Dir::rename(chstr old_dirname, chstr new_dirname)
	{
#ifndef NO_FS_TREE
		hstr old_name = normalize_path(old_dirname);
		hstr new_name = normalize_path(new_dirname);
		if (!hdir::exists(old_name) || hdir::exists(new_name))
		{
			return false;
		}
		hdir::create_path(new_name);
		return (d_rename(old_name.c_str(), new_name.c_str()) == 0);
#else
		// TODO rename all files that have old_dirname in them with new_dirname
		return true;
#endif
	}
	
	bool Dir::move(chstr dirname, chstr path)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		hstr path_name = normalize_path(path);
		return hdir::rename(name, path_name + "/" + name.rsplit("/", 1, false).pop_last());
#else
		// TODO
		return true;
#endif
	}
	
	bool Dir::copy(chstr old_dirname, chstr new_dirname)
	{
#ifndef NO_FS_TREE
		hstr old_name = normalize_path(old_dirname);
		hstr new_name = normalize_path(new_dirname);
		if (!hdir::exists(old_name) || hdir::exists(new_name))
		{
			return false;
		}
		hdir::create(new_name);
		Array<hstr> directories = hdir::directories(old_name);
		foreach (hstr, it, directories)
		{
			hdir::copy(old_name + "/" + (*it), new_name + "/" + (*it));
		}
		Array<hstr> files = hdir::files(old_name);
		foreach (hstr, it, files)
		{
			hfile::copy(old_name + "/" + (*it), new_name + "/" + (*it));
		}
		return true;
#else
		return true;
#endif
	}
	
	bool Dir::create_path(chstr path)
	{
#ifndef NO_FS_TREE
		hstr dir = get_basedir(path);
		return (dir != "." && hdir::create(dir));
#else
		return true;
#endif
	}
    
    void prepend_directory(chstr dirname, Array<hstr>& entries)
    {
		if (dirname != "")
		{
			foreach (hstr, it, entries)
			{
				(*it) = dirname + "/" + (*it);
			}
		}
    }
	
	Array<hstr> Dir::entries(chstr dirname, bool prepend_dir)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		Array<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = opendir(name.c_str());
			struct dirent* entry;
			while ((entry = readdir(dir)))
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
#else
		//2do
		Array<hstr> result;
		return result;
#endif
	}
	
	Array<hstr> Dir::contents(chstr dirname, bool prepend_dir)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		Array<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = opendir(name.c_str());
			struct dirent* entry;
			while ((entry = readdir(dir)))
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
#else
		//2do
		Array<hstr> result;
		return result;
#endif
	}
	
	Array<hstr> Dir::directories(chstr dirname, bool prepend_dir)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		hstr current;
		Array<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = opendir(name.c_str());
			struct dirent* entry;
			while ((entry = readdir(dir)))
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
#else
		//2do
		Array<hstr> result;
		return result;
#endif
	}
	
	Array<hstr> Dir::files(chstr dirname, bool prepend_dir)
	{
#ifndef NO_FS_TREE
		hstr name = normalize_path(dirname);
		hstr current;
		Array<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = opendir(name.c_str());
			struct dirent* entry;
			while ((entry = readdir(dir)))
			{
				current = name + "/" + entry->d_name;
				if (hfile::exists(current))
				{
					result += entry->d_name;
				}
			}
			if (result.contains("."))
			{
				result.remove(".");
			}
			if (result.contains(".."))
			{
				result.remove("..");
			}
			closedir(dir);
		}
#else
		Array<hstr> result;
		DIR *dir = opendir("./");
		struct dirent *entry;
		hstr tmp;
		hstr name = normalize_path(dirname);
		name = name.replace("/","___");
		while ((entry = readdir(dir)))
		{
			tmp = hstr(entry->d_name);
			if (tmp.starts_with(name))
			{
				printf("%s\n", entry->d_name);
				tmp = tmp.split("___")[-1];
				result += tmp;
			}
		}
		name = name.replace("___", "/");
		closedir(dir);
#endif
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	void hdir::chdir(chstr dirname)
	{
		_chdir(dirname.c_str());
	}
}
