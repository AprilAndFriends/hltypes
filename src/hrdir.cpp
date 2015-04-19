/// @file
/// @version 3.0
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
		if (Resource::isZipArchive())
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
		return Dir::exists(Resource::makeFullPath(name), caseSensitive);
	}
	
	Array<String> ResourceDir::entries(const String& dirName, bool prependDir)
	{
		Array<String> result = ResourceDir::contents(dirName, false);
		result += String(".");
		result += String("..");
		if (prependDir)
		{
			ResourceDir::_prependDirectory(dirName, result);
		}
		return result;
	}
	
	Array<String> ResourceDir::contents(const String& dirName, bool prependDir)
	{
		Array<String> result;
		result = ResourceDir::directories(dirName, false) + ResourceDir::files(dirName, false);
		if (!Resource::isZipArchive())
		{
			result.removeDuplicates();
		}
		if (prependDir)
		{
			ResourceDir::_prependDirectory(dirName, result);
		}
		return result;
	}
	
	Array<String> ResourceDir::directories(const String& dirName, bool prependDir)
	{
		String name = Resource::makeFullPath(dirName);
		Array<String> result;
#ifdef _ZIPRESOURCE
		if (Resource::isZipArchive())
		{
			if (ResourceDir::cacheDirectories.hasKey(name))
			{
				result = ResourceDir::cacheDirectories[name];
			}
			else
			{
				Array<String> files = ResourceDir::_getInternalFiles();
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
			result = ResourceDir::_removeCwd(Dir::directories(name, false)).removedDuplicates();
		}
		if (prependDir)
		{
			ResourceDir::_prependDirectory(dirName, result);
		}
		return result;
	}

	Array<String> ResourceDir::files(const String& dirName, bool prependDir)
	{
		String name = Resource::makeFullPath(dirName);
		Array<String> result;
#ifdef _ZIPRESOURCE
		if (Resource::isZipArchive())
		{
			if (ResourceDir::cacheFiles.hasKey(name))
			{
				result = ResourceDir::cacheFiles[name];
			}
			else
			{
				Array<String> files = ResourceDir::_getInternalFiles();
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
			result = ResourceDir::_removeCwd(Dir::files(name, false)).removedDuplicates();
		}
		if (prependDir)
		{
			ResourceDir::_prependDirectory(dirName, result);
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
	
	Array<String> ResourceDir::_getInternalFiles()
	{
		Array<String> files;
#ifdef _ZIPRESOURCE
		void* a = zip::open(NULL); // NULL, because this is a static function which will close the archive right after it is done
		if (a != NULL)
		{
			files = zip::getFiles(a);
			zip::close(NULL, a);
		}
#endif
		return files;
	}

	Array<String> ResourceDir::_removeCwd(Array<String> paths)
	{
		String cwd = Resource::getCwd() + "/";
		if (!Resource::isZipArchive())
		{
			if (cwd != "/" && cwd != "./")
			{
				cwd = Resource::getArchive() + "/" + cwd;
			}
			else
			{
				cwd = Resource::getArchive();
			}
		}
		if (cwd != "/" && cwd != "./")
		{
			for_iter (i, 0, paths.size())
			{
				paths[i] = paths[i].replaced(cwd, "");
			}
		}
		return paths;
	}

}
