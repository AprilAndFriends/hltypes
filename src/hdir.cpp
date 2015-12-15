/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#include "msvc_dirent.h"
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define _chdir(name) ::chdir(name)
#define _getcwd(buffer, size) ::getcwd(buffer, size)
#define _opendir(name) opendir(name.cStr())
#define _readdir(dirp) readdir(dirp)
#define _closedir(dirp) closedir(dirp)
#endif

// prevents recursive calls of Dir::rename as this function is called via this pointer
int (*d_rename)(const char* oldName, const char* newName) = rename;

#include "harray.h"
#include "hdir.h"
#include "hfile.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"
#include "platform_internal.h"

#ifdef _WINRT
#define _chdir(name) winrtcwd = name
static hltypes::String winrtcwd = ".";
#endif

namespace hltypes
{
	bool Dir::win32FullDirectoryPermissions = true;

	bool Dir::create(const String& dirName)
	{
		String name = Dir::normalize(dirName);
		if (name == "" || Dir::exists(name))
		{
			return false;
		}
		Array<String> folders = name.split("/", -1, false);
		if (folders.size() > 0)
		{
			String path = folders.removeFirst();
			_platformCreateDirectory(path);
			foreach (String, it, folders)
			{
				path = Dir::joinPath(path, (*it), false);
				_platformCreateDirectory(path);
			}
		}
		return Dir::exists(dirName);
	}
	
	bool Dir::createNew(const String& dirName)
	{
		return (Dir::create(dirName) || Dir::clear(dirName));
	}
	
	bool Dir::remove(const String& dirName)
	{
		String name = Dir::normalize(dirName);
		if (name == "" || !Dir::exists(name))
		{
			return false;
		}
		Array<String> directories = Dir::directories(name);
		foreach (String, it, directories)
		{
			Dir::remove(Dir::joinPath(name, (*it), false));
		}
		Array<String> files = Dir::files(name);
		foreach (String, it, files)
		{
			File::remove(Dir::joinPath(name, (*it), false));
		}
		_platformRemoveDirectory(name);
		return Dir::exists(name);
	}
	
	bool Dir::exists(const String& dirName, bool caseSensitive)
	{
		String name = Dir::normalize(dirName);
		bool result = false;
		DIR* dir = _opendir(name);
		if (dir != NULL)
		{
			_closedir(dir);
			result = true;
		}
		if (!result && !caseSensitive)
		{
			String baseDir = Dir::baseDir(name);
			String baseName = Dir::baseName(name);
			Array<String> directories = Dir::directories(baseDir);
			foreach (String, it, directories)
			{
				if ((*it).lowered() == baseName.lowered())
				{
					name = Dir::joinPath(baseDir, (*it));
					result = true;
					break;
				}
			}
		}
		return result;
	}
	
	bool Dir::clear(const String& dirName)
	{
		String name = Dir::normalize(dirName);
		if (name == "" || !Dir::exists(name))
		{
			return false;
		}
		Array<String> directories = Dir::directories(name);
		foreach (String, it, directories)
		{
			Dir::remove(Dir::joinPath(name, (*it), false));
		}
		Array<String> files = Dir::files(name);
		foreach (String, it, files)
		{
			File::remove(Dir::joinPath(name, (*it), false));
		}
		return (directories.size() > 0 || files.size() > 0);
	}
	
	bool Dir::rename(const String& oldDirName, const String& newDirName)
	{
		String oldName = Dir::normalize(oldDirName);
		String newName = Dir::normalize(newDirName);
		if (!Dir::exists(oldName) || Dir::exists(newName))
		{
			return false;
		}
		Dir::create(Dir::baseDir(newName));
		return (d_rename(oldName.cStr(), newName.cStr()) == 0);
	}
	
	bool Dir::move(const String& dirName, const String& path)
	{
		String name = Dir::normalize(dirName);
		String path_name = Dir::normalize(path);
		return Dir::rename(name, Dir::joinPath(path_name, Dir::baseName(name), false));
	}
	
	bool Dir::copy(const String& oldDirName, const String& newDirName)
	{
		String oldName = Dir::normalize(oldDirName);
		String newName = Dir::normalize(newDirName);
		if (!Dir::exists(oldName) || Dir::exists(newName))
		{
			return false;
		}
		Dir::create(newName);
		Array<String> directories = Dir::directories(oldName);
		foreach (String, it, directories)
		{
			Dir::copy(Dir::joinPath(oldName, (*it), false), Dir::joinPath(newName, (*it), false));
		}
		Array<String> files = Dir::files(oldName);
		foreach (String, it, files)
		{
			File::copy(Dir::joinPath(oldName, (*it), false), Dir::joinPath(newName, (*it), false));
		}
		return true;
	}
	
	Array<String> Dir::entries(const String& dirName, bool prependDir)
	{
		String name = Dir::normalize(dirName);
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				result += String::fromUnicode(entry->d_name);
			}
			if (!result.has("."))
			{
				result += ".";
			}
			if (!result.has(".."))
			{
				result += "..";
			}
			_closedir(dir);
		}
		if (prependDir)
		{
			Dir::_prependDirectory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::contents(const String& dirName, bool prependDir)
	{
		String name = Dir::normalize(dirName);
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				result += String::fromUnicode(entry->d_name);
			}
			if (result.has("."))
			{
				result.remove(".");
			}
			if (result.has(".."))
			{
				result.remove("..");
			}
			_closedir(dir);
		}
		if (prependDir)
		{
			Dir::_prependDirectory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::directories(const String& dirName, bool prependDir)
	{
		String name = Dir::normalize(dirName);
		String current;
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				current = Dir::joinPath(name, String::fromUnicode(entry->d_name), false);
				if (Dir::exists(current))
				{
					result += String::fromUnicode(entry->d_name);
				}
			}
			if (result.has("."))
			{
				result.remove(".");
			}
			if (result.has(".."))
			{
				result.remove("..");
			}
			_closedir(dir);
		}
		if (prependDir)
		{
			Dir::_prependDirectory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::files(const String& dirName, bool prependDir)
	{
		String name = Dir::normalize(dirName);
		String current;
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				current = Dir::joinPath(name, String::fromUnicode(entry->d_name), false);
				if (File::exists(current))
				{
					result += String::fromUnicode(entry->d_name);
				}
			}
			if (result.has("."))
			{
				result.remove(".");
			}
			if (result.has(".."))
			{
				result.remove("..");
			}
			_closedir(dir);
		}
		if (prependDir)
		{
			Dir::_prependDirectory(name, result);
		}
		return result;
	}

	void Dir::chdir(const String& dirName)
	{
#ifdef _WIN32
#ifndef _WINRT
		_wchdir(Dir::systemize(dirName).wStr().c_str());
#endif
#else
		_chdir(Dir::systemize(dirName).cStr());
#endif
	}

	String Dir::cwd()
	{
#ifndef _WINRT
		char dir[FILENAME_MAX] = {'\0'};
		_getcwd(dir, FILENAME_MAX);
		return Dir::systemize(dir);
#else
		return winrtcwd;
#endif
	}

}
