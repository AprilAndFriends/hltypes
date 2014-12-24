/// @file
/// @version 2.5
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
	Stream::Stream(int32_t initial_capacity) : StreamBase()
	{
		this->capacity = (int64_t)initial_capacity;
		this->stream_size = 0;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int32_t)this->capacity);
	}

	Stream::Stream(unsigned char* initial_data, int32_t initial_data_size) : StreamBase()
	{
		this->capacity = (int64_t)initial_data_size;
		this->stream_size = (int64_t)initial_data_size;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int32_t)this->capacity);
		memcpy(this->stream, initial_data, initial_data_size);
		this->_update_data_size();
	}

	Stream::Stream(unsigned char* initial_data, int32_t initial_data_size, int32_t initial_capacity) : StreamBase()
	{
		this->capacity = (int64_t)hmax(initial_capacity, initial_data_size);
		this->stream_size = (int64_t)initial_data_size;
		this->stream_position = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int32_t)this->capacity);
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
	
	void Stream::clear(int32_t initial_capacity)
	{
		this->stream_size = 0;
		this->stream_position = 0;
		this->set_capacity(initial_capacity);
		this->_update_data_size();
	}
	
	bool Stream::set_capacity(int32_t new_capacity)
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
				this->_update_data_size();
			}
		}
		return true;
	}

	int32_t Stream::write_raw(void* buffer, int32_t count)
	{
		return StreamBase::write_raw(buffer, count);
	}

	int32_t Stream::write_raw(StreamBase& stream, int32_t count)
	{
		this->_check_availability();
		int32_t result = 0;
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
						this->_update_data_size();
					}
				}
			}
		}
		return result;
	}

	int32_t Stream::write_raw(StreamBase& stream)
	{
		return StreamBase::write_raw(stream);
	}

	int32_t Stream::write_raw(Stream& stream, int32_t count)
	{
		return StreamBase::write_raw(stream, count);
	}

	int32_t Stream::write_raw(Stream& stream)
	{
		return StreamBase::write_raw(stream);
	}

	int32_t Stream::prepare_manual_write_raw(int32_t count)
	{
		this->_check_availability();
		int32_t result = 0;
		if (count > 0)
		{
			this->_try_increase_capacity(count);
			if (count > 0)
			{
				result = count;
				if (this->stream_size < this->stream_position + count)
				{
					this->stream_size = this->stream_position + count;
					this->_update_data_size();
				}
			}
		}
		return result;
	}

	int32_t Stream::fill(unsigned char value, int32_t count)
	{
		this->_check_availability();
		int32_t result = 0;
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
					this->_update_data_size();
				}
			}
		}
		return result;
	}

	const unsigned char& Stream::operator[](int32_t index)
	{
		if (index < 0)
		{
			index = (int32_t)((int64_t)index + this->stream_size);
		}
		return this->stream[index];
	}
	
	void Stream::_update_data_size()
	{
		this->data_size = this->stream_size;
	}
	
	int32_t Stream::_read(void* buffer, int32_t count)
	{
		int32_t read_size = (int32_t)hclamp((int64_t)count, 0LL, this->stream_size - this->stream_position);
		if (read_size > 0)
		{
			memcpy(buffer, &this->stream[this->stream_position], read_size);
			this->stream_position += read_size;
		}
		return read_size;
	}

	int32_t Stream::_write(const void* buffer, int32_t count)
	{
		int32_t result = 0;
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
	
	bool Stream::_seek(int64_t offset, SeekMode seek_mode)
	{
		switch (seek_mode)
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
	
	bool Stream::_try_increase_capacity(int32_t& write_size)
	{
		if (write_size > this->capacity - this->stream_position && !this->set_capacity(hpotceil((int32_t)(write_size + this->stream_position))))
		{
			// could not reallocate enough memory, reduce write_size
			write_size = (int32_t)hmax(this->capacity - this->stream_position, 0LL);
			return false;
		}
		return true;
	}

}
