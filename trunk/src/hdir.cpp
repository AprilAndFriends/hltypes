/// @file
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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

// prevents recursive calls of Dir::rename as this function is called via this pointer
int (*d_rename)(const char* old_name, const char* new_name) = rename;

#include "harray.h"
#include "hdir.h"
#include "hfile.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"

#ifdef _WIN32
#ifdef _WINRT
#define _chdir(name) winrtcwd = name
static hltypes::String winrtcwd = ".";
#elif defined(_MSC_VER)
#include <AccCtrl.h>
#endif
#endif

namespace hltypes
{
	bool Dir::win32FullDirectoryPermissions = true;

#if defined(_WIN32) && defined(_MSC_VER) && !defined(_WINRT) // god help us all
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

	static bool hmkdir(const String& path)
	{
#if defined(_WIN32) && defined(_MSC_VER) && !defined(_WINRT)
		if (Dir::getWin32FullDirectoryPermissions() && _mkdirWin32FullPermissions(path))
		{
			return true;
		}
#endif
#ifdef _WIN32
		return (_wmkdir(path.w_str().c_str()) != 0);
#else
		return (_mkdir(path.c_str()) != 0); // TODO - should be ported to Unix systems as well
#endif
	}
	
	static bool hremove(const String& dirname)
	{
#ifdef _WIN32
		return (_wremove(dirname.w_str().c_str()) != 0);
#else
		return (remove(dirname.c_str()) != 0); // TODO - should be ported to Unix systems as well
#endif
	}
	
	static bool hrmdir(const String& dirname)
	{
#ifdef _WIN32
		return (_wrmdir(dirname.w_str().c_str()) != 0);
#else
		return (_rmdir(dirname.c_str()) != 0); // TODO - should be ported to Unix systems as well
#endif
	}
	
	bool Dir::create(const String& dirname)
	{
		String name = Dir::normalize(dirname);
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
				path = Dir::join_path(path, (*it), false);
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
		String name = Dir::normalize(dirname);
		if (name == "" || !Dir::exists(name))
		{
			return false;
		}
		Array<String> directories = Dir::directories(name);
		foreach (String, it, directories)
		{
			Dir::remove(Dir::join_path(name, (*it), false));
		}
		Array<String> files = Dir::files(name);
		foreach (String, it, files)
		{
			File::remove(Dir::join_path(name, (*it), false));
		}
		hrmdir(name);
		return Dir::exists(name);
	}
	
	bool Dir::exists(const String& dirname, bool case_sensitive)
	{
		String name = Dir::normalize(dirname);
		bool result = false;
		DIR* dir = _opendir(name);
		if (dir != NULL)
		{
			_closedir(dir);
			result = true;
		}
		if (!result && !case_sensitive)
		{
			String basedir = Dir::basedir(name);
			String basename = Dir::basename(name);
			Array<String> directories = Dir::directories(basedir);
			foreach (String, it, directories)
			{
				if ((*it).lower() == basename.lower())
				{
					name = Dir::join_path(basedir, (*it));
					result = true;
					break;
				}
			}
		}
		return result;
	}
	
	bool Dir::clear(const String& dirname)
	{
		String name = Dir::normalize(dirname);
		if (name == "" || !Dir::exists(name))
		{
			return false;
		}
		Array<String> directories = Dir::directories(name);
		foreach (String, it, directories)
		{
			Dir::remove(Dir::join_path(name, (*it), false));
		}
		Array<String> files = Dir::files(name);
		foreach (String, it, files)
		{
			File::remove(Dir::join_path(name, (*it), false));
		}
		return (directories.size() > 0 || files.size() > 0);
	}
	
	bool Dir::rename(const String& old_dirname, const String& new_dirname)
	{
		String old_name = Dir::normalize(old_dirname);
		String new_name = Dir::normalize(new_dirname);
		if (!Dir::exists(old_name) || Dir::exists(new_name))
		{
			return false;
		}
		Dir::create(Dir::basedir(new_name));
		return (d_rename(old_name.c_str(), new_name.c_str()) == 0);
	}
	
	bool Dir::move(const String& dirname, const String& path)
	{
		String name = Dir::normalize(dirname);
		String path_name = Dir::normalize(path);
		return Dir::rename(name, Dir::join_path(path_name, Dir::basename(name), false));
	}
	
	bool Dir::copy(const String& old_dirname, const String& new_dirname)
	{
		String old_name = Dir::normalize(old_dirname);
		String new_name = Dir::normalize(new_dirname);
		if (!Dir::exists(old_name) || Dir::exists(new_name))
		{
			return false;
		}
		Dir::create(new_name);
		Array<String> directories = Dir::directories(old_name);
		foreach (String, it, directories)
		{
			Dir::copy(Dir::join_path(old_name, (*it), false), Dir::join_path(new_name, (*it), false));
		}
		Array<String> files = Dir::files(old_name);
		foreach (String, it, files)
		{
			File::copy(Dir::join_path(old_name, (*it), false), Dir::join_path(new_name, (*it), false));
		}
		return true;
	}
	
	Array<String> Dir::entries(const String& dirname, bool prepend_dir)
	{
		String name = Dir::normalize(dirname);
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
			Dir::_prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::contents(const String& dirname, bool prepend_dir)
	{
		String name = Dir::normalize(dirname);
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
			Dir::_prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::directories(const String& dirname, bool prepend_dir)
	{
		String name = Dir::normalize(dirname);
		String current;
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				current = Dir::join_path(name, String::from_unicode(entry->d_name), false);
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
			Dir::_prepend_directory(name, result);
		}
		return result;
	}
	
	Array<String> Dir::files(const String& dirname, bool prepend_dir)
	{
		String name = Dir::normalize(dirname);
		String current;
		Array<String> result;
		if (Dir::exists(name))
		{
			DIR* dir = _opendir(name);
			struct dirent* entry;
			while ((entry = _readdir(dir)))
			{
				current = Dir::join_path(name, String::from_unicode(entry->d_name), false);
				if (File::exists(current))
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
			Dir::_prepend_directory(name, result);
		}
		return result;
	}

	void Dir::chdir(const String& dirname)
	{
#ifdef _WIN32
#ifndef _WINRT
		_wchdir(Dir::systemize(dirname).w_str().c_str());
#endif
#else
		_chdir(Dir::systemize(dirname).c_str());
#endif
	}

	String Dir::cwd()
	{
#ifndef _WINRT
		char dir[FILENAME_MAX] = {'\0'};
		_getcwd(dir, FILENAME_MAX);
		return Dir::systemize(dir);
#else
		return winrtcwd;
#endif
	}

}
