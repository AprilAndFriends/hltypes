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
/// Provides high level resource file handling.

#ifndef HLTYPES_RESOURCE_H
#define HLTYPES_RESOURCE_H

#include "hfbase.h"
#include "hmap.h"
#include "hstring.h"

#include "hltypesExport.h"

namespace hltypes
{
	class ResourceDir;

	/// @brief Provides high level resource file handling.
	/// @note When writing, \\r may be used, but \\r will be removed during read.
	class hltypesExport Resource : public FileBase
	{
	public:
		friend class ResourceDir;

		/// @brief Basic constructor.
		Resource();
		/// @brief Destructor.
		~Resource();
		/// @brief Opens a resource file.
		/// @param[in] filename Name of the resource file (may include path).
		/// @note If this instance is already working with an opened resource file handle, that resource file handle will be closed.
		void open(const String& filename);
		/// @brief Closes resource file.
		void close();
		
		/// @brief Checks if a resource file exists.
		/// @param[in] filename Name of the resource file.
		/// @param[in] caseSensitive Whether to check case sensitive files if file was not found.
		/// @return True if resource file exists.
		/// @note Disabling caseSensitive is somewhat costly if the given file is not found at first.
		static bool exists(const String& filename, bool caseSensitive = true);
		/// @brief Opens file, reads data and closes file.
		/// @see read(int count)
		static String hread(const String& filename, int count);
		/// @brief Opens file, reads data and closes file.
		/// @see read
		static String hread(const String& filename, const String& delimiter = "");
		/// @brief Gets the file information provided by the implementation.
		/// @param[in] filename The filename of the file.
		/// @return File information provided by the implementation.
		static FileInfo hinfo(const String& filename);
		
		/// @brief Gets the currently mounted ZIP archives.
		/// @return Resource archive's filename.
		static inline Map<String, String> getMountedArchives() { return mountedArchives; }
		/// @brief Checks if compiled with ZIP support.
		/// @return True if compiled with ZIP support.
		static bool hasZip();

		/// @brief Mounts ZIP archive or directory for resource supply.
		/// @param[in] path The mounting path.
		/// @param[in] archiveFilename The ZIP archive filename or a plain directory path.
		/// @param[in] cwd The root directory within the ZIP archive file. Ignored if plain directory path.
		/// @return True if mount succeeded.
		/// @note Requires a default ZIP file, specified by an empty string for path.
		/// @note If a plain directory is mounted, no other mounts than the default path are allowed.
		static bool mountArchive(const String& path, const String& archiveFilename, const String& cwd = "");
		/// @brief Unmounts ZIP archive or directory.
		/// @param[in] path The mounting path.
		/// @return True if unmount succeeded.
		static bool unmountArchive(const String& path);

	protected:
		/// @brief Data position;
		int64_t dataPosition;
		/// @brief Internal filename
		hstr resourceFilename;
		/// @brief Whether this resource uses a ZIP file as provider
		bool zipResource;

		/// @brief Defines currently mounted archive filenames and mount paths.
		static Map<String, String> mountedArchives;

		/// @brief Updates internal data size.
		void _updateDataSize();

		/// @brief Reads data from the stream.
		/// @param[in] buffer Destination data buffer.
		/// @param[in] count Number of elements to read.
		/// @return Number of bytes read.
		int _read(void* buffer, int count);
		/// @brief Writes data to the stream.
		/// @param[in] buffer Source data buffer.
		/// @param[in] count Number of elements contained in buffer.
		/// @return Number of bytes written.
		int _write(const void* buffer, int count);
		/// @brief Checks if file is open.
		/// @return True if file is open.
		bool _isOpen() const;
		/// @brief Gets current position in file.
		/// @return Current position in file.
		int64_t _position() const;
		/// @brief Seeks to position in file.
		/// @param[in] offset Seeking offset in bytes.
		/// @param[in] seekMode Seeking mode.
		bool _seek(int64_t offset, SeekMode seekMode = SeekMode::Current);
		/// @brief Checks if a resource file exists in a ZIP file.
		/// @param[in] filename Name of the resource file.
		/// @param[in] caseSensitive Whether to check case sensitive files if file was not found.
		/// @return True if resource file exists.
		/// @note Disabling caseSensitive is somewhat costly if the given file is not found at first.
		static bool _zipExists(const String& filename, bool caseSensitive = true);

		/// @brief Create a filename that is outside of the ZIP file system.
		/// @param[in] filename Original filename.
		/// @return Filename that is outside of the ZIP file system.
		static String _makeNonZipPath(const String& filename);

	private:
		/// @brief Copy constructor.
		/// @note Usage is not allowed and it will throw an exception.
		Resource(const Resource& other);
		/// @brief Assignment operator.
		/// @note Usage is not allowed and it will throw an exception.
		Resource& operator=(Resource& other);

	};

}

/// @brief Alias for simpler code.
typedef hltypes::Resource hresource;

#endif
