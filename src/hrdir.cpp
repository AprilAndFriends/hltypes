/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "harray.h"
#include "hdir.h"
#include "hmap.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"
#ifdef _ZIPRESOURCE
#include "zipaccess.h"
#endif

namespace hltypes
{
	Map<String, Array<String> > ResourceDir::cacheDirectories;
	Map<String, Array<String> > ResourceDir::cacheFiles;

	bool ResourceDir::exists(const String& dirName, bool caseSensitive)
	{
		String name = ResourceDir::normalize(dirName);
		if (name == "" || name == ".")
		{
			return true;
		}
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			// this approach is used, because sometimes ZIP files don't enumerate their directories
			bool result = ResourceDir::directories(ResourceDir::baseDir(name)).has(ResourceDir::baseName(name));
			if (!result && !caseSensitive)
			{
				String baseDir = ResourceDir::baseDir(name);
				String baseName = ResourceDir::baseName(name);
				Array<String> directories = ResourceDir::directories(baseDir);
				foreach (String, it, directories)
				{
					if ((*it).lowered() == baseName.lowered())
					{
						name = ResourceDir::joinPath(baseDir, (*it));
						result = true;
						break;
					}
				}
			}
			return result;
		}
#endif
		return Dir::exists(Resource::_makeNonZipPath(name), caseSensitive);
	}
	
	Array<String> ResourceDir::entries(const String& dirName, bool prependDir)
	{
		String name = ResourceDir::normalize(dirName);
		Array<String> result = ResourceDir::contents(name, false);
		result += String(".");
		result += String("..");
		if (prependDir)
		{
			ResourceDir::_prependDirectory(name, result);
		}
		return result;
	}
	
	Array<String> ResourceDir::contents(const String& dirName, bool prependDir)
	{
		String name = ResourceDir::normalize(dirName);
		Array<String> result;
		result = ResourceDir::directories(name, false) + ResourceDir::files(name, false);
		if (!Resource::zipMounts)
		{
			result.removeDuplicates();
		}
		if (prependDir)
		{
			ResourceDir::_prependDirectory(name, result);
		}
		return result;
	}
	
	Array<String> ResourceDir::directories(const String& dirName, bool prependDir)
	{
		String name = ResourceDir::normalize(dirName);
		Array<String> result;
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			if (ResourceDir::cacheDirectories.hasKey(name))
			{
				result = ResourceDir::cacheDirectories[name];
			}
			else
			{
				Array<String> files = zip::getFiles();
				String current;
				foreach (String, it, files)
				{
					current = (*it);
					if (ResourceDir::_checkDirPrefix(current, name) && current != "" && current.contains('/'))
					{
						result += current.split('/', 1).first();
					}
				}
				result.removeDuplicates();
				ResourceDir::cacheDirectories[name] = result;
			}
		}
		else
#endif
		{
			result = Dir::directories(Resource::_makeNonZipPath(name), false).removedDuplicates();
		}
		if (prependDir)
		{
			ResourceDir::_prependDirectory(name, result);
		}
		return result;
	}

	Array<String> ResourceDir::files(const String& dirName, bool prependDir)
	{
		String name = ResourceDir::normalize(dirName);
		Array<String> result;
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			if (ResourceDir::cacheFiles.hasKey(name))
			{
				result = ResourceDir::cacheFiles[name];
			}
			else
			{
				Array<String> files = zip::getFiles();
				String current;
				foreach (String, it, files)
				{
					current = (*it);
					if (ResourceDir::_checkDirPrefix(current, name) && current != "" && !current.contains('/'))
					{
						result += current;
					}
				}
				result.removeDuplicates();
				ResourceDir::cacheFiles[name] = result;
			}
		}
		else
#endif
		{
			result = Dir::files(Resource::_makeNonZipPath(name), false).removedDuplicates();
		}
		if (prependDir)
		{
			ResourceDir::_prependDirectory(name, result);
		}
		return result;
	}

	bool ResourceDir::_checkDirPrefix(String& path, const String& prefix)
	{
		if (prefix == "" || prefix == ".")
		{
			if (path.startsWith("./"))
			{
				path = path(2, path.size() - 2);
			}
			return true;
		}
		if (path.startsWith(prefix + "/"))
		{
			path = path(prefix.size() + 1, path.size() - prefix.size() - 1);
			if (path == "")
			{
				return false;
			}
			return true;
		}
		return false;
	}
	
}
