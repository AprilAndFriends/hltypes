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
	#define _mkdir(name) ::mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
	#define _rmdir(name) ::rmdir(name)
	#define _chdir(name) ::chdir(name)
	#define _getcwd(buffer, size) ::getcwd(buffer, size)
	#define _opendir(name) opendir(name.cStr())
	#define _readdir(dirp) readdir(dirp)
	#define _closedir(dirp) closedir(dirp)
#endif

#define __HL_INCLUDE_PLATFORM_HEADERS
#include "harray.h"
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

}
