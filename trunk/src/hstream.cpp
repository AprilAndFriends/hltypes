/// @file
/// @version 2.6
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
	Stream::Stream(int initialCapacity) : StreamBase()
	{
		this->capacity = (int64_t)initialCapacity;
		this->stream_size = 0;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int)this->capacity);
	}

	Stream::Stream(unsigned char* initial_data, int initialDataSize) : StreamBase()
	{
		this->capacity = (int64_t)initialDataSize;
		this->stream_size = (int64_t)initialDataSize;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int)this->capacity);
		memcpy(this->stream, initial_data, initialDataSize);
		this->_updateDataSize();
	}

	Stream::Stream(unsigned char* initial_data, int initialDataSize, int initialCapacity) : StreamBase()
	{
		this->capacity = (int64_t)hmax(initialCapacity, initialDataSize);
		this->stream_size = (int64_t)initialDataSize;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int)this->capacity);
		memcpy(this->stream, initial_data, initialDataSize);
		this->_updateDataSize();
	}

	Stream::~Stream()
	{
		if (this->stream != NULL)
		{
			free(this->stream);
		}
	}
	
	void Stream::clear(int initialCapacity)
	{
		this->stream_size = 0;
		this->stream_position = 0;
		this->set_capacity(initialCapacity);
		this->_updateDataSize();
	}
	
	bool Stream::set_capacity(int new_capacity)
	{
		if (this->capacity != new_capacity)
		{
			unsigned char* new_stream = (unsigned char*)realloc(this->stream, new_capacity);
			if (new_stream == NULL) // could not reallocate enough memory
			{
				return false;
			}
			this->stream = new_stream;
			this->capacity = new_capacity;
			if (this->stream_size > this->capacity)
			{
				this->stream_size = this->capacity;
				this->_updateDataSize();
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
		this->_validate();
		int result = 0;
		if (count > 0)
		{
			this->_try_increase_capacity(count);
			if (count > 0)
			{
				result = stream.read_raw(&this->stream[this->stream_position], count);
				if (result > 0)
				{
					stream.seek(-result);
					this->stream_position += result;
					if (this->stream_size < this->stream_position)
					{
						this->stream_size = this->stream_position;
						this->_updateDataSize();
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
		this->_validate();
		int result = 0;
		if (count > 0)
		{
			this->_try_increase_capacity(count);
			if (count > 0)
			{
				result = count;
				if (this->stream_size < this->stream_position + count)
				{
					this->stream_size = this->stream_position + count;
					this->_updateDataSize();
				}
			}
		}
		return result;
	}

	int Stream::fill(unsigned char value, int count)
	{
		this->_validate();
		int result = 0;
		if (count > 0)
		{
			this->_try_increase_capacity(count);
			if (count > 0)
			{
				memset(&this->stream[this->stream_position], value, count);
				result = count;
				this->stream_position += count;
				if (this->stream_size < this->stream_position)
				{
					this->stream_size = this->stream_position;
					this->_updateDataSize();
				}
			}
		}
		return result;
	}

	const unsigned char& Stream::operator[](int index)
	{
		if (index < 0)
		{
			index = (int)((int64_t)index + this->stream_size);
		}
		return this->stream[index];
	}
	
	void Stream::_updateDataSize()
	{
		this->dataSize = this->stream_size;
	}
	
	int Stream::_read(void* buffer, int count)
	{
		int read_size = (int)hclamp((int64_t)count, 0LL, this->stream_size - this->stream_position);
		if (read_size > 0)
		{
			memcpy(buffer, &this->stream[this->stream_position], read_size);
			this->stream_position += read_size;
		}
		return read_size;
	}

	int Stream::_write(const void* buffer, int count)
	{
		int result = 0;
		if (count > 0)
		{
			this->_try_increase_capacity(count);
			if (count > 0)
			{
				memcpy(&this->stream[this->stream_position], buffer, count);
				result = count;
				this->stream_position += result;
				if (this->stream_size < this->stream_position)
				{
					this->stream_size = this->stream_position;
				}
			}
		}
		return result;
	}
	
	bool Stream::_is_open()
	{
		return (this->stream != NULL);
	}
	
	int64_t Stream::_position()
	{
		return this->stream_position;
	}
	
	bool Stream::_seek(int64_t offset, SeekMode seekMode)
	{
		switch (seekMode)
		{
		case CURRENT:
			this->stream_position = hclamp(this->stream_position + offset, 0LL, this->stream_size);
			break;
		case START:
			this->stream_position = hclamp(offset, 0LL, this->stream_size);
			break;
		case END:
			this->stream_position = hclamp(this->stream_size + offset, 0LL, this->stream_size);
			break;
		}
		return true;
	}
	
	bool Stream::_try_increase_capacity(int& write_size)
	{
		if (write_size > this->capacity - this->stream_position && !this->set_capacity(hpotceil((int)(write_size + this->stream_position))))
		{
			// could not reallocate enough memory, reduce write_size
			write_size = (int)hmax(this->capacity - this->stream_position, 0LL);
			return false;
		}
		return true;
	}

}
