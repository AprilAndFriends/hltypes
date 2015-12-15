/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#define __HL_INCLUDE_PLATFORM_HEADERS
#include "harray.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

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

}
