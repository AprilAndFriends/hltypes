/// @file
/// @version 2.5
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
	int Log::LevelWrite = (int)ANDROID_LOG_INFO;
	int Log::LevelError = (int)ANDROID_LOG_ERROR;
	int Log::LevelWarn = (int)ANDROID_LOG_WARN;
	int Log::LevelDebug = (int)ANDROID_LOG_DEBUG;
#else
	int Log::LevelWrite = 4;
	int Log::LevelError = 3;
	int Log::LevelWarn = 2;
	int Log::LevelDebug = 1;
#endif

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
	Mutex Log::mutex;
	int Log::file_index = 0;
	String Log::file_extension;

	String Log::_get_file_name(const hltypes::String& filename, int index)
	{
		return (filename + ".hlog/" + String(index) + "." + Log::file_extension);
	}

	String Log::_get_current_file_name(const hltypes::String& filename)
	{
		if (!Dir::exists(filename + ".hlog"))
		{
			Dir::create(filename + ".hlog");
		}
		String newFilename = Log::_get_file_name(filename, Log::file_index);
		if (File::hsize(newFilename) > MAX_FILE_SIZE)
		{
			++Log::file_index;
			newFilename = Log::_get_file_name(filename, Log::file_index);
			File::create_new(newFilename); // clears the file
		}
		return newFilename;
	}
	
	void Log::setLevels(bool write, bool error, bool warn, bool debug)
	{
		Log::level_write = write;
		Log::level_error = error;
		Log::level_warn = warn;
		Log::level_debug = debug;
	}
	
	void Log::setFilename(const String& filename, bool clearFile)
	{
		Log::filename = Dir::normalize(filename);
		Mutex::ScopeLock lock(&Log::mutex);
#ifdef _WIN32
		if (clearFile)
		{
			Dir::remove(filename + ".hlog"); // left over from last run, delete
			File::create_new(Log::filename);
			lock.release();
		}
		else if (Dir::exists(filename + ".hlog"))
		{
			lock.release();
			Log::finalize(clearFile); // left over from last run, merge
		}
		else
		{
			lock.release();
		}
		Log::file_extension = File::extension_of(filename);
#else
		if (clearFile)
		{
			File::create_new(Log::filename);
		}
		lock.release();
#endif
	}
	
	bool Log::_system_log(const String& tag, const String& message, int level) // level is needed for Android
	{
		if (level == LevelWrite && !Log::level_write)
		{
			return false;
		}
		if (level == LevelError && !Log::level_error)
		{
			return false;
		}
		if (level == LevelWarn && !Log::level_warn)
		{
			return false;
		}
		if (level == LevelDebug && !Log::level_debug)
		{
			return false;
		}
		if (tag != "" && Log::tag_filters.size() > 0 && !Log::tag_filters.contains(tag))
		{
			return false;
		}
		Log::mutex.lock();
		_platform_print(tag, message, LEVEL_PLATFORM(level));
		if (Log::filename != "")
		{
			try
			{
#ifndef _WIN32
				File file(Log::filename, File::APPEND);
#else
				File file(Log::_get_current_file_name(Log::filename), File::APPEND);
#endif
				String log_message = (tag != "" ? "[" + tag + "] " + message : message);
				file.writef("%s\n", log_message.c_str());
			}
			catch (hltypes::exception& e)
			{
				_platform_print("FATAL", e.getMessage(), LevelError);
				Log::mutex.unlock();
				throw e;
			}
		}
		try
		{
			if (Log::callback_function != NULL)
			{
				(*Log::callback_function)(tag, message);
			}
		}
		catch (hltypes::exception& e)
		{
			_platform_print("FATAL", e.getMessage(), LevelError);
			Log::mutex.unlock();
			throw e;
		}
		Log::mutex.unlock();
		return true;
	}
	
	bool Log::write(const String& tag, const String& message)
	{
		return Log::_system_log(tag, message, LevelWrite);
	}
	
	bool Log::error(const String& tag, const String& message)
	{
		return Log::_system_log(tag, "ERROR: " + message, LevelError);
	}
	
	bool Log::warn(const String& tag, const String& message)
	{
		return Log::_system_log(tag, "WARNING: " + message, LevelWarn);
	}
	
	bool Log::debug(const String& tag, const String& message)
	{
		return Log::_system_log(tag, "DEBUG: " + message, LevelDebug);
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
		if (Log::filename == "" || !File::exists(Log::filename))
		{
			return;
		}
		File file;
		String filename;
		Log::mutex.lock();
		if (clearFile)
		{
			file.open(Log::filename, File::WRITE);
		}
		else
		{
			file.open(Log::filename, File::APPEND);
		}
		for_iter (i, 0, Log::file_index + 1)
		{
			filename = Log::_get_file_name(Log::filename, i);
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
		Log::mutex.unlock();
		Log::file_index = 0;
#endif
	}

}
