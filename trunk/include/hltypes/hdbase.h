/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Provides high level directory handling.

#ifndef HLTYPES_DIR_BASE_H
#define HLTYPES_DIR_BASE_H

#include "harray.h"
#include "hstring.h"
#include "hltypesExport.h"

namespace hltypes
{
	template <class T> class Array;
	/// @brief Provides high level directory handling.
	/// @author Boris Mikic
	/// @author Kresimir Spes
	class hltypesExport DirBase
	{
	public:
		/// @brief Gets the base directory of a filename/directory.
		/// @param[in] path The path.
		/// @return Base directory of the given filename/directory.
		static String basedir(const String& path);
		/// @brief Gets the base filename/directory without the prepended directory path.
		/// @param[in] path The path.
		/// @return Base filename/directory without the prepended directory path.
		static String basename(const String& path);
		/// @brief Changes all platform-specific directory separators to / and removal of duplicate /.
		/// @param[in] path The path.
		/// @return Path with all platform-specific directory separators changed to /.
		static String systemize(const String& path);
		/// @brief Normalizes a file path by converting all platform-specific directory separators into /, removal of duplicate / and proper removal of "." and ".." where necessary.
		/// @param[in] path The path.
		/// @return Normalized path.
		/// @note Calls Dir::systemize() internally.
		static String normalize(const String& path);

	protected:
		/// @brief Basic constructor.
		/// @note Forces this to be a static class.
		DirBase() { }
		/// @brief Basic constructor.
		/// @note Forces this to be a static class.
		virtual ~DirBase() { }

		/// @brief Prepends the directory name to each path entry.
		/// @param[in] dirname Directory name.
		/// @param[in] entries Directory entries.
		static void _prepend_directory(const String& dirname, Array<String>& entries);
	
	};
}

/// @brief Alias for simpler code.
typedef hltypes::DirBase hdbase;

#endif

