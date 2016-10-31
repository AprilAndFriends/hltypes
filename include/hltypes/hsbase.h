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
/// Provides a base class for streaming.

#ifndef HLTYPES_STREAM_BASE_H
#define HLTYPES_STREAM_BASE_H

#include <stdint.h>
#include <stdio.h>

#include "harray.h"
#include "hltypesExport.h"
#include "hstring.h"

namespace hltypes
{
	class Stream;
	template <typename T> class Array;

	/// @brief Provides a base class for streaming.
	class hltypesExport StreamBase
	{
	public:
		/// @brief Defines file seek modes.
		enum SeekMode
		{
			/// @brief Seek from current position. (SEEK_CUR)
			CURRENT = 0,
			/// @brief Seek from start position. (SEEK_SET)
			START = 1,
			/// @brief Seek from current position. (SEEK_END)
			END = 2
		};
		
		/// @brief Basic constructor.
		StreamBase();
		/// @brief Destructor.
		virtual ~StreamBase();

		/// @brief Checks if data is "open".
		/// @return True if data is "open".
		bool isOpen() const;
		/// @brief Seeks to position in data.
		/// @param[in] offset Seeking offset in bytes.
		/// @param[in] seekMode Seeking mode.
		/// @return True if successful.
		bool seek(int64_t offset, SeekMode seekMode = CURRENT);
		/// @brief Seeks to position 0.
		/// @return True if successful.
		bool rewind();
		/// @brief Gets current position in data.
		/// @return Current position in data.
		int64_t position() const;
		/// @brief Gets size of the data in bytes.
		/// @return Size of the data in bytes.
		int64_t size() const;
		/// @brief Checks if data has reached the end.
		/// @return True if data has reached the end.
		bool eof() const;

		/// @brief Reads from the stream until delimiter character is read.
		/// @param[in] delimiter String where to stop reading.
		/// @return The read string.
		/// @note Delimiter String is not included in return result.
		/// @note When delimiter is omitted, the file will be read until EOF.
		String read(const String& delimiter = "");
		/// @brief Reads n bytes from the stream.
		/// @param[in] count Number of bytes to read.
		/// @return The read string.
		String read(int count);
		/// @brief Reads one line from the stream.
		/// @return The read line.
		/// @note \\n is not included in the returned String.
		String readLine();
		/// @brief Reads all lines from the stream.
		/// @return Array with read lines.
		/// @note \\n is not included in the read lines.
		Array<String> readLines();
		/// @brief Writes string to the stream.
		/// @param[in] text String to write.
		void write(const String& text);
		/// @brief Writes string to the stream.
		/// @param[in] text C-type string to write.
		void write(const char* text);
		/// @brief Writes string to the stream and appends \\n at the end.
		/// @param[in] text String to write.
		void writeLine(const String& text);
		/// @brief Writes string to the stream and appends \\n at the end.
		/// @param[in] text C-type string to write.
		void writeLine(const char* text);
		/// @brief Writes formatted string to the stream.
		/// @param[in] format C-type string containing format.
		/// @param[in] ... Formatting arguments.
		void writef(const char* format, ...);
		/// @brief Reads raw data from the stream.
		/// @param[out] buffer Pointer to raw data buffer.
		/// @param[in] count Number of bytes to read.
		/// @return Number of bytes read.
		/// @note If return value differs from parameter count, it can indicate a reading error or that end of file has been reached.
		int readRaw(void* buffer, int count);
		/// @brief Writes raw data to the stream.
		/// @param[in] buffer Pointer to raw data buffer.
		/// @param[in] count Number of bytes to write.
		/// @return Number of bytes written.
		/// @note If return value differs from parameter count, it can indicate a writing error.
		virtual int writeRaw(void* buffer, int count);
		/// @brief Writes raw data to the stream from another stream.
		/// @param[in] stream Another stream.
		/// @param[in] count Number of bytes to write.
		/// @return Number of bytes written.
		virtual int writeRaw(StreamBase& stream, int count);
		/// @brief Writes raw data to the stream from another stream.
		/// @param[in] stream Another stream.
		/// @return Number of bytes written.
		virtual int writeRaw(StreamBase& stream);
		/// @brief Writes raw data to the stream from another stream.
		/// @param[in] stream Another stream.
		/// @param[in] count Number of bytes to write.
		/// @return Number of bytes written.
		virtual int writeRaw(Stream& stream, int count);
		/// @brief Writes raw data to the stream from another stream.
		/// @param[in] stream Another stream.
		/// @return Number of bytes written.
		virtual int writeRaw(Stream& stream);

		/// @brief Dumps data to file in a platform-aware format.
		/// @param c char to dump.
		/// @note Not using uint8_t because it is problematic with int8_t which is signed char and not char.
		virtual void dump(char c);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param c unsigned char to dump.
		/// @note Not using uint8_t because it is problematic with int8_t which is signed char and not char.
		virtual void dump(unsigned char c);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param s short to dump.
		virtual void dump(short s);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param s unsigned short int to dump.
		virtual void dump(unsigned short s);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param i int to dump.
		virtual void dump(int i);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param i unsigned int to dump.
		virtual void dump(unsigned int i);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param l int64 to dump.
		virtual void dump(int64_t l);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param l unsigned int64 to dump.
		virtual void dump(uint64_t l);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param f Float to dump.
		virtual void dump(float f);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param d Double to dump.
		virtual void dump(double d);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param b Bool to dump.
		virtual void dump(bool b);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param string String to dump.
		virtual void dump(const String& string);
		/// @brief Dumps data to file in a platform-aware format.
		/// @param c C-type string to dump.
		virtual void dump(const char* c);

		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded char.
		virtual char loadInt8();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded unsigned char.
		virtual unsigned char loadUint8();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded short.
		virtual short loadInt16();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded unsigned short.
		virtual unsigned short loadUint16();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded int32.
		virtual int loadInt32();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded unsigned int32.
		virtual unsigned int loadUint32();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded int64.
		virtual int64_t loadInt64();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded unsigned int64.
		virtual uint64_t loadUint64();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded float.
		virtual float loadFloat();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded double.
		virtual double loadDouble();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded bool.
		virtual bool loadBool();
		/// @brief Loads data from file in a platform-aware format.
		/// @return Loaded String.
		virtual String loadString();
		
	protected:
		/// @brief Data size, mostly used for optimization and faster "eof" detection.
		int64_t dataSize;

		/// @brief Updates internal data size.
		virtual void _updateDataSize();
		/// @brief Checks if object can be used.
		virtual void _validate() const;

		/// @brief Gets special descriptor.
		/// @return Special descriptor.
		virtual inline String _descriptor() const { return "stream"; }
		/// @brief Reads data from the stream.
		/// @param[in] buffer Destination data buffer.
		/// @param[in] count Number of elements to read.
		/// @return Number of bytes read.
		virtual int _read(void* buffer, int count) = 0;
		/// @brief Writes data to the stream.
		/// @param[in] buffer Source data buffer.
		/// @param[in] count Number of elements contained in buffer.
		/// @return Number of bytes written.
		virtual int _write(const void* buffer, int count) = 0;
		/// @brief Checks if data is "open".
		/// @return True if data is "open".
		virtual bool _isOpen() const = 0;
		/// @brief Gets current position in data.
		/// @return Current position in data.
		virtual int64_t _position() const = 0;
		/// @brief Seeks to position in data.
		/// @param[in] offset Seeking offset in bytes.
		/// @param[in] seekMode Seeking mode.
		/// @return True if successful.
		virtual bool _seek(int64_t offset, SeekMode seekMode = CURRENT) = 0;

	private:
		/// @brief Copy constructor.
		/// @note Usage is not allowed and it will throw an exception.
		StreamBase(const StreamBase& other);
		/// @brief Assignment operator.
		/// @note Usage is not allowed and it will throw an exception.
		StreamBase& operator=(StreamBase& other);

	};
}

/// @brief Alias for simpler code.
typedef hltypes::StreamBase hsbase;

#endif

