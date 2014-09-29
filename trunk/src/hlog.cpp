/// @file
/// @version 2.4
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
	static hmutex log_mutex;
	
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

#ifdef _WIN32
	static int fileIndex = 0;
	static hstr fileExtension;

	hstr _get_file_name(chstr filename, int index)
	{
		return (filename + ".hlog/" + hstr(index) + "." + fileExtension);
	}

	hstr _get_current_file_name(chstr filename)
	{
		log_mutex.lock();
		if (!hdir::exists(filename + ".hlog"))
		{
			hdir::create(filename + ".hlog");
		}
		hstr newFilename = _get_file_name(filename, fileIndex);
		if (hfile::hsize(newFilename) > MAX_FILE_SIZE)
		{
			++fileIndex;
			newFilename = _get_file_name(filename, fileIndex);
			hfile::create_new(newFilename); // clears the file
		}
		log_mutex.unlock();
		return newFilename;
	}
#endif
	
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
#ifdef _WIN32
		log_mutex.lock();
		if (clearFile)
		{
			hdir::remove(filename + ".hlog"); // left over from last run, delete
			hfile::create_new(Log::filename);
			log_mutex.unlock();
		}
		else if (hdir::exists(filename + ".hlog"))
		{
			log_mutex.unlock();
			Log::finalize(clearFile); // left over from last run, merge
		}
		else
		{
			log_mutex.unlock();
		}
		fileExtension = hfile::extension_of(filename);
#else
		log_mutex.lock();
		if (clearFile)
		{
			hfile::create_new(Log::filename);
		}
		log_mutex.unlock();
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
		log_mutex.lock();
		_platform_print(tag, message, LEVEL_PLATFORM(level));
		if (Log::filename != "")
		{
			try
			{
#ifndef _WIN32
				hfile file(Log::filename, hfile::APPEND);
#else
				hfile file(_get_current_file_name(Log::filename), hfile::APPEND);
#endif
				String log_message = (tag != "" ? "[" + tag + "] " + message : message);
				file.writef("%s\n", log_message.c_str());
			}
			catch (hltypes::exception& e)
			{
				_platform_print("FATAL", e.getMessage(), LevelError);
				log_mutex.unlock();
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
			log_mutex.unlock();
			throw e;
		}
		log_mutex.unlock();
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
		if (Log::filename == "" || !hfile::exists(Log::filename))
		{
			return;
		}
		hfile file;
		hstr filename;
		log_mutex.lock();
		if (clearFile)
		{
			file.open(Log::filename, File::WRITE);
		}
		else
		{
			file.open(Log::filename, File::APPEND);
		}
		for_iter (i, 0, fileIndex + 1)
		{
			filename = _get_file_name(Log::filename, i);
			if (hfile::exists(filename))
			{
				file.write(hfile::hread(filename));
			}
			else
			{
				file.write("\n[ERROR]\n\nLog segment file '" + filename + "' not found!!\n\n[-----]\n\n");
			}
		}
		file.close(); // to flush everything
		hdir::remove(Log::filename + ".hlog");
		log_mutex.unlock();
		fileIndex = 0;
#endif
	}

}
