/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
	String DirBase::basedir(const String& path)
	{
		harray<String> result = DirBase::split_path(path);
		if (result.size() < 2)
		{
			return ".";
		}
		result.remove_last();
		return DirBase::join_paths(result, false);
	}

	String DirBase::basename(const String& path)
	{
		harray<String> result = DirBase::split_path(path);
		return (result.size() > 0 ? result.remove_last() : hstr(""));
	}

	String DirBase::systemize(const String& path)
	{
		String result = path.replace('\\', '/');
		if (result.contains("//"))
		{
#ifdef _DEBUG // using _platform_print() directory to avoid possible deadlock when saving to file during logging
			hltypes::_platform_print(hltypes::logTag, "The path '" + result + "' contains multiple consecutive '/' (slash) characters. It will be systemized properly, but you may want to consider fixing this.", LEVEL_WARN);
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
		harray<String> directories = DirBase::split_path(path);
		harray<String> result;
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
		return DirBase::join_paths(result);
	}
		
	String DirBase::join_path(const String& path1, const String& path2, bool systemize_result)
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
		if (systemize_result)
		{
			result = systemize(result);
		}
		return result;
	}

	String DirBase::join_paths(Array<String> paths, bool systemize_result)
	{
		if (paths.size() == 0)
		{
			return "";
		}
		hstr path;
		while (paths.size() > 1)
		{
			path = paths.remove_last();
			paths[paths.size() - 1] = DirBase::join_path(paths.last(), path, systemize_result);
		}
		return paths[0];
	}

	Array<String> DirBase::split_path(const String& path)
	{
		return DirBase::systemize(path).rtrim('/').split('/');
	}

	void DirBase::_prepend_directory(const String& dirname, Array<String>& entries)
	{
		if (dirname != "")
		{
			foreach (String, it, entries)
			{
				(*it) = DirBase::join_path(dirname, (*it));
			}
		}
	}
	
}
