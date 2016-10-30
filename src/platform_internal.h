/// @file
/// @version 4.0
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

#include "hfile.h"
#include "hstring.h"

#define CLIPBOARD_ERROR "Clipboard operations are not supported on this platform!"

namespace hltypes
{
	typedef void _platformDir;
	typedef void _platformDirEntry;
	typedef void _platformFile;

	void _platformPrint(const String& tag, const String& message, int level);
	bool _platformClipboardClear();
	bool _platformClipboardIsString();
	bool _platformClipboardGetString(hstr& string);
	bool _platformClipboardSetString(chstr string);

	int _platformSprintf(char* buffer, const char* format, ...);
	int _platformVsnprintf(char* buffer, size_t bufferCount, const char* format, va_list args);

	String _platformEnv(const String& name);
	String _platformResourceCwd();

	_platformFile* _platformOpenFile(const String& name, const String& accessMode);
	void _platformCloseFile(_platformFile* file);
	int _platformReadFile(void* buffer, int elementSize, int elementCount, _platformFile* file);
	int _platformWriteFile(const void* buffer, int elementSize, int elementCount, _platformFile* file);
	int64_t _platformGetFilePosition(_platformFile* file);
	bool _platformSeekFile(_platformFile* file, int64_t size, int64_t position, int64_t offset, FileBase::SeekMode seekMode);
	bool _platformFileExists(const String& name);
	bool _platformRenameFile(const String& oldName, const String& newName);
	bool _platformRemoveFile(const String& name);
	FileInfo _platformStatFile(const String& name);

	_platformDir* _platformOpenDirectory(const String& dirName);
	void _platformCloseDirectory(_platformDir* dir);
	_platformDirEntry* _platformReadDirectory(_platformDir* dir);
	String _platformGetDirEntryName(_platformDirEntry* entry);
	bool _platformDirectoryExists(const String& dirName);
	bool _platformCreateDirectory(const String& dirName);
	bool _platformRenameDirectory(const String& oldName, const String& newName);
	bool _platformRemoveDirectory(const String& dirName);

	void _platformChdir(const String& dirName);
	String _platformCwd();

}

#endif