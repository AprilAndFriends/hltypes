/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _ANDROID
#include <android/log.h>
#endif
#include <stdarg.h>

#include "harray.h"
#include "hdir.h"
#include "hfile.h"
#include "hlog.h"
#include "hmutex.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

// required for Win32 only actually
#define MAX_FILE_SIZE 131072

#if defined(_ANDROID) && !defined(_DEBUG)
#define LEVEL_PLATFORM(level) ((level) == Log::LevelDebug ? Log::LevelWrite : (level))
#else
#define LEVEL_PLATFORM(level) (level)
#endif

#define MAKE_VA_ARGS(result, format) \
	String result; \
	{ \
		va_list args; \
		va_start(args, format); \
		result = String(hvsprintf(format, args)); \
		va_end(args); \
	}

namespace hltypes
{
#ifdef _ANDROID
	const int Log::LevelWrite = (int)ANDROID_LOG_INFO;
	const int Log::LevelError = (int)ANDROID_LOG_ERROR;
	const int Log::LevelWarn = (int)ANDROID_LOG_WARN;
	const int Log::LevelDebug = (int)ANDROID_LOG_DEBUG;
#else
	const int Log::LevelWrite = 4;
	const int Log::LevelError = 3;
	const int Log::LevelWarn = 2;
	const int Log::LevelDebug = 1;
#endif

	bool Log::levelWrite = true;
	bool Log::levelError = true;
	bool Log::levelWarn = true;
#ifndef _DEBUG
	bool Log::levelDebug = false;
#else
	bool Log::levelDebug = true;
#endif
	Array<String> Log::tagFilters;
	bool Log::outputEnabled = true;
	String Log::filename;
	void (*Log::callbackFunction)(const String&, const String&) = NULL;
	Mutex Log::mutex;
	int Log::fileIndex = 0;
	String Log::fileExtension;

	void (*Log::getCallbackFunction())(const String&, const String&)
	{
		hmutex::ScopeLock lock(&Log::mutex);
		return Log::callbackFunction;
	}
	
	void Log::setCallbackFunction(void(*function)(const String&, const String&))
	{
		hmutex::ScopeLock lock(&Log::mutex);
		Log::callbackFunction = function;
	}
	
	void Log::setLevels(bool write, bool error, bool warn, bool debug)
	{
		Log::levelWrite = write;
		Log::levelError = error;
		Log::levelWarn = warn;
		Log::levelDebug = debug;
	}
	
	void Log::setFilename(const String& filename, bool clearFile)
	{
		Log::filename = Dir::normalize(filename);
		Mutex::ScopeLock lock(&Log::mutex);
#ifdef _WIN32
		if (clearFile)
		{
			Dir::remove(filename + ".hlog"); // left over from last run, delete
			File::createNew(Log::filename);
		}
		else if (Dir::exists(filename + ".hlog"))
		{
			lock.release();
			Log::finalize(clearFile); // left over from last run, merge
		}
		Log::fileExtension = File::extensionOf(filename);
#else
		if (clearFile)
		{
			File::createNew(Log::filename);
		}
#endif
	}

	bool Log::write(const String& tag, const String& message)
	{
		return Log::_systemLog(tag, message, LevelWrite);
	}
	
	bool Log::error(const String& tag, const String& message)
	{
		return Log::_systemLog(tag, "ERROR: " + message, LevelError);
	}
	
	bool Log::warn(const String& tag, const String& message)
	{
		return Log::_systemLog(tag, "WARNING: " + message, LevelWarn);
	}
	
	bool Log::debug(const String& tag, const String& message)
	{
		return Log::_systemLog(tag, "DEBUG: " + message, LevelDebug);
	}
	
	bool Log::writef(const String& tag, const char* format, ...)
	{
		MAKE_VA_ARGS(result, format);
		return Log::write(tag, result);
	}
	
	bool Log::errorf(const String& tag, const char* format, ...)
	{
		MAKE_VA_ARGS(result, format);
		return Log::error(tag, result);
	}
	
	bool Log::warnf(const String& tag, const char* format, ...)
	{
		MAKE_VA_ARGS(result, format);
		return Log::warn(tag, result);
	}
	
	bool Log::debugf(const String& tag, const char* format, ...)
	{
		MAKE_VA_ARGS(result, format);
		return Log::debug(tag, result);
	}

	void Log::finalize(bool clearFile)
	{
#ifdef _WIN32
		Mutex::ScopeLock lock(&Log::mutex);
		if (Log::filename == "" || !File::exists(Log::filename))
		{
			return;
		}
		File file;
		String filename;
		if (clearFile)
		{
			file.open(Log::filename, File::AccessMode::Write);
		}
		else
		{
			file.open(Log::filename, File::AccessMode::Append);
		}
		for_iter (i, 0, Log::fileIndex + 1)
		{
			filename = Log::_makeFilename(Log::filename, i);
			if (File::exists(filename))
			{
				file.write(File::hread(filename));
			}
			else
			{
				file.write("\n[ERROR]\n\nLog segment file '" + filename + "' not found!!\n\n[-----]\n\n");
			}
		}
		file.close(); // to flush everything
		Dir::remove(Log::filename + ".hlog");
		Log::fileIndex = 0;
#endif
	}

	String Log::_makeFilename(const hltypes::String& filename, int index)
	{
		return (Dir::joinPath(filename + ".hlog", String(index) + "." + Log::fileExtension));
	}

	String Log::_makeCurrentFilename(const hltypes::String& filename)
	{
		if (!Dir::exists(filename + ".hlog"))
		{
			Dir::create(filename + ".hlog");
		}
		String newFilename = Log::_makeFilename(filename, Log::fileIndex);
		if (File::exists(newFilename) && File::hinfo(newFilename).size > MAX_FILE_SIZE)
		{
			++Log::fileIndex;
			newFilename = Log::_makeFilename(filename, Log::fileIndex);
			File::createNew(newFilename); // clears the file
		}
		return newFilename;
	}

	bool Log::_systemLog(const String& tag, const String& message, int level) // level is needed for Android
	{
		if (level == LevelWrite && !Log::levelWrite)
		{
			return false;
		}
		if (level == LevelError && !Log::levelError)
		{
			return false;
		}
		if (level == LevelWarn && !Log::levelWarn)
		{
			return false;
		}
		if (level == LevelDebug && !Log::levelDebug)
		{
			return false;
		}
		if (tag != "" && Log::tagFilters.size() > 0 && !Log::tagFilters.has(tag))
		{
			return false;
		}
		Mutex::ScopeLock lock(&Log::mutex);
		if (outputEnabled)
		{
			_platformPrint(tag, message, LEVEL_PLATFORM(level));
		}
		if (Log::filename != "")
		{
			try
			{
				File file;
#ifndef _WIN32
				file.open(Log::filename, File::AccessMode::Append);
#else
				file.open(Log::_makeCurrentFilename(Log::filename), File::AccessMode::Append);
#endif
				String logMessage = (tag != "" ? "[" + tag + "] " + message : message);
				file.write(logMessage);
				file.write("\n");
			}
			catch (_Exception& e)
			{
				if (outputEnabled)
				{
					_platformPrint("FATAL", e.getFullMessage(), LevelError);
				}
#ifdef _DEBUG
				throw e;
#endif
			}
		}
		try
		{
			if (Log::callbackFunction != NULL)
			{
				(*Log::callbackFunction)(tag, message);
			}
		}
		catch (_Exception& e)
		{
			if (outputEnabled)
			{
				_platformPrint("FATAL", e.getFullMessage(), LevelError);
			}
			throw e;
		}
		return true;
	}
	
}
