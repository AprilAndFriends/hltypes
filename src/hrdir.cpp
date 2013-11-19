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
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"

#ifdef _ZIPRESOURCE
#include "zipaccess.h"
#endif

namespace hltypes
{
	bool ResourceDir::exists(const String& dirname)
	{
		String name = Dir::normalize(dirname);
		if (name == "" || name == ".")
		{
			return true;
		}
#ifdef _ZIPRESOURCE
		return ResourceDir::directories(Dir::basedir(name)).contains(Dir::basename(name));
#else
		return Dir::exists(Resource::make_full_path(name));
#endif
	}
	
	Array<String> ResourceDir::entries(const String& dirname, bool prepend_dir)
	{
		String name = Dir::normalize(dirname);
		Array<String> result = ResourceDir::contents(dirname, false);
		result += String(".");
		result += String("..");
		if (prepend_dir)
		{
			DirBase::_prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> ResourceDir::contents(const String& dirname, bool prepend_dir)
	{
		String name = Dir::normalize(dirname);
		Array<String> result;
#ifdef _ZIPRESOURCE
		String cwd = Resource::getCwd();
		void* a = zip::open(NULL); // NULL, because this is a static function which will close the archive right after it is done
		if (a != NULL)
		{
			Array<String> files = zip::getFiles(a);
			zip::close(NULL, a);
			String current;
			int slashCount = 0;
			foreach (String, it, files)
			{
				current = (*it);
				if (ResourceDir::_check_dir_prefix(current, cwd) && ResourceDir::_check_dir_prefix(current, name))
				{
					slashCount = current.count('/');
					if (slashCount == 0) // file
					{
						result += current;
					}
					else if (slashCount == 1) // directory
					{
						result += current.split('/', 1).first();
					}
				}
			}
		}
#else
		String full_path = Resource::make_full_path(name);
		result = ResourceDir::_remove_cwd(Dir::directories(full_path, false) + Dir::files(full_path, false));
#endif
		result.remove_duplicates();
		if (prepend_dir)
		{
			DirBase::_prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> ResourceDir::directories(const String& dirname, bool prepend_dir)
	{
		String name = Dir::normalize(dirname);
		Array<String> result;
#ifdef _ZIPRESOURCE
		String cwd = Resource::getCwd();
		void* a = zip::open(NULL); // NULL, because this is a static function which will close the archive right after it is done
		if (a != NULL)
		{
			Array<String> files = zip::getFiles(a);
			zip::close(NULL, a);
			String current;
			foreach (String, it, files)
			{
				current = (*it);
				if (ResourceDir::_check_dir_prefix(current, cwd) && ResourceDir::_check_dir_prefix(current, name) && current.count('/') == 1)
				{
					result += current.split('/', 1).first();
				}
			}
		}
#else
		result = ResourceDir::_remove_cwd(Dir::directories(Resource::make_full_path(name), false));
#endif
		result.remove_duplicates();
		if (prepend_dir)
		{
			DirBase::_prepend_directory(name, result);
		}
		return result;
	}

	Array<String> ResourceDir::files(const String& dirname, bool prepend_dir)
	{
		String name = Dir::normalize(dirname);
		Array<String> result;
#ifdef _ZIPRESOURCE
		String cwd = Resource::getCwd();
		void* a = zip::open(NULL); // NULL, because this is a static function which will close the archive right after it is done
		if (a != NULL)
		{
			Array<String> files = zip::getFiles(a);
			zip::close(NULL, a);
			String current;
			foreach (String, it, files)
			{
				current = (*it);
				if (ResourceDir::_check_dir_prefix(current, cwd) && ResourceDir::_check_dir_prefix(current, name) && current.count('/') == 0)
				{
					result += current;
				}
			}
		}
#else
		result = ResourceDir::_remove_cwd(Dir::files(Resource::make_full_path(name), false));
#endif
		result.remove_duplicates();
		if (prepend_dir)
		{
			DirBase::_prepend_directory(name, result);
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
