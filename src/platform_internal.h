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
	typedef void _platformDir;
	typedef void _platformDirEntry;

	void _platformPrint(const String& tag, const String& message, int level);
	int _platformSprintf(char* buffer, const char* format, ...);
	int _platformVsnprintf(char* buffer, size_t bufferCount, const char* format, va_list args);

	String _platformEnv(const String& name);
	String _platformResourceCwd();

	bool _platformRemoveDirectory(const String& dirName);
	bool _platformCreateDirectory(const String& dirName);
	bool _platformRenameDirectory(const String& dirName, const String& newName);
	_platformDir* _platformOpenDirectory(const String& dirName);
	_platformDirEntry* _platformReadDirectory(_platformDir* dir);
	String _platformGetDirEntryName(_platformDirEntry* entry);
	void _platformCloseDirectory(_platformDir* dir);

	void _platformChdir(const String& dirName);
	String _platformCwd();

}

#endif