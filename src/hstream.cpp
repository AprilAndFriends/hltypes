/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifndef _MSC_VER // required for memcpy on non-MSVC compilers
#include <string.h>
#endif

#include "hstream.h"
#include "util.h"

namespace hltypes
{
	Stream::Stream(unsigned char encryption_offset) : StreamBase(encryption_offset)
	{
		this->current_size = 16;
		this->stream_size = 0;
		this->stream_position = 0;
		this->stream = (unsigned char*)malloc(this->current_size * sizeof(unsigned char));
	}
	
	Stream::~Stream()
	{
		if (this->stream != NULL)
		{
			free(this->stream);
		}
	}
	
	void Stream::clear()
	{
		long old_size = this->current_size;
		this->current_size = 16;
		this->stream_size = 0;
		this->stream_position = 0;
		unsigned char* new_stream = (unsigned char*)realloc(this->stream, this->current_size * sizeof(unsigned char));
		if (new_stream == NULL) // could not reallocate enough memory
		{
			this->current_size = old_size;
		}
		else
		{
			this->stream = new_stream;
		}
		this->_update_data_size();
	}
	
	void Stream::_update_data_size()
	{
		this->data_size = this->stream_size;
	}

	long Stream::_read(void* buffer, int size, int count)
	{
		long read_size = hclamp((long)(size * count), 0L, this->stream_size - this->stream_position);
		memcpy(buffer, &this->stream[this->stream_position], read_size);
		this->stream_position += read_size;
		return read_size;
	}
	
	long Stream::_write(const void* buffer, int size, int count)
	{
		long write_size = hmax(size * count, 0);
		if (write_size > this->current_size - this->stream_position)
		{
			long old_size = this->current_size;
			while (write_size > this->current_size - this->stream_position)
			{
				this->current_size *= 2;
			}
			unsigned char* new_stream = (unsigned char*)realloc(this->stream, this->current_size * sizeof(unsigned char));
			if (new_stream == NULL) // could not reallocate enough memory
			{
				this->current_size = old_size;
				write_size = this->current_size - this->stream_position;
			}
			else
			{
				this->stream = new_stream;
			}
		}
		memcpy(&this->stream[this->stream_position], buffer, write_size);
		this->stream_position += write_size;
		if (this->stream_position > this->stream_size)
		{
			this->stream_size = this->stream_position;
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
	
}
