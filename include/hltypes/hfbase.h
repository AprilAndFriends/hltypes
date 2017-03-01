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
/// Provides a base for high level file handling.

#ifndef HLTYPES_FILE_BASE_H
#define HLTYPES_FILE_BASE_H

#include <stdint.h>
#include <stdio.h>

#include "henum.h"
#include "hsbase.h"
#include "hstring.h"

#include "hltypesExport.h"

namespace hltypes
{
	/// @brief Used for information about files.
	struct hltypesExport FileInfo
	{
		/// @brief File size.
		int64_t size;
		/// @brief The time the file was created.
		int64_t creationTime;
		/// @brief The time the file was last accessed.
		int64_t accessTime;
		/// @brief The time the file was last modified.
		int64_t modificationTime;

		/// @brief Constructor.
		FileInfo();
		/// @brief Destructor.
		~FileInfo();

	};
	
	/// @brief Provides a base for high level file handling.
	/// @note When writing, \\r may be used, but \\r will be removed during read.
	class hltypesExport FileBase : public StreamBase
	{
	public:
		/// @class AccessMode
		/// @brief Defines file access modes.
		/// @note Windows text read/write modes are not used because they do not work properly in multiplatform environments.
		HL_ENUM_CLASS_PREFIX_DECLARE(hltypesExport, AccessMode,
		(
			/// @var static const AccessMode AccessMode::Read
			/// @brief Read-only file mode. ("rb")
			HL_ENUM_DECLARE(AccessMode, Read);
			/// @var static const AccessMode AccessMode::Write
			/// @brief Write-only file mode. ("wb")
			HL_ENUM_DECLARE(AccessMode, Write);
			/// @var static const AccessMode AccessMode::Append
			/// @brief Write and append file mode. ("ab")
			HL_ENUM_DECLARE(AccessMode, Append);
			/// @var static const AccessMode AccessMode::ReadWrite
			/// @brief Read and write file mode. ("r+b")
			HL_ENUM_DECLARE(AccessMode, ReadWrite);
			/// @var static const AccessMode AccessMode::ReadWriteCreate
			/// @brief Read, write and create file mode. ("w+b")
			HL_ENUM_DECLARE(AccessMode, ReadWriteCreate);
			/// @var static const AccessMode AccessMode::ReadAppend
			/// @brief Read and append file mode. ("a+b")
			HL_ENUM_DECLARE(AccessMode, ReadAppend);
		));

		/// @brief Constructor with filename.
		/// @param[in] filename Name of the file (may include path).
		FileBase(const String& filename);
		/// @brief Basic constructor.
		FileBase();
		/// @brief Destructor.
		~FileBase();
		/// @brief Gets the current filename.
		/// @return The current filename.
		HL_DEFINE_GET(String, filename, Filename);
		/// @brief Opens a file-base object.
		/// @param[in] filename Name of the file-base object (may include path).
		virtual void open(const String& filename) = 0;
		/// @brief Closes resource file.
		virtual void close() = 0;

		/// @brief Sets the number of repeated attempts to access a file.
		/// @param[in] value New value.
		/// @note Not implemented as inline, because optimizer strips it away even if compiled as DLL.
		static void setRepeats(int value);
		/// @brief Sets the timeout in miliseconds between repeated attempts to access a file.
		/// @param[in] value New value.
		/// @note Not implemented as inline, because optimizer strips it away even if compiled as DLL.
		static void setTimeout(float value);

		/// @brief Gets the extension of the filename.
		/// @param[in] path The path.
		/// @return Extension of the filename.
		static String extensionOf(const String& path);
		/// @brief Gets the filename with the extension (with the prepended directory path).
		/// @param[in] path The path.
		/// @return Filename with the extension (with the prepended directory path).
		static String withoutExtension(const String& path);

	protected:
		/// @brief Current filename.
		String filename;
		/// @brief OS file handle.
		void* cfile;

		/// @brief Gets special descriptor.
		/// @return Special descriptor.
		String _descriptor() const;

		/// @brief Opens an OS file.
		/// @param[in] filename Name of the file (may include path).
		/// @param[in] accessMode File access mode.
		/// @param[in] repeats Number of repeated attempts to access a file.
		/// @param[in] timeout Timeout in miliseconds between repeated attempts to access a file.
		/// @note If this instance is already working with an opened file handle, that file handle will be closed.
		void _fopen(const String& filename, AccessMode accessMode, int repeats, float timeout);
		/// @brief Closes file.
		void _fclose();
		/// @brief Reads data from the file.
		/// @param[in] buffer Destination data buffer.
		/// @param[in] count Number of elements to read.
		/// @return Number of bytes read.
		int _fread(void* buffer, int count);
		/// @brief Writes data to the file.
		/// @param[in] buffer Source data buffer.
		/// @param[in] count Number of elements contained in buffer.
		/// @return Number of bytes written.
		int _fwrite(const void* buffer, int count);
		/// @brief Checks if file is open.
		/// @return True if file is open.
		virtual bool _fisOpen() const;
		/// @brief Gets current position in file.
		/// @return Current position in file.
		int64_t _fposition() const;
		/// @brief Seeks to position in file.
		/// @param[in] offset Seeking offset in bytes.
		/// @param[in] seekMode Seeking mode.
		/// @return True if successful.
		bool _fseek(int64_t offset, SeekMode seekMode = SeekMode::Current);

		/// @brief Checks if a file exists.
		/// @param[in] filename Name of the file.
		/// @param[in] caseSensitive Whether to check case sensitive files if file was not found.
		/// @return True if file exists.
		/// @note Disabling caseSensitive is somewhat costly if the given file is not found at first.
		static bool _fexists(const String& filename, bool caseSensitive = true);

		/// @brief Defines the number of repeated attempts to access a file.
		static int repeats;
		/// @brief Defines the timeout in miliseconds between repeated attempts to access a file.
		static float timeout;

	private:
		/// @brief Copy constructor.
		/// @note Usage is not allowed and it will throw an exception.
		FileBase(const FileBase& other);
		/// @brief Assignment operator.
		/// @note Usage is not allowed and it will throw an exception.
		FileBase& operator=(FileBase& other);

	};

}

/// @brief Alias for simpler code.
typedef hltypes::FileBase hfbase;
/// @brief Alias for simpler code.
typedef hltypes::FileInfo hfinfo;
/// @brief Alias for simpler code.
typedef hltypes::FileBase::AccessMode hfaccess;

#endif

