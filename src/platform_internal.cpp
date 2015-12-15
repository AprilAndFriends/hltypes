/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _WIN32
	#include <direct.h>
	#include "msvc_dirent.h"
#else
	#include <dirent.h>
	#include <sys/stat.h>
	#include <unistd.h>
	#define _chdir(name) ::chdir(name)
	#define _getcwd(buffer, size) ::getcwd(buffer, size)
	#define _opendir(name) opendir(name.cStr())
	#define _readdir(dirp) readdir(dirp)
	#define _closedir(dirp) closedir(dirp)
#endif

#define __HL_INCLUDE_PLATFORM_HEADERS
#include "harray.h"
#include "hdir.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

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
#if defined(_WIN32) && defined(_MSC_VER) && !defined(_WINRT) // god help us all
	static bool _mkdirWin32FullPermissions(const String& path)
	{
		typedef BOOL(WINAPI* TInitSD)(PSECURITY_DESCRIPTOR, DWORD);
		typedef BOOL(WINAPI* TAllocSid)(PSID_IDENTIFIER_AUTHORITY, BYTE, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PSID*);
		typedef DWORD(WINAPI* TSetEntAcl)(ULONG, PEXPLICIT_ACCESS, PACL, PACL*);
		typedef DWORD(WINAPI* TSetSDAcl)(PSECURITY_DESCRIPTOR, BOOL, PACL, BOOL);
		typedef PVOID(WINAPI* TFreeSid)(PSID);
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
					result = (CreateDirectory(path.wStr().c_str(), &sa) == TRUE);
				}
				LocalFree(acl);
			}
			pFreeSid(pEveryoneSid);
		}
		return result;
	}
#endif

	int _platformSprintf(char* buffer, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		int result = vsprintf(buffer, format, args);
		va_end(args);
		return result;
	}

	int _platformVsnprintf(char* buffer, size_t bufferCount, const char* format, va_list args)
	{
		return vsnprintf(buffer, bufferCount, format, args);
	}

	String _platformEnv(const String& name)
	{
#ifdef _WIN32
#ifndef _WINRT
		return String::fromUnicode(_wgetenv(name.wStr().c_str()));
#else
		return ""; // WinRT does not support environment variables
#endif
#else
		return String(getenv(name.cStr()));
#endif
	}

	String _platformResourceCwd()
	{
		return ".";
	}

	bool _platformRemoveDirectory(const String& dirName)
	{
#ifdef _WIN32
		return (_wrmdir(dirName.wStr().c_str()) != 0);
#else
		return (::rmdir(dirName.cStr()) != 0); // TODO - should be ported to Unix systems as well
#endif
	}

	bool _platformCreateDirectory(const String& dirName)
	{
#ifdef _WIN32
#if defined(_MSC_VER) && !defined(_WINRT)
		if (Dir::getWin32FullDirectoryPermissions() && _mkdirWin32FullPermissions(dirName))
		{
			return true;
		}
#endif
		return (_wmkdir(dirName.wStr().c_str()) != 0);
#else
		return (::mkdir(dirName.cStr(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0); // TODO - should be ported to Unix systems as well
#endif
	}

}
