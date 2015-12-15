/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides platform-specific definitions for special functionality.

#ifndef HLTYPES_PLATFORM_INTERNAL_H
#define HLTYPES_PLATFORM_INTERNAL_H

#include "hstring.h"

namespace hltypes
{
	void _platformPrint(const String& tag, const String& message, int level);
	int _platformSprintf(char* buffer, const char* format, ...);
	int _platformVsnprintf(char* buffer, size_t bufferCount, const char* format, va_list args);
	String _platformEnv(const String& name);
	String _platformResourceCwd();
	bool _platformRemoveDirectory(const String& dirName);
	bool _platformCreateDirectory(const String& dirName);

}

#endif