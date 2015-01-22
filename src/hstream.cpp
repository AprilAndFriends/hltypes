/// @file
/// @version 3.0
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
		this->streamSize = 0;
		this->streamPosition = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int)this->capacity);
	}

	Stream::Stream(unsigned char* initialData, int initialDataSize) : StreamBase()
	{
		this->capacity = (int64_t)initialDataSize;
		this->streamSize = (int64_t)initialDataSize;
		this->streamPosition = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int)this->capacity);
		memcpy(this->stream, initialData, initialDataSize);
		this->_updateDataSize();
	}

	Stream::Stream(unsigned char* initialData, int initialDataSize, int initialCapacity) : StreamBase()
	{
		this->capacity = (int64_t)hmax(initialCapacity, initialDataSize);
		this->streamSize = (int64_t)initialDataSize;
		this->streamPosition = 0;
		// using malloc because realloc is used later
		this->stream = (unsigned char*)malloc((int)this->capacity);
		memcpy(this->stream, initialData, initialDataSize);
		this->_updateDataSize();
	}

	Stream::~Stream()
	{
		if (this->stream != NULL)
		{
			free(this->stream);
		}
	}
	
	void Stream::clear(int newCapacity)
	{
		this->streamSize = 0;
		this->streamPosition = 0;
		this->setCapacity(newCapacity);
		this->_updateDataSize();
	}
	
	bool Stream::setCapacity(int newCapacity)
	{
		if (this->capacity != newCapacity)
		{
			unsigned char* newStream = (unsigned char*)realloc(this->stream, newCapacity);
			if (newStream == NULL) // could not reallocate enough memory
			{
				return false;
			}
			this->stream = newStream;
			this->capacity = newCapacity;
			if (this->streamSize > this->capacity)
			{
				this->streamSize = this->capacity;
				this->_updateDataSize();
			}
		}
		return true;
	}

	int Stream::writeRaw(void* buffer, int count)
	{
		return StreamBase::writeRaw(buffer, count);
	}

	int Stream::writeRaw(StreamBase& stream, int count)
	{
		this->_validate();
		int result = 0;
		if (count > 0)
		{
			this->_tryIncreaseCapacity(count);
			if (count > 0)
			{
				result = stream.readRaw(&this->stream[(int)this->streamPosition], count);
				if (result > 0)
				{
					stream.seek(-result);
					this->streamPosition += result;
					if (this->streamSize < this->streamPosition)
					{
						this->streamSize = this->streamPosition;
						this->_updateDataSize();
					}
				}
			}
		}
		return result;
	}

	int Stream::writeRaw(StreamBase& stream)
	{
		return StreamBase::writeRaw(stream);
	}

	int Stream::writeRaw(Stream& stream, int count)
	{
		return StreamBase::writeRaw(stream, count);
	}

	int Stream::writeRaw(Stream& stream)
	{
		return StreamBase::writeRaw(stream);
	}

	int Stream::prepareManualWriteRaw(int count)
	{
		this->_validate();
		int result = 0;
		if (count > 0)
		{
			this->_tryIncreaseCapacity(count);
			if (count > 0)
			{
				result = count;
				if (this->streamSize < this->streamPosition + count)
				{
					this->streamSize = this->streamPosition + count;
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
			this->_tryIncreaseCapacity(count);
			if (count > 0)
			{
				memset(&this->stream[(int)this->streamPosition], value, count);
				result = count;
				this->streamPosition += count;
				if (this->streamSize < this->streamPosition)
				{
					this->streamSize = this->streamPosition;
					this->_updateDataSize();
				}
			}
		}
		return result;
	}

	bool Stream::truncate(int64_t targetSize)
	{
		this->_validate();
		if (targetSize >= this->size())
		{
			return false;
		}
		this->streamSize = hmax(targetSize, (int64_t)0);
		this->streamPosition = hmin(this->streamPosition, this->streamSize);
		this->_updateDataSize();
		return true;
	}

	const unsigned char& Stream::operator[](int index)
	{
		if (index < 0)
		{
			index = (int)((int64_t)index + this->streamSize);
		}
		return this->stream[index];
	}
	
	void Stream::_updateDataSize()
	{
		this->dataSize = this->streamSize;
	}
	
	int Stream::_read(void* buffer, int count)
	{
		int readSize = (int)hclamp((int64_t)count, 0LL, this->streamSize - this->streamPosition);
		if (readSize > 0)
		{
			memcpy(buffer, &this->stream[this->streamPosition], readSize);
			this->streamPosition += readSize;
		}
		return readSize;
	}

	int Stream::_write(const void* buffer, int count)
	{
		int result = 0;
		if (count > 0)
		{
			this->_tryIncreaseCapacity(count);
			if (count > 0)
			{
				memcpy(&this->stream[(int)this->streamPosition], buffer, count);
				result = count;
				this->streamPosition += result;
				this->streamSize = hmin(this->streamSize, this->streamPosition);
			}
		}
		return result;
	}
	
	bool Stream::_isOpen()
	{
		return (this->stream != NULL);
	}
	
	int64_t Stream::_position()
	{
		return this->streamPosition;
	}
	
	bool Stream::_seek(int64_t offset, SeekMode seekMode)
	{
		switch (seekMode)
		{
		case CURRENT:
			this->streamPosition = hclamp(this->streamPosition + offset, 0LL, this->streamSize);
			break;
		case START:
			this->streamPosition = hclamp(offset, 0LL, this->streamSize);
			break;
		case END:
			this->streamPosition = hclamp(this->streamSize + offset, 0LL, this->streamSize);
			break;
		}
		return true;
	}
	
	bool Stream::_tryIncreaseCapacity(int& write_size)
	{
		if (write_size > this->capacity - this->streamPosition && !this->setCapacity(hpotceil((int)(write_size + this->streamPosition))))
		{
			// could not reallocate enough memory, reduce write_size
			write_size = (int)hmax(this->capacity - this->streamPosition, 0LL);
			return false;
		}
		return true;
	}

}
