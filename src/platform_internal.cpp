/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>

#ifdef _WIN32
	#include <direct.h>
	#include "msvc_dirent.h"
	#define WINDOWS_TICK 10000000ULL
	#define SEC_TO_UNIX_EPOCH 11644473600ULL
#else
	#include <stdlib.h>
	#include <dirent.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
	#if defined(_ANDROID) || defined(__APPLE__)
		#include <errno.h>
	#endif
#endif

#define __HL_INCLUDE_PLATFORM_HEADERS
#include "harray.h"
#include "hdir.h"
#include "hfbase.h"
#include "hlog.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

#if defined(_WIN32) && defined(_MSC_VER) && !defined(_WINRT)
	#define _WIN32_MKDIR_FULL_PERMISSIONS
#endif
#ifdef _WIN32_MKDIR_FULL_PERMISSIONS
	#include <AccCtrl.h>
#endif

namespace hltypes
{
#ifdef _WINRT
	static hltypes::String winrtcwd = ".";
#endif

	// god help us all
#ifdef _WIN32_MKDIR_FULL_PERMISSIONS
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

	_platformFile* _platformOpenFile(const String& name, const String& accessMode)
	{
		errno = 0;
#ifdef _WIN32
		return (_platformFile*)_wfopen(name.wStr().c_str(), accessMode.wStr().c_str());
#else
		return (_platformFile*)fopen(name.cStr(), accessMode.cStr()); // TODO - should be ported to Unix systems as well
#endif
	}

	void _platformCloseFile(_platformFile* file)
	{
		fclose((FILE*)file);
	}

	int _platformReadFile(void* buffer, int elementSize, int elementCount, _platformFile* file)
	{
		return (int)fread(buffer, elementSize, elementCount, (FILE*)file);
	}

	int _platformWriteFile(const void* buffer, int elementSize, int elementCount, _platformFile* file)
	{
		return (int)fwrite(buffer, elementSize, elementCount, (FILE*)file);
	}

	int64_t _platformGetFilePosition(_platformFile* file)
	{
		fpos_t position = 0;
		if (fgetpos((FILE*)file, &position) != 0)
		{
			return -1;
		}
		return (int64_t)position;
	}

	bool _platformSeekFile(_platformFile* file, int64_t size, int64_t position, int64_t offset, FileBase::SeekMode seekMode)
	{
		fpos_t _position = position;
		switch (seekMode)
		{
		case FileBase::CURRENT:
			break;
		case FileBase::START:
			_position = 0;
			break;
		case FileBase::END:
			_position = size;
			break;
		}
		_position += offset;
		return (fsetpos((FILE*)file, &_position) == 0);
	}

	bool _platformFileExists(const String& name)
	{
#ifdef _WIN32
		// benchmarked, fastest way to check for file existence
		WIN32_FILE_ATTRIBUTE_DATA data;
		memset(&data, 0, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
		return (GetFileAttributesExW(name.wStr().c_str(), GetFileExInfoStandard, &data) != 0 && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
		struct stat stats;
		return (stat(name.cStr(), &stats) == 0 && (stats.st_mode & S_IFREG) != 0);
#endif
	}

	bool _platformRenameFile(const String& oldName, const String& newName)
	{
#ifdef _WIN32
		return (_wrename(oldName.wStr().c_str(), newName.wStr().c_str()) == 0);
#else
		return (rename(oldName.cStr(), newName.cStr()) == 0); // TODO - should be ported to Unix systems as well
#endif
	}

	bool _platformRemoveFile(const String& name)
	{
#ifdef _WIN32
		return (_wremove(name.wStr().c_str()) == 0);
#else
		return (remove(name.cStr()) == 0); // TODO - should be ported to Unix systems as well
#endif
	}

	FileInfo _platformStatFile(const String& name)
	{
		FileInfo info;
#ifdef _WIN32
		WIN32_FILE_ATTRIBUTE_DATA data;
		memset(&data, 0, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
		if (GetFileAttributesExW(name.wStr().c_str(), GetFileExInfoStandard, &data) != 0)
		{
			info.size = (int64_t)data.nFileSizeLow;
			ULARGE_INTEGER ull;
			ull.LowPart = data.ftCreationTime.dwLowDateTime;
			ull.HighPart = data.ftCreationTime.dwHighDateTime;
			info.creationTime = (int64_t)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
			ull.LowPart = data.ftLastAccessTime.dwLowDateTime;
			ull.HighPart = data.ftLastAccessTime.dwHighDateTime;
			info.accessTime = (int64_t)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
			ull.LowPart = data.ftLastWriteTime.dwLowDateTime;
			ull.HighPart = data.ftLastWriteTime.dwHighDateTime;
			info.modificationTime = (int64_t)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
		}
#else
		struct stat s;
		if (stat(name.cStr(), &s) != 0)
		{
			FILE* file = fopen(name.cStr(), "rb");
			if (file != NULL)
			{
				fclose(file);
				throw FileCouldNotOpenException("stat() failed on '" + name + "'!");
			}
			throw FileCouldNotOpenException("stat() failed on '" + name + "', file not found!");
		}
		info.size = (int64_t)s.st_size;
		info.creationTime = (int64_t)s.st_ctime;
		info.accessTime = (int64_t)s.st_atime;
		info.modificationTime = (int64_t)s.st_mtime;
#endif
		return info;
	}

	_platformDir* _platformOpenDirectory(const String& dirName)
	{
#ifdef _WIN32
		return (_platformDir*)_opendir(dirName);
#else
		return (_platformDir*)opendir(dirName.cStr());
#endif
	}

	void _platformCloseDirectory(_platformDir* dir)
	{
#ifdef _WIN32
		_closedir((DIR*)dir);
#else
		closedir((DIR*)dir);
#endif
	}

	_platformDirEntry* _platformReadDirectory(_platformDir* dir)
	{
#ifdef _WIN32
		return (_platformDirEntry*)_readdir((DIR*)dir);
#else
		return (_platformDirEntry*)readdir((DIR*)dir);
#endif
	}

	String _platformGetDirEntryName(_platformDirEntry* entry)
	{
		return String::fromUnicode(((struct dirent*)entry)->d_name);
	}

	bool _platformDirectoryExists(const String& dirName)
	{
#ifdef _WIN32
		// benchmarked, fastest way to check for directory existence
		WIN32_FILE_ATTRIBUTE_DATA data;
		memset(&data, 0, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
		return (GetFileAttributesExW(dirName.wStr().c_str(), GetFileExInfoStandard, &data) != 0 && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
		struct stat stats;
		return (stat(dirName.cStr(), &stats) == 0 && (stats.st_mode & S_IFDIR) != 0);
#endif
	}

	bool _platformCreateDirectory(const String& dirName)
	{
#ifdef _WIN32
#if defined(_MSC_VER) && !defined(_WINRT)
		if (Dir::isWin32FullDirectoryPermissions() && _mkdirWin32FullPermissions(dirName))
		{
			return true;
		}
#endif
		return (_wmkdir(dirName.wStr().c_str()) != 0);
#else
		return (mkdir(dirName.cStr(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0); // TODO - should be ported to Unix systems as well
#endif
	}

	bool _platformRenameDirectory(const String& oldName, const String& newName)
	{
#ifdef _WIN32
		return (_wrename(oldName.wStr().c_str(), newName.wStr().c_str()) == 0);
#else
		return (rename(oldName.cStr(), newName.cStr()) == 0); // TODO - should be ported to Unix systems as well
#endif
	}

	bool _platformRemoveDirectory(const String& dirName)
	{
#ifdef _WIN32
		return (_wrmdir(dirName.wStr().c_str()) != 0);
#else
		return (rmdir(dirName.cStr()) != 0); // TODO - should be ported to Unix systems as well
#endif
	}

	void _platformChdir(const String& dirName)
	{
#ifdef _WIN32
#ifndef _WINRT
		_wchdir(dirName.wStr().c_str());
#else
		winrtcwd = dirName;
#endif
#else
		chdir(dirName.cStr());
#endif
	}

	String _platformCwd()
	{
#ifdef _WIN32
#ifndef _WINRT
		char dir[FILENAME_MAX + 1] = { '\0' };
		_getcwd(dir, FILENAME_MAX);
		return Dir::systemize(dir);
#else
		return winrtcwd;
#endif
#else
		char dir[FILENAME_MAX + 1] = { '\0' };
		getcwd(dir, FILENAME_MAX - 1);
		return Dir::systemize(dir);
#endif
	}

}
