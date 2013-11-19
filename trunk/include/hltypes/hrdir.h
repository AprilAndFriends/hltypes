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
/// Provides high level resource directory handling.

#ifndef HLTYPES_RESOURCE_DIR_H
#define HLTYPES_RESOURCE_DIR_H

#include "hstring.h"
#include "hdbase.h"
#include "hltypesExport.h"

namespace hltypes
{
	template <class T> class Array;
	/// @brief Provides high level directory handling.
	/// @author Boris Mikic
	/// @author Kresimir Spes
	class hltypesExport ResourceDir : public DirBase
	{
	public:
		/// @brief Checks if a resource directory exists.
		/// @param[in] dirname Name of the resource directory.
		/// @return True if resource directory exists.
		static bool exists(const String& dirname);
		/// @brief Gets all resource directory entries in the given resource directory.
		/// @param[in] dirname Name of the resource directory.
		/// @param[in] prepend_dir Whether the same parent path should be appended to the resource entries.
		/// @return Array of all resource directory entries.
		/// @note Entries include "." and "..".
		static Array<String> entries(const String& dirname, bool prepend_dir = false);
		/// @brief Gets all physical resource directory contents in the given resource directory.
		/// @param[in] dirname Name of the resource directory.
		/// @param[in] prepend_dir Whether the same parent path should be appended to the resource contents.
		/// @return Array of all resource directory contents.
		/// @note Contents do not include "." and "..".
		static Array<String> contents(const String& dirname, bool prepend_dir = false);
		/// @brief Gets all resource directories in the given directory.
		/// @param[in] dirname Name of the resource directory.
		/// @param[in] prepend_dir Whether the same parent path should be appended to the resource directory paths.
		/// @return Array of all resource directories.
		static Array<String> directories(const String& dirname, bool prepend_dir = false);
		/// @brief Gets all resource files in the given directory.
		/// @param[in] dirname Name of the directory.
		/// @param[in] prepend_dir Whether the same parent path should be appended to the file paths.
		/// @return Array of all files.
		static Array<String> files(const String& dirname, bool prepend_dir = false);

	protected:
		/// @brief Basic constructor.
		/// @note Forces this to be a static class.
		ResourceDir() : DirBase() { }
		/// @brief Basic constructor.
		/// @note Forces this to be a static class.
		~ResourceDir() { }

		/// @brief Checks if the prefix matches the resource path and removes it.
		/// @param[in,out] path The resource path which gets modified.
		/// @param[in] prefix Prefix to check.
		/// @return True if prefix matches.
		static bool _check_dir_prefix(String& path, const String& prefix);
		/// @brief Removes CWD from resource paths.
		/// @param[in] paths The resource paths.
		/// @return Resource paths without the CWD.
		static Array<String> _remove_cwd(Array<String> paths);

	};
}

/// @brief Alias for simpler code.
typedef hltypes::ResourceDir hrdir;

#endif

