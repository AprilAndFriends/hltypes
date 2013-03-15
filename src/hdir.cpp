/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#include "msvc_dirent.h"
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define _mkdir(name) ::mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define _rmdir(name) ::rmdir(name)
#define _chdir(name) ::chdir(name)
#define _getcwd(buffer, size) ::getcwd(buffer, size)
#define _opendir(name) opendir(name.c_str())
#define _readdir(dirp) readdir(dirp)
#define _closedir(dirp) closedir(dirp)
#endif
#ifdef HAVE_ZIPRESOURCE
#include "zipaccess.h"
#endif

// prevents recursive calls of Dir::rename as this function is called via this pointer
int (*d_rename)(const char* old_name, const char* new_name) = rename;

#include "harray.h"
#include "hdir.h"
#include "hfile.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hresource.h"
#include "hstring.h"

#ifdef _WIN32
#if _HL_WINRT
#define _chdir(name) winrtcwd = name
static hltypes::String winrtcwd = ".";
#elif defined(_MSC_VER)
#include <AccCtrl.h>
#endif
#endif

namespace hltypes
{
	bool Dir::win32FullDirectoryPermissions = true;

#if defined(_WIN32) && defined(_MSC_VER) && !_HL_WINRT // god help us all
	static bool _mkdirWin32FullPermissions(const String& path)
	{
		typedef BOOL (WINAPI* TInitSD)(PSECURITY_DESCRIPTOR, DWORD);
		typedef BOOL (WINAPI* TAllocSid)(PSID_IDENTIFIER_AUTHORITY, BYTE, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PSID*);
		typedef DWORD (WINAPI* TSetEntAcl)(ULONG, PEXPLICIT_ACCESS, PACL, PACL*);
		typedef DWORD (WINAPI* TSetSDAcl)(PSECURITY_DESCRIPTOR, BOOL, PACL, BOOL);
		typedef PVOID (WINAPI* TFreeSid)(PSID);
		HMODULE hAdvapi32 = LoadLibrary(L"Advapi32.dll");
		if (hAdvapi32 == 0)
		{
			return false;
		}
		TInitSD pInitSD = reinterpret_cast<TInitSD>(GetProcAddress(hAdvapi32, "InitializeSecurityDescriptor"));
		TAllocSid pAllocSid = reinterpret_cast<TAllocSid>(GetProcAddress(hAdvapi32, "AllocateAndInitializeSid"));
		TSetEntAcl pSetEntAcl = reinterpret_cast<TSetEntAcl>(GetProcAddress(hAdvapi32, "SetEntriesInAclW"));
		TSetSDAcl pSetSDAcl = reinterpret_cast<TSetSDAcl>(GetProcAddress(hAdvapi32, "SetSecurityDescriptorDacl"));
		TFreeSid pFreeSid = reinterpret_cast<TFreeSid>(GetProcAddress(hAdvapi32, "FreeSid"));
		if (!pInitSD || !pAllocSid || !pSetEntAcl || !pSetSDAcl || !pFreeSid)
		{
			return false;
		}
		SECURITY_DESCRIPTOR sd;
		if (!pInitSD(&sd, SECURITY_DESCRIPTOR_REVISION))
		{
			return false;
		}
		PSID pEveryoneSid;
		SID_IDENTIFIER_AUTHORITY authority = SECURITY_WORLD_SID_AUTHORITY;
		bool result = false;
		if (pAllocSid(&authority, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSid))
		{
			EXPLICIT_ACCESS ea;
			memset(&ea, 0, sizeof(ea));
			ea.grfAccessPermissions = GENERIC_ALL;
			ea.grfAccessMode = SET_ACCESS;
			ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
			ea.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
			ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea.Trustee.ptstrName = static_cast<LPWSTR>(pEveryoneSid);
			PACL acl;
			if (pSetEntAcl(1, &ea, 0, &acl) == ERROR_SUCCESS)
			{
				if (pSetSDAcl(&sd, true, acl, false))
				{
					SECURITY_ATTRIBUTES sa;
					sa.nLength = sizeof(sa);
					sa.lpSecurityDescriptor = &sd;
					sa.bInheritHandle = false;
					result = (CreateDirectory(path.w_str().c_str(), &sa) == TRUE);
				}
				LocalFree(acl);
			}
			pFreeSid(pEveryoneSid);
		}
		return result;
	}
#endif

	bool _check_dir_prefix(String& path, const String& prefix)
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
	
	static bool hmkdir(const String& path)
	{
		// TODO
		/*
		wchar_t* wpath = utf8_to_unicode(path);
		bool result =_wmkdir(wpath)
		delete [] wpath;
		return result;

		*/
#if defined(_WIN32) && defined(_MSC_VER) && !_HL_WINRT
		if (Dir::getWin32FullDirectoryPermissions() && _mkdirWin32FullPermissions(path))
		{
			return true;
		}
#endif
		return (_mkdir(path.c_str()) != 0);
	}
	
	static bool hremove(const String& dirname)
	{
		// TODO
		/*
		wchar_t* wdirname = utf8_to_unicode(dirname);
		bool result =_wremove(wdirname)
		delete [] wdirname;
		return result;
		*/
		return (remove(dirname.c_str()) != 0);
	}
	
	static bool hrmdir(const String& dirname)
	{
		// TODO
		/*
		wchar_t* wdirname = utf8_to_unicode(dirname);
		bool result = _wrmdir(wdirname);
		delete [] wdirname;
		return result;
		*/
		return (_rmdir(dirname.c_str()) != 0);
	}
	
	bool Dir::create(const String& dirname)
	{
		String name = normalize_path(dirname);
		if (name == "" || Dir::exists(name))
		{
			return false;
		}
		Array<String> folders = name.split("/", -1, false);
		if (folders.size() > 0)
		{
			String path = folders.remove_first();
			hmkdir(path);
			foreach (String, it, folders)
			{
				path += "/" + (*it);
				hmkdir(path);
			}
		}
		return Dir::exists(dirname);
	}
	
	bool Dir::create_new(const String& dirname)
	{
		return (Dir::create(dirname) || Dir::clear(dirname));
	}
	
	bool Dir::remove(const String& dirname)
	{
		String name = normalize_path(dirname);
		if (name == "" || !Dir::exists(name))
		{
			return false;
		}
		Array<String> directories = Dir::directories(name);
		foreach (String, it, directories)
		{
			Dir::remove(name + "/" + (*it));
		}
		Array<String> files = Dir::files(name);
		foreach (String, it, files)
		{
			hfile::remove(name + "/" + (*it));
		}
		hrmdir(name);
		return Dir::exists(name);
	}
	
	bool Dir::exists(const String& dirname)
	{
		String name = normalize_path(dirname);
		DIR* dir = _opendir(name);
		if (dir != NULL)
		{
			_closedir(dir);
			return true;
		}
		return false;
	}
	
	bool Dir::resource_exists(const String& dirname)
	{
		String name = normalize_path(dirname);
		if (name == "" || name == ".")
		{
			return true;
		}
#ifndef HAVE_ZIPRESOURCE
		return Dir::exists(Resource::make_full_path(name));
#else
		return Dir::resource_directories(get_basedir(name)).contains(get_basename(name));
#endif
	}
	
	bool Dir::clear(const String& dirname)
	{
		String name = normalize_path(dirname);
		if (name == "" || !Dir::exists(name))
		{
			return false;
		}
		Array<String> directories = Dir::directories(name);
		foreach (String, it, directories)
		{
			Dir::remove(name + "/" + (*it));
		}
		Array<String> files = Dir::files(name);
		foreach (String, it, files)
		{
			hfile::remove(name + "/" + (*it));
		}
		return (directories.size() > 0 || files.size() > 0);
	}
	
	bool Dir::rename(const String& old_dirname, const String& new_dirname)
	{
		String old_name = normalize_path(old_dirname);
		String new_name = normalize_path(new_dirname);
		if (!Dir::exists(old_name) || Dir::exists(new_name))
		{
			return false;
		}
		Dir::create_path(new_name);
		return (d_rename(old_name.c_str(), new_name.c_str()) == 0);
	}
	
	bool Dir::move(const String& dirname, const String& path)
	{
		String name = normalize_path(dirname);
		String path_name = normalize_path(path);
		return Dir::rename(name, path_name + "/" + name.rsplit("/", 1, false).remove_last());
	}
	
	bool Dir::copy(const String& old_dirname, const String& new_dirname)
	{
		String old_name = normalize_path(old_dirname);
		String new_name = normalize_path(new_dirname);
		if (!Dir::exists(old_name) || Dir::exists(new_name))
		{
			return false;
		}
		Dir::create(new_name);
		Array<String> directories = Dir::directories(old_name);
		foreach (String, it, directories)
		{
			Dir::copy(old_name + "/" + (*it), new_name + "/" + (*it));
		}
		Array<String> files = Dir::files(old_name);
		foreach (String, it, files)
		{
			hfile::copy(old_name + "/" + (*it), new_name + "/" + (*it));
		}
		return true;
	}
	
	bool Dir::create_path(const String& path)
	{
		String name = get_basedir(path);
		return (name != "" && name != "." && Dir::create(name));
	}
	
	void prepend_directory(const String& dirname, Array<String>& entries)
	{
		if (dirname != "")
		{
			foreach (String, it, entries)
			{
				(*it) = dirname + "/" + (*it);
			}
		}
	}
	
	Array<String> Dir::entries(const String& dirname, bool prepend_dir)
	{
		String name = normalize_path(dirname);
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				result += String::from_unicode(entry->d_name);
			}
			if (!result.contains("."))
			{
				result += ".";
			}
			if (!result.contains(".."))
			{
				result += "..";
			}
			_closedir(dir);
		}
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::resource_entries(const String& dirname, bool prepend_dir)
	{
		String name = normalize_path(dirname);
		Array<String> result = Dir::resource_contents(dirname, false);
		result += String(".");
		result += String("..");
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::contents(const String& dirname, bool prepend_dir)
	{
		String name = normalize_path(dirname);
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				result += String::from_unicode(entry->d_name);
			}
			if (result.contains("."))
			{
				result.remove(".");
			}
			if (result.contains(".."))
			{
				result.remove("..");
			}
			_closedir(dir);
		}
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::resource_contents(const String& dirname, bool prepend_dir)
	{
		String name = normalize_path(dirname);
#ifndef HAVE_ZIPRESOURCE
		String full_path = Resource::make_full_path(name);
		Array<String> result = Dir::directories(full_path, false) + Dir::files(full_path, false);
		for_iter (i, 0, result.size())
		{
			result[i] = result[i].replace(Resource::getCwd() + "/", "");
		}
#else
		Array<String> result;
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
				if (_check_dir_prefix(current, cwd) && _check_dir_prefix(current, name))
				{
					slashCount = current.count('/');
					if (slashCount == 0) // file
					{
						result += current;
					}
					else if (slashCount == 1) // directory
					{
						result += current.split('/', 1).remove_first();
					}
				}
			}
		}
#endif
		result.remove_duplicates();
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::directories(const String& dirname, bool prepend_dir)
	{
		String name = normalize_path(dirname);
		String current;
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				current = name + "/" + String::from_unicode(entry->d_name);
				if (Dir::exists(current))
				{
					result += String::from_unicode(entry->d_name);
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
			_closedir(dir);
		}
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::resource_directories(const String& dirname, bool prepend_dir)
	{
		String name = normalize_path(dirname);
#ifndef HAVE_ZIPRESOURCE
		Array<String> result = Dir::directories(Resource::make_full_path(name), false);
		for_iter (i, 0, result.size())
		{
			result[i] = result[i].replace(Resource::getCwd() + "/", "");
		}
#else
		Array<String> result;
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
				if (_check_dir_prefix(current, cwd) && _check_dir_prefix(current, name) && current.count('/') == 1)
				{
					result += current.split('/', 1).remove_first();
				}
			}
		}
#endif
		result.remove_duplicates();
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}

	Array<String> Dir::files(const String& dirname, bool prepend_dir)
	{
		String name = normalize_path(dirname);
		String current;
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				current = name + "/" + String::from_unicode(entry->d_name);
				if (hfile::exists(current))
				{
					result += String::from_unicode(entry->d_name);
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
			_closedir(dir);
		}
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}

	Array<String> Dir::resource_files(const String& dirname, bool prepend_dir)
	{
		String name = normalize_path(dirname);
#ifndef HAVE_ZIPRESOURCE
		Array<String> result = Dir::files(Resource::make_full_path(name), false);
		String cwd = Resource::getCwd() + "/";
		if (cwd != "./" && cwd != "/")
		{
			for_iter (i, 0, result.size())
			{
				result[i] = result[i].replace(cwd, "");
			}
		}
#else
		Array<String> result;
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
				if (_check_dir_prefix(current, cwd) && _check_dir_prefix(current, name) && current.count('/') == 0)
				{
					result += current;
				}
			}
		}
#endif
		result.remove_duplicates();
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}

	void Dir::chdir(const String& dirname)
	{
		_chdir(dirname.c_str());
	}

	String Dir::cwd()
	{
#if !_HL_WINRT
		char dir[FILENAME_MAX] = {'\0'};
		_getcwd(dir, FILENAME_MAX);
		return systemize_path(dir);
#else
		return winrtcwd;
#endif
	}

}
