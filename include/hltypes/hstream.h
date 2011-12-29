/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Provides high level file streaming from RAM.

#ifndef HLTYPES_FILE_STREAM_H
#define HLTYPES_FILE_STREAM_H

#include "hsbase.h"
#include "hltypesExport.h"

namespace hltypes
{
	template <class T> class Array;
	/// @brief Provides high level data streaming.
	/// @author Boris Mikic
	class hltypesExport Stream : public StreamBase
	{
	public:
		/// @brief Basic constructor.
		Stream(unsigned char encryption_offset = 0);
		/// @brief Destructor.
		~Stream();

	protected:
		/// @brief Reads data from the stream.
		/// @param[in] src Destination data buffer.
		/// @param[in] size Size in bytes of a single buffer element.
		/// @param[in] sound Number of elements to read.
		/// @return Number of bytes read.
		long _fread(void* buffer, int size, int count) { return 0L; }
		/// @brief Writes data to the stream.
		/// @param[in] src Source data buffer.
		/// @param[in] size Size in bytes of a single buffer element.
		/// @param[in] sound Number of elements contained in buffer.
		/// @return Number of bytes written.
		long _fwrite(const void* buffer, int size, int count) { return 0L; }
		/// @brief Checks if data is "open".
		/// @return True if data is "open".
		bool _is_open() { return true; }
		/// @brief Gets current position in data.
		/// @return Current position in data.
		long _position() { return 0L; }
		/// @brief Seeks to position in data.
		/// @param[in] offset Seeking offset in bytes.
		/// @param[in] seek_mode Seeking mode.
		void _seek(long offset, SeekMode seek_mode = CURRENT) { }

	};
}

/// @brief Alias for simpler code.
typedef hltypes::Stream hstream;

#endif

