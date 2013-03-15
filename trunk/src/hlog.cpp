/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _ANDROID
#include <android/log.h>
#endif
#include <stdarg.h>

#include "harray.h"
#include "hfile.h"
#include "hlog.h"
#include "hmutex.h"
#include "hplatform.h"
#include "hstring.h"

#ifdef _ANDROID
#define LEVEL_WRITE ((int)ANDROID_LOG_INFO)
#define LEVEL_ERROR ((int)ANDROID_LOG_ERROR)
#define LEVEL_WARN ((int)ANDROID_LOG_WARN)
#define LEVEL_DEBUG ((int)ANDROID_LOG_DEBUG)
#ifndef _DEBUG
#define LEVEL_PLATFORM(level) ((level) == LEVEL_DEBUG ? LEVEL_WRITE : (level))
#else
#define LEVEL_PLATFORM(level) (level)
#endif
#else
#define LEVEL_WRITE 4
#define LEVEL_ERROR 3
#define LEVEL_WARN 2
#define LEVEL_DEBUG 1
#define LEVEL_PLATFORM(level) (level)
#endif

#if defined(__APPLE__)
    void nsLog(const hltypes::String& message); // defined in Mac_platform.mm and iOS_platform.mm
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
	hmutex log_mutex;

	bool Log::level_write = true;
	bool Log::level_error = true;
	bool Log::level_warn = true;
#ifndef _DEBUG
	bool Log::level_debug = false;
#else
	bool Log::level_debug = true;
#endif
	Array<String> Log::tag_filters;
	String Log::filename;
	void (*Log::callback_function)(const String&, const String&) = NULL;

	void Log::setLevels(bool write, bool error, bool warn, bool debug)
	{
		Log::level_write = write;
		Log::level_error = error;
		Log::level_warn = warn;
		Log::level_debug = debug;
	}

	void Log::setFilename(const String& filename, bool clearFile)
	{
		Log::filename = filename;
		if (clearFile)
		{
			hfile::create_new(Log::filename);
		}
	}

	bool Log::_system_log(const String& tag, const String& message, int level) // level is needed for Android
	{
		if (level == LEVEL_WRITE && !Log::level_write)
		{
			return false;
		}
		if (level == LEVEL_ERROR && !Log::level_error)
		{
			return false;
		}
		if (level == LEVEL_WARN && !Log::level_warn)
		{
			return false;
		}
		if (level == LEVEL_DEBUG && !Log::level_debug)
		{
			return false;
		}
		if (tag != "" && Log::tag_filters.size() > 0 && !Log::tag_filters.contains(tag))
		{
			return false;
		}
		log_mutex.lock();
		_platform_print(tag, message, LEVEL_PLATFORM(level));
		if (Log::filename != "")
		{
			hfile file(Log::filename, hfile::APPEND);
			String log_message = (tag != "" ? "[" + tag + "] " + message : message);
			file.writef("%s\n", log_message.c_str());
		}
		if (Log::callback_function != NULL)
		{
			(*Log::callback_function)(tag, message);
		}
		log_mutex.unlock();
		return true;
	}

	bool Log::write(const String& tag, const String& message)
	{
		return Log::_system_log(tag, message, LEVEL_WRITE);
	}

	bool Log::error(const String& tag, const String& message)
	{
		return Log::_system_log(tag, "ERROR: " + message, LEVEL_ERROR);
	}

	bool Log::warn(const String& tag, const String& message)
	{
		return Log::_system_log(tag, "WARNING: " + message, LEVEL_WARN);
	}

	bool Log::debug(const String& tag, const String& message)
	{
		return Log::_system_log(tag, "DEBUG: " + message, LEVEL_DEBUG);
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

}
