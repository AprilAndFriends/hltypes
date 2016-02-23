/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "harray.h"
#include "hdir.h"
#include "hfile.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"
#include "platform_internal.h"

namespace hltypes
{
	bool Dir::win32FullDirectoryPermissions = true;

	bool Dir::isWin32FullDirectoryPermissions()
	{
		return win32FullDirectoryPermissions;
	}

	void Dir::setWin32FullDirectoryPermissions(bool value)
	{
		win32FullDirectoryPermissions = value;
	}

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
		_platformDir* dir = _platformOpenDirectory(name);
		if (dir != NULL)
		{
			_platformCloseDirectory(dir);
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
		return _platformRenameDirectory(oldName, newName);
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
			_platformDir* dir = _platformOpenDirectory(name);
			_platformDirEntry* entry = NULL;
			while ((entry = _platformReadDirectory(dir)))
			{
				result += _platformGetDirEntryName(entry);
			}
			if (!result.has("."))
			{
				result += ".";
			}
			if (!result.has(".."))
			{
				result += "..";
			}
			_platformCloseDirectory(dir);
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
			_platformDir* dir = _platformOpenDirectory(name);
			_platformDirEntry* entry = NULL;
			while ((entry = _platformReadDirectory(dir)))
			{
				result += _platformGetDirEntryName(entry);
			}
			if (result.has("."))
			{
				result.remove(".");
			}
			if (result.has(".."))
			{
				result.remove("..");
			}
			_platformCloseDirectory(dir);
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
		Array<String> result;
		if (Dir::exists(name))
		{
			_platformDir* dir = _platformOpenDirectory(name);
			_platformDirEntry* entry = NULL;
			String entryName;
			while ((entry = _platformReadDirectory(dir)))
			{
				entryName = _platformGetDirEntryName(entry);
				if (Dir::exists(Dir::joinPath(name, entryName, false)))
				{
					result += entryName;
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
			_platformCloseDirectory(dir);
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
		Array<String> result;
		if (Dir::exists(name))
		{
			_platformDir* dir = _platformOpenDirectory(name);
			_platformDirEntry* entry = NULL;
			String entryName;
			while ((entry = _platformReadDirectory(dir)))
			{
				entryName = _platformGetDirEntryName(entry);
				if (File::exists(Dir::joinPath(name, entryName, false)))
				{
					result += entryName;
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
			_platformCloseDirectory(dir);
		}
		if (prependDir)
		{
			Dir::_prependDirectory(name, result);
		}
		return result;
	}

	void Dir::chdir(const String& dirName)
	{
		return _platformChdir(Dir::systemize(dirName));
	}

	String Dir::cwd()
	{
		return _platformCwd();
	}

}
