/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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

	bool ResourceDir::exists(const String& dirname)
	{
		String name = ResourceDir::normalize(dirname);
		if (name == "" || name == ".")
		{
			return true;
		}
#ifdef _ZIPRESOURCE
		return ResourceDir::directories(ResourceDir::basedir(name)).contains(ResourceDir::basename(name));
#else
		return Dir::exists(Resource::make_full_path(name));
#endif
	}
	
	Array<String> ResourceDir::entries(const String& dirname, bool prepend_dir)
	{
		Array<String> result = ResourceDir::contents(dirname, false);
		result += String(".");
		result += String("..");
		if (prepend_dir)
		{
			ResourceDir::_prepend_directory(dirname, result);
		}
		return result;
	}
	
	Array<String> ResourceDir::contents(const String& dirname, bool prepend_dir)
	{
#ifdef _ZIPRESOURCE
		Array<String> result = ResourceDir::directories(dirname, false) + ResourceDir::files(dirname, false);
#else
		String name = Resource::make_full_path(dirname);
		Array<String> result = ResourceDir::_remove_cwd(Dir::directories(name, false) + Dir::files(name, false)).removed_duplicates();
#endif
		if (prepend_dir)
		{
			ResourceDir::_prepend_directory(dirname, result);
		}
		return result;
	}
	
	Array<String> ResourceDir::directories(const String& dirname, bool prepend_dir)
	{
		String name = Resource::make_full_path(dirname);
		Array<String> result;
#ifdef _ZIPRESOURCE
		if (ResourceDir::cacheDirectories.has_key(name))
		{
			result = ResourceDir::cacheDirectories[name];
		}
		else
		{
			Array<String> files = ResourceDir::_get_internal_files();
			String current;
			foreach (String, it, files)
			{
				current = (*it);
				if (ResourceDir::_check_dir_prefix(current, name) && current != "" && current.contains('/'))
				{
					result += current.split('/', 1).first();
				}
			}
			result.remove_duplicates();
			ResourceDir::cacheDirectories[name] = result;
		}
#else
		result = ResourceDir::_remove_cwd(Dir::directories(name, false)).removed_duplicates();
#endif
		if (prepend_dir)
		{
			ResourceDir::_prepend_directory(dirname, result);
		}
		return result;
	}

	Array<String> ResourceDir::files(const String& dirname, bool prepend_dir)
	{
		String name = Resource::make_full_path(dirname);
		Array<String> result;
#ifdef _ZIPRESOURCE
		if (ResourceDir::cacheFiles.has_key(name))
		{
			result = ResourceDir::cacheFiles[name];
		}
		else
		{
			Array<String> files = ResourceDir::_get_internal_files();
			String current;
			foreach (String, it, files)
			{
				current = (*it);
				if (ResourceDir::_check_dir_prefix(current, name) && current != "" && !current.contains('/'))
				{
					result += current;
				}
			}
			result.remove_duplicates();
			ResourceDir::cacheFiles[name] = result;
		}
#else
		result = ResourceDir::_remove_cwd(Dir::files(name, false)).removed_duplicates();
#endif
		if (prepend_dir)
		{
			ResourceDir::_prepend_directory(dirname, result);
		}
		return result;
	}

	bool ResourceDir::_check_dir_prefix(String& path, const String& prefix)
	{
		if (prefix == "" || prefix == ".")
		{
			if (path.starts_with("./"))
			{
				path = path(2, path.size() - 2);
			}
			return true;
		}
		if (path.starts_with(prefix + "/"))
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
	
	Array<String> ResourceDir::_get_internal_files()
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

	Array<String> ResourceDir::_remove_cwd(Array<String> paths)
	{
		String cwd = Resource::getCwd() + "/";
		if (cwd != "/" && cwd != "./")
		{
			for_iter (i, 0, paths.size())
			{
				paths[i] = paths[i].replace(cwd, "");
			}
		}
		return paths;
	}

}
