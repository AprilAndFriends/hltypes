/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 1.73
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>
#ifdef _WIN32
#ifdef _MSC_VER
#include <AccCtrl.h>
#endif
#include <direct.h>
#include "msvc_dirent.h"
#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_PARTITION_APP
#define _chdir(name) winrtcwd = name
static hstr winrtcwd = ".";
#endif
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
#include <zip/zip.h>
#endif

// prevents recursive calls of hdir::rename as this function is called via this pointer
int (*d_rename)(const char* old_name, const char* new_name) = rename;

#include "harray.h"
#include "hdir.h"
#include "hfile.h"
#include "hltypesUtil.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	bool Dir::win32FullDirectoryPermissions = true;

#if defined(_WIN32) && defined(_MSC_VER) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_PARTITION_DESKTOP) // god help us all
	static bool _mkdirWin32FullPermissions(chstr path)
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
					wchar_t* wpath = path.w_str();
					result = (CreateDirectory(wpath, &sa) == TRUE);
					delete [] wpath;
				}
				LocalFree(acl);
			}
			pFreeSid(pEveryoneSid);
		}
		return result;
	}
#endif

	bool _check_dir_prefix(hstr& path, chstr prefix)
	{
		if (prefix == "" || prefix == ".")
		{
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
	
	static bool hmkdir(chstr path)
	{
		/*
		wchar_t* wpath = utf8_to_unicode(path);
		bool result =_wmkdir(wpath)
		delete [] wpath;
		return result;

		*/
#if defined(_WIN32) && defined(_MSC_VER) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_PARTITION_DESKTOP)
		if (Dir::getWin32FullDirectoryPermissions() && _mkdirWin32FullPermissions(path))
		{
			return true;
		}
#endif
		return (_mkdir(path.c_str()) != 0);
	}
	
	static bool hremove(chstr dirname)
	{
		/*
		wchar_t* wdirname = utf8_to_unicode(dirname);
		bool result =_wremove(wdirname)
		delete [] wdirname;
		return result;
		*/
		return (remove(dirname.c_str()) != 0);
	}
	
	static bool hrmdir(chstr dirname)
	{
		/*
		wchar_t* wdirname = utf8_to_unicode(dirname);
		bool result = _wrmdir(wdirname);
		delete [] wdirname;
		return result;
		*/
		return (_rmdir(dirname.c_str()) != 0);
	}
	
	bool Dir::create(chstr dirname)
	{
		hstr name = normalize_path(dirname);
		if (name == "" || hdir::exists(name))
		{
			return false;
		}
		Array<hstr> folders = name.split("/", -1, false);
		if (folders.size() > 0)
		{
			hstr path = folders.pop_first();
			hmkdir(path);
			foreach (hstr, it, folders)
			{
				path += "/" + (*it);
				hmkdir(path);
			}
		}
		return hdir::exists(dirname);
	}
	
	bool Dir::create_new(chstr dirname)
	{
		return (hdir::create(dirname) || hdir::clear(dirname));
	}
	
	bool Dir::remove(chstr dirname)
	{
		hstr name = normalize_path(dirname);
		if (name == "" || !hdir::exists(name))
		{
			return false;
		}
		Array<hstr> directories = hdir::directories(name);
		foreach (hstr, it, directories)
		{
			hdir::remove(name + "/" + (*it));
		}
		Array<hstr> files = hdir::files(name);
		foreach (hstr, it, files)
		{
			hfile::remove(name + "/" + (*it));
		}
		hrmdir(name);
		return hdir::exists(name);
	}
	
	bool Dir::exists(chstr dirname)
	{
		hstr name = normalize_path(dirname);
		DIR* dir = _opendir(name);
		if (dir != NULL)
		{
			_closedir(dir);
			return true;
		}
		return false;
	}
	
	bool Dir::resource_exists(chstr dirname)
	{
		hstr name = normalize_path(dirname);
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
	
	bool Dir::clear(chstr dirname)
	{
		hstr name = normalize_path(dirname);
		if (name == "" || !hdir::exists(name))
		{
			return false;
		}
		Array<hstr> directories = hdir::directories(name);
		foreach (hstr, it, directories)
		{
			hdir::remove(name + "/" + (*it));
		}
		Array<hstr> files = hdir::files(name);
		foreach (hstr, it, files)
		{
			hfile::remove(name + "/" + (*it));
		}
		return (directories.size() > 0 || files.size() > 0);
	}
	
	bool Dir::rename(chstr old_dirname, chstr new_dirname)
	{
		hstr old_name = normalize_path(old_dirname);
		hstr new_name = normalize_path(new_dirname);
		if (!hdir::exists(old_name) || hdir::exists(new_name))
		{
			return false;
		}
		hdir::create_path(new_name);
		return (d_rename(old_name.c_str(), new_name.c_str()) == 0);
	}
	
	bool Dir::move(chstr dirname, chstr path)
	{
		hstr name = normalize_path(dirname);
		hstr path_name = normalize_path(path);
		return hdir::rename(name, path_name + "/" + name.rsplit("/", 1, false).pop_last());
	}
	
	bool Dir::copy(chstr old_dirname, chstr new_dirname)
	{
		hstr old_name = normalize_path(old_dirname);
		hstr new_name = normalize_path(new_dirname);
		if (!hdir::exists(old_name) || hdir::exists(new_name))
		{
			return false;
		}
		hdir::create(new_name);
		Array<hstr> directories = hdir::directories(old_name);
		foreach (hstr, it, directories)
		{
			hdir::copy(old_name + "/" + (*it), new_name + "/" + (*it));
		}
		Array<hstr> files = hdir::files(old_name);
		foreach (hstr, it, files)
		{
			hfile::copy(old_name + "/" + (*it), new_name + "/" + (*it));
		}
		return true;
	}
	
	bool Dir::create_path(chstr path)
	{
		hstr name = get_basedir(path);
		return (name != "" && name != "." && hdir::create(name));
	}
	
	void prepend_directory(chstr dirname, Array<hstr>& entries)
	{
		if (dirname != "")
		{
			foreach (hstr, it, entries)
			{
				(*it) = dirname + "/" + (*it);
			}
		}
	}
	
	Array<hstr> Dir::entries(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		Array<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				result += unicode_to_utf8(entry->d_name);
			}
			if (!result.contains("."))
			{
				result += hstr(".");
			}
			if (!result.contains(".."))
			{
				result += hstr("..");
			}
			_closedir(dir);
		}
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	Array<hstr> Dir::resource_entries(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		harray<hstr> result = Dir::resource_contents(dirname, false);
		result += hstr(".");
		result += hstr("..");
		if (prepend_dir)
		{
			prepend_directory(name, result);
		}
		return result;
	}
	
	Array<hstr> Dir::contents(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		Array<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				result += unicode_to_utf8(entry->d_name);
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
	
	Array<hstr> Dir::resource_contents(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
#ifndef HAVE_ZIPRESOURCE
		hstr full_path = Resource::make_full_path(name);
		Array<hstr> result = Dir::directories(full_path, false) + Dir::files(full_path, false);
		for_iter (i, 0, result.size())
		{
			result[i] = result[i].replace(Resource::getCwd() + "/", "");
		}
#else
		hstr current;
		Array<hstr> result;
		hstr cwd = Resource::getCwd();
		struct zip* archivefile = zip_open(Resource::getArchive().c_str(), 0, NULL);
		if (archivefile != NULL)
		{
			int count = zip_get_num_files(archivefile);
			int slashCount = 0;
			for_iter (i, 0, count)
			{
				current = systemize_path(hstr(zip_get_name(archivefile, i, 0)));
				if (_check_dir_prefix(current, cwd) && _check_dir_prefix(current, name))
				{
					slashCount = current.count("/");
					if (slashCount == 0) // directory // file
					{
						result += current;
					}
					else if (slashCount == 1) // directory
					{
						result += current(0, current.size() - 1);
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
	
	Array<hstr> Dir::directories(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		hstr current;
		Array<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				current = name + "/" + unicode_to_utf8(entry->d_name);
				if (hdir::exists(current))
				{
					result += unicode_to_utf8(entry->d_name);
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
	
	Array<hstr> Dir::resource_directories(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
#ifndef HAVE_ZIPRESOURCE
		Array<hstr> result = Dir::directories(Resource::make_full_path(name), false);
		for_iter (i, 0, result.size())
		{
			result[i] = result[i].replace(Resource::getCwd() + "/", "");
		}
#else
		hstr current;
		Array<hstr> result;
		hstr cwd = Resource::getCwd();
		struct zip* archivefile = zip_open(Resource::getArchive().c_str(), 0, NULL);
		if (archivefile != NULL)
		{
			int count = zip_get_num_files(archivefile);
			for_iter (i, 0, count)
			{
				current = systemize_path(hstr(zip_get_name(archivefile, i, 0)));
				if (_check_dir_prefix(current, cwd) && _check_dir_prefix(current, name) && current.count("/") == 1)
				{
					result += current(0, current.size() - 1);
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

	Array<hstr> Dir::files(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
		hstr current;
		Array<hstr> result;
		if (hdir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				current = name + "/" + unicode_to_utf8(entry->d_name);
				if (hfile::exists(current))
				{
					result += unicode_to_utf8(entry->d_name);
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

	Array<hstr> Dir::resource_files(chstr dirname, bool prepend_dir)
	{
		hstr name = normalize_path(dirname);
#ifndef HAVE_ZIPRESOURCE
		Array<hstr> result = Dir::files(Resource::make_full_path(name), false);
		for_iter (i, 0, result.size())
		{
			result[i] = result[i].replace(Resource::getCwd() + "/", "");
		}
#else
		hstr current;
		Array<hstr> result;
		hstr cwd = Resource::getCwd();
		struct zip* archivefile = zip_open(Resource::getArchive().c_str(), 0, NULL);
		if (archivefile != NULL)
		{
			int count = zip_get_num_files(archivefile);
			for_iter (i, 0, count)
			{
				current = systemize_path(hstr(zip_get_name(archivefile, i, 0)));
				if (_check_dir_prefix(current, cwd) && _check_dir_prefix(current, name) && current.count("/") == 0)
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

	void hdir::chdir(chstr dirname)
	{
		_chdir(dirname.c_str());
	}

	hstr hdir::cwd()
	{
#if !defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_PARTITION_DESKTOP
		char dir[FILENAME_MAX] = {'\0'};
		_getcwd(dir, FILENAME_MAX);
		return systemize_path(dir);
#else
		return winrtcwd;
#endif
	}

}
