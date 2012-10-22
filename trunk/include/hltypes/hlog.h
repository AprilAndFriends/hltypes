/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Provides high level logging.

#ifndef HLTYPES_LOG_H
#define HLTYPES_LOG_H

#include "harray.h"
#include "hstring.h"
#include "hltypesExport.h"

namespace hltypes
{
	/// @brief Provides high level logging.
	/// @author Boris Mikic
	class hltypesExport Log
	{
	public:
		static void setLevelWrite(bool value) { level_write = value; }
		static void setLevelError(bool value) { level_error = value; }
		static void setLevelWarn(bool value) { level_warn = value; }
		static void setLevelDebug(bool value) { level_debug = value; }
		static void setTagFilters(harray<hstr> value) { tag_filters = value; }
		static void setLevels(bool write, bool error, bool warn, bool debug);
		static void setFilename(chstr filename, bool clearFile = true);

		/// @brief Logs a message on the log level Write.
		/// @param[in] message The message to log.
		/// @param[in] tag The message tag.
		/// @return True if level Write and tag allowed.
		static bool write(chstr tag, chstr message);
		/// @brief Logs a message on the log level Error.
		/// @param[in] message The message to log.
		/// @param[in] tag The message tag.
		/// @return True if level Error and tag allowed.
		static bool error(chstr tag, chstr message);
		/// @brief Logs a message on the log level Warn.
		/// @param[in] message The message to log.
		/// @param[in] tag The message tag.
		/// @return True if level Warn and tag allowed.
		static bool warn(chstr tag, chstr message);
		/// @brief Logs a message on the log level Debug.
		/// @param[in] message The message to log.
		/// @param[in] tag The message tag.
		/// @return True if level Debug and tag allowed.
		static bool debug(chstr tag, chstr message);
		/// @brief Same as write, except with string formatting.
		/// @see write
		static bool writef(chstr tag, const char* format, ...);
		/// @brief Same as error, except with string formatting.
		/// @see error
		static bool errorf(chstr tag, const char* format, ...);
		/// @brief Same as warn, except with string formatting.
		/// @see warn
		static bool warnf(chstr tag, const char* format, ...);
		/// @brief Same as debug, except with string formatting.
		/// @see debug
		static bool debugf(chstr tag, const char* format, ...);

	protected:
		/// @brief Flag for Write level logging.
		static bool level_write;
		/// @brief Flag for Error level logging.
		static bool level_error;
		/// @brief Flag for Warn level logging.
		static bool level_warn;
		/// @brief Flag for Debug level logging.
		static bool level_debug;
		/// @brief Filters for tags that should be logged.
		static harray<hstr> tag_filters;
		/// @brief Filename for logging to files.
		static hstr filename;

		/// @brief Executes the actual message loggging.
		/// @param[in] message The message to log.
		/// @param[in] tag The message tag.
		/// @param[in] level Log level (required for Android).
		/// @return True if the message could be logged.
		static bool _platform_log(chstr message, chstr tag, int level);

	};
}

/// @brief Alias for simpler code.
typedef hltypes::Log hlog;

#endif

