/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "harray.h"
#include "hdbase.h"
#include "hstring.h"

#ifdef _ANDROID // needed for Android only anyway so this "second" definition (aside from hlog.h) is ok (needed for systemize)
#include <android/log.h>
#define LEVEL_WARN ((int)ANDROID_LOG_WARN)
#else
#define LEVEL_WARN 2
#endif

namespace hltypes
{
	String DirBase::baseDir(const String& path)
	{
		Array<String> result = DirBase::splitPath(path);
		if (result.size() < 2)
		{
			return ".";
		}
		result.remove_last();
		return DirBase::joinPaths(result, false);
	}

	String DirBase::baseName(const String& path)
	{
		Array<String> result = DirBase::splitPath(path);
		return (result.size() > 0 ? result.remove_last() : String());
	}

	String DirBase::systemize(const String& path)
	{
		String result = path.replace('\\', '/');
		if (result.contains("//"))
		{
#ifdef _DEBUG // using _platformPrint() directory to avoid possible deadlock when saving to file during logging
			hltypes::_platformPrint(hltypes::logTag, "The path '" + result + "' contains multiple consecutive '/' (slash) characters. It will be systemized properly, but you may want to consider fixing this.", LEVEL_WARN);
#endif
			for (int i = 0; i < 1000 && result.contains("//"); ++i) // to prevent an infinite loop
			{
				result = result.replace("//", "/");
			}
		}
		return result;
	}

	String DirBase::normalize(const String& path)
	{
		Array<String> directories = DirBase::splitPath(path);
		Array<String> result;
		while (directories.size() > 0)
		{
			if (directories.first() == ".")
			{
				directories.remove_first();
			}
			else if (directories.first() == "..")
			{
				if (result.size() == 0)
				{
					result += directories.remove_first();
				}
				else if (result.last() == "..")
				{
					result += directories.remove_first();
				}
				else
				{
					result.remove_last();
					directories.remove_first();
				}
			}
			else
			{
				result += directories.remove_first();
			}
		}
		if (result.size() == 0)
		{
			return ".";
		}
		return DirBase::joinPaths(result);
	}
		
	String DirBase::joinPath(const String& path1, const String& path2, bool systemizeResult)
	{
		String result;
		bool slash1 = path1.ends_with("/");
		bool slash2 = path2.starts_with("/");
		if (!slash1 && !slash2)
		{
			result = path1 + "/" + path2;
		}
		else if (slash1 ^ slash2)
		{
			result = path1 + path2;
		}
		else // (slash1 && slash2)
		{
			result = path1.substr(0, path1.size() - 1) + path2;
		}
		if (systemizeResult)
		{
			result = systemize(result);
		}
		return result;
	}

	String DirBase::joinPaths(Array<String> paths, bool systemizeResult)
	{
		if (paths.size() == 0)
		{
			return "";
		}
		String path;
		while (paths.size() > 1)
		{
			path = paths.remove_last();
			paths[paths.size() - 1] = DirBase::joinPath(paths.last(), path, systemizeResult);
		}
		return paths[0];
	}

	Array<String> DirBase::splitPath(const String& path)
	{
		return DirBase::systemize(path).rtrim('/').split('/');
	}

	void DirBase::_prependDirectory(const String& dirname, Array<String>& entries)
	{
		if (dirname != "")
		{
			foreach (String, it, entries)
			{
				(*it) = DirBase::joinPath(dirname, (*it));
			}
		}
	}
	
}
