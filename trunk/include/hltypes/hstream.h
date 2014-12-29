/// @file
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
	class hltypesExport Stream : public StreamBase
	{
	public:
		/// @brief Constructor.
		/// @param[in] initial_capacity Initial capacity of the internal buffer.
		/// @note initial_capacity is used to prevent unnecessary calls to realloc() internally if it's not needed. This is NOT the Stream's initial size.
		Stream(int initial_capacity = 16);
		/// @brief Constructor.
		/// @param[in] initial_data Initial data in the stream.
		/// @param[in] initial_data_size Initial data's size.
		/// @note initial_data is copied into the stream.
		Stream(unsigned char* initial_data, int initial_data_size);
		/// @brief Constructor.
		/// @param[in] initial_data Initial data in the stream.
		/// @param[in] initial_data_size Initial data's size.
		/// @param[in] initial_capacity Initial capacity of the internal buffer.
		/// @note initial_data is copied into the stream. initial_capacity will be corrected to initial_data_size if less than initial_data_size.
		Stream(unsigned char* initial_data, int initial_data_size, int initial_capacity);
		/// @brief Destructor.
		~Stream();
		/// @brief Clears the stream.
		/// @param[in] initial_capacity Initial capacity of the internal buffer.
		/// @note initial_capacity is used to prevent unnecessary calls to realloc() internally if it's not needed. This is NOT the Stream's initial size.
		void clear(int initial_capacity = 16LL);
		/// @brief Resizes internal buffer.
		/// @param[in] new_capacity New capacity of the internal buffer.
		/// @return True if internal buffer was resized or already the same size that was requested.
		/// @note This does not change the data stream size. Use this to avoid allocation of too much data if not needed.
		/// @note If new_capacity is smaller than the stream size, data will be lost and the stream will be resized.
		bool set_capacity(int new_capacity);
		/// @brief Writes raw data to the stream.
		/// @param[in] buffer Pointer to raw data buffer.
		/// @param[in] count Number of bytes to write.
		/// @return Number of bytes written.
		/// @note If return value differs from parameter count, it can indicate a writing error.
		int write_raw(void* buffer, int count);
		/// @brief Writes raw data to the stream from another stream.
		/// @param[in] stream Another stream.
		/// @param[in] count Number of bytes to write.
		/// @return Number of bytes written.
		int write_raw(StreamBase& stream, int count);
		/// @brief Writes raw data to the stream from another stream.
		/// @param[in] stream Another stream.
		/// @return Number of bytes written.
		int write_raw(StreamBase& stream);
		/// @brief Writes raw data to the stream from another stream.
		/// @param[in] stream Another stream.
		/// @param[in] count Number of bytes to write.
		/// @return Number of bytes written.
		virtual int write_raw(Stream& stream, int count);
		/// @brief Writes raw data to the stream from another stream.
		/// @param[in] stream Another stream.
		/// @return Number of bytes written.
		virtual int write_raw(Stream& stream);
		/// @brief Prepares stream for manual writing without using write_raw() directly.
		/// @param[in] count Number of bytes to prepare. Stream size is increased if necessary, but contains garbage data.
		/// @return Number of bytes ready to be written.
		/// @note Use this when you intend to manually write data. This does not change the current position and seeking has to be done manually as well.
		/// @see write_raw()
		int prepare_manual_write_raw(int count);
		/// @brief Writes a certain value into the buffer.
		/// @param[in] value The value.
		/// @param[in] count Number of bytes to write.
		/// @return Number of bytes written.
		/// @note If return value differs from parameter count, it can indicate a writing error.
		int fill(unsigned char value, int count);
		/// @brief Gets a direct reference to the internal steam.
		/// @param[in] index Reference to a specific element.
		/// @return Direct reference to the internal steam.
		const unsigned char& operator[](int index);

	protected:
		/// @brief Data stream container.
		unsigned char* stream;
		/// @brief Data stream size.
		int64_t stream_size;
		/// @brief writing position;
		int64_t stream_position;
		/// @brief Capacity data of stream container.
		int64_t capacity;

		/// @brief Updates internal data size.
		void _update_data_size();

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
		/// @brief Checks if stream is open.
		/// @return True if stream is open.
		bool _is_open();
		/// @brief Gets current position in stream.
		/// @return Current position in stream.
		int64_t _position();
		/// @brief Seeks to position in stream.
		/// @param[in] offset Seeking offset in bytes.
		/// @param[in] seek_mode Seeking mode.
		bool _seek(int64_t offset, SeekMode seek_mode = CURRENT);
		/// @brief Resizes internal buffer if necessary.
		/// @param[in,out] write_size Number of bytes that is needed for the next write.
		/// @note This does not change the data stream size. Use this to avoid allocation of too much data if not needed.
		bool _try_increase_capacity(int& write_size);

	};
}

/// @brief Alias for simpler code.
typedef hltypes::Stream hstream;

#endif

