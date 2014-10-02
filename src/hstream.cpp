/// @file
/// @version 2.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef _MSC_VER // required for memcpy on non-MSVC compilers
#include <string.h>
#include <stdlib.h>
#endif

#include "hfile.h"
#include "hltypesUtil.h"
#include "hstream.h"

namespace hltypes
{
	Stream::Stream(long initial_capacity) : StreamBase()
	{
		this->capacity = initial_capacity;
		this->stream_size = 0;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc(this->capacity * sizeof(unsigned char));
	}

	Stream::Stream(unsigned char* initial_data, long initial_data_size) : StreamBase()
	{
		this->capacity = initial_data_size;
		this->stream_size = initial_data_size;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc(this->capacity * sizeof(unsigned char));
		memcpy(this->stream, initial_data, initial_data_size);
		this->_update_data_size();
	}

	Stream::Stream(unsigned char* initial_data, long initial_data_size, long initial_capacity) : StreamBase()
	{
		this->capacity = hmax(initial_capacity, initial_data_size);
		this->stream_size = initial_data_size;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc(this->capacity * sizeof(unsigned char));
		memcpy(this->stream, initial_data, initial_data_size);
		this->_update_data_size();
	}

	Stream::~Stream()
	{
		if (this->stream != NULL)
		{
			free(this->stream);
		}
	}
	
	void Stream::clear(long initial_capacity)
	{
		this->stream_size = 0;
		this->stream_position = 0;
		this->set_capacity(initial_capacity);
		this->_update_data_size();
	}
	
	bool Stream::set_capacity(long new_capacity)
	{
		if (this->capacity != new_capacity)
		{
			unsigned char* new_stream = (unsigned char*)realloc(this->stream, new_capacity * sizeof(unsigned char));
			if (new_stream == NULL) // could not reallocate enough memory
			{
				return false;
			}
			this->stream = new_stream;
			this->capacity = new_capacity;
			if (this->stream_size > this->capacity)
			{
				this->stream_size = this->capacity;
				this->_update_data_size();
			}
		}
		return true;
	}

	int Stream::write_raw(void* buffer, int count)
	{
		return StreamBase::write_raw(buffer, count);
	}

	int Stream::write_raw(StreamBase& stream, int count)
	{
		this->_check_availability();
		int result = 0;
		if (count > 0)
		{
			long size = (long)count;
			this->_try_increase_capacity(size);
			if (size > 0)
			{
				result = stream.read_raw(&this->stream[this->stream_position], (int)size);
				if (result > 0)
				{
					stream.seek(-result);
					this->stream_position += result;
					if (this->stream_size < this->stream_position)
					{
						this->stream_size = this->stream_position;
						this->_update_data_size();
					}
				}
			}
		}
		return result;
	}

	int Stream::write_raw(StreamBase& stream)
	{
		return StreamBase::write_raw(stream);
	}

	int Stream::write_raw(Stream& stream, int count)
	{
		return StreamBase::write_raw(stream, count);
	}

	int Stream::write_raw(Stream& stream)
	{
		return StreamBase::write_raw(stream);
	}

	int Stream::prepare_manual_write_raw(int count)
	{
		this->_check_availability();
		int result = 0;
		if (count > 0)
		{
			long size = (long)count;
			this->_try_increase_capacity(size);
			if (size > 0)
			{
				result = (int)size;
				if (this->stream_size < this->stream_position + result)
				{
					this->stream_size = this->stream_position + result;
					this->_update_data_size();
				}
			}
		}
		return result;
	}

	int Stream::fill(unsigned char value, int count)
	{
		this->_check_availability();
		int result = 0;
		if (count > 0)
		{
			long size = (long)count;
			this->_try_increase_capacity(size);
			if (size > 0)
			{
				memset(&this->stream[this->stream_position], value, size);
				result = (int)size;
				this->stream_position += result;
				if (this->stream_size < this->stream_position)
				{
					this->stream_size = this->stream_position;
					this->_update_data_size();
				}
			}
		}
		return result;
	}

	const unsigned char& Stream::operator[](int index)
	{
		if (index < 0)
		{
			index += this->stream_size;
		}
		return this->stream[index];
	}
	
	void Stream::_update_data_size()
	{
		this->data_size = this->stream_size;
	}
	
	long Stream::_read(void* buffer, int size, int count)
	{
		long read_size = hclamp((long)size * count, 0L, this->stream_size - this->stream_position);
		if (read_size > 0)
		{
			memcpy(buffer, &this->stream[this->stream_position], read_size);
			this->stream_position += read_size;
		}
		return read_size;
	}

	long Stream::_write(const void* buffer, int size, int count)
	{
		long write_size = hmax((long)size * count, 0L);
		if (write_size > 0)
		{
			this->_try_increase_capacity(write_size);
			if (write_size > 0)
			{
				memcpy(&this->stream[this->stream_position], buffer, write_size);
				this->stream_position += write_size;
				if (this->stream_size < this->stream_position)
				{
					this->stream_size = this->stream_position;
				}
			}
		}
		return write_size;
	}
	
	bool Stream::_is_open()
	{
		return (this->stream != NULL);
	}
	
	long Stream::_position()
	{
		return this->stream_position;
	}
	
	void Stream::_seek(long offset, SeekMode seek_mode)
	{
		switch (seek_mode)
		{
		case CURRENT:
			this->stream_position = hclamp(this->stream_position + offset, 0L, this->stream_size);
			break;
		case START:
			this->stream_position = hclamp(offset, 0L, this->stream_size);
			break;
		case END:
			this->stream_position = hclamp(this->stream_size + offset, 0L, this->stream_size);
			break;
		}
	}
	
	bool Stream::_try_increase_capacity(long& write_size)
	{
		if (write_size > this->capacity - this->stream_position && !this->set_capacity(hpotceil(write_size + this->stream_position)))
		{
			// could not reallocate enough memory
			write_size = hmax(this->capacity - this->stream_position, 0L);
			return false;
		}
		return true;
	}

}
