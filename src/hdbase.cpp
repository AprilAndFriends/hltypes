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
		result.removeLast();
		return DirBase::joinPaths(result, false);
	}

	String DirBase::baseName(const String& path)
	{
		Array<String> result = DirBase::splitPath(path);
		return (result.size() > 0 ? result.removeLast() : String());
	}

	String DirBase::systemize(const String& path)
	{
		String result = path;
#ifdef _WIN32
		bool networkDrive = path.startsWith("\\\\");
		if (networkDrive)
		{
			result = result(2, -1);
		}
#endif
		result = result.replaced('\\', '/');
		if (result.contains("//"))
		{
#ifdef _DEBUG // using _platformPrint() directory to avoid possible deadlock when saving to file during logging
			hltypes::_platformPrint(logTag, "The path '" + result + "' contains multiple consecutive '/' (slash) characters. It will be systemized properly, but you may want to consider fixing this.", LEVEL_WARN);
#endif
			for (int i = 0; i < 10000 && result.contains("//"); ++i) // 10000 loops are there to prevent a possible infinite loop
			{
				result = result.replaced("//", "/");
			}
		}
#ifdef _WIN32
		// there could still be a single leading slash
		if (result.size() > 0 && result[0] == '/')
		{
			result = result(1, -1);
		}
		if (networkDrive)
		{
			result = "\\\\" + result;
		}
#endif
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
				directories.removeFirst();
			}
			else if (directories.first() == "..")
			{
				if (result.size() == 0)
				{
					result += directories.removeFirst();
				}
				else if (result.last() == "..")
				{
					result += directories.removeFirst();
				}
				else
				{
					result.removeLast();
					directories.removeFirst();
				}
			}
			else
			{
				result += directories.removeFirst();
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
		bool slash1 = path1.endsWith("/");
		bool slash2 = path2.startsWith("/");
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
			result = path1.subString(0, path1.size() - 1) + path2;
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
			path = paths.removeLast();
			paths[paths.size() - 1] = DirBase::joinPath(paths.last(), path, systemizeResult);
		}
		return paths[0];
	}

	Array<String> DirBase::splitPath(const String& path)
	{
		return DirBase::systemize(path).trimmedRight('/').split('/');
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
