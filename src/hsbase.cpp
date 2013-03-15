/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <string.h>
#include <stdarg.h>

#include "exception.h"
#include "harray.h"
#include "hsbase.h"
#include "hstring.h"

#define BUFFER_SIZE 4096

namespace hltypes
{
	StreamBase::StreamBase(unsigned char encryption_offset) : data_size(0)
	{
		this->encryption_offset = encryption_offset;
	}
	
	StreamBase::~StreamBase()
	{
	}
	
	bool StreamBase::is_open()
	{
		return this->_is_open();
	}
	
	void StreamBase::seek(long offset, SeekMode seek_mode)
	{
		this->_check_availability();
		this->_seek(offset, seek_mode);
	}
	
	void StreamBase::rewind()
	{
		this->seek(0, START);
	}
	
	long StreamBase::position()
	{
		this->_check_availability();
		return this->_position();
	}
	
	long StreamBase::size()
	{
		this->_check_availability();
		return this->data_size;
	}
	
	bool StreamBase::eof()
	{
		this->_check_availability();
		// "feof" doesn't really work if you use a write mode, so we use our own detection
		return (this->_position() >= this->data_size);
	}
	
	String StreamBase::read(const String& delimiter)
	{
		this->_check_availability();
		String result;
		Array<String> parts;
		int count;
		int index;
		while (!this->eof())
		{
			char c[BUFFER_SIZE + 1] = {'\0'};
			count = this->_read(c, 1, BUFFER_SIZE);
			if (count == 0)
			{
				break;
			}
			result += c;
			if (delimiter != "")
			{
				index = result.find(delimiter);
				if (index >= 0)
				{
					this->_seek(index - result.size() + delimiter.size(), CURRENT);
					result = result(0, index);
					break;
				}
			}
		}
		result = result.replace("\r", "");
		return result;
	}
	
	String StreamBase::read(int count)
	{
		this->_check_availability();
		String result;
		int current = BUFFER_SIZE;
		int read;
		while (count > 0)
		{
			char c[BUFFER_SIZE + 1] = {'\0'};
			if (count <= BUFFER_SIZE)
			{
				current = count;
			}
			read = this->_read(c, 1, current);
			if (read == 0)
			{
				break;
			}
			count -= BUFFER_SIZE;
			result += String(c);
		}
		return result;
	}

	String StreamBase::read_line()
	{
		return this->read("\n");
	}
	
	Array<String> StreamBase::read_lines()
	{
		return this->read().split("\n", -1, false);
	}
	
	void StreamBase::write(const String& text)
	{
		this->_check_availability();
		this->_write(text.c_str(), 1, text.size());
		this->_update_data_size();
	}
	
	void StreamBase::write(const char* text)
	{
		this->_check_availability();
		this->_write(text, 1, strlen(text));
		this->_update_data_size();
	}
	
	void StreamBase::write_line(const String& text)
	{
		this->_check_availability();
		this->_write((text + "\n").c_str(), 1, text.size() + 1);
		this->_update_data_size();
	}
	
	void StreamBase::write_line(const char* text)
	{
		this->write_line(String(text));
	}
	
	void StreamBase::writef(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		String result(hvsprintf(format, args));
		va_end(args);
		this->write(result);
	}

	int StreamBase::read_raw(void* buffer, int count)
	{
		this->_check_availability();
		return this->_read(buffer, 1, count);
	}
	
	int StreamBase::write_raw(void* buffer, int count)
	{
		this->_check_availability();
		int result = this->_write(buffer, 1, count);
		this->_update_data_size();
		return result;
	}
		
	int StreamBase::write_raw(StreamBase& stream, int count)
	{
		this->_check_availability();
		count = hmin(count, (int)(stream.size() - stream.position()));
		unsigned char* buffer = new unsigned char[count];
		stream.read_raw(buffer, count);
		stream.seek(-count);
		int result = this->_write(buffer, 1, count);
		delete [] buffer;
		this->_update_data_size();
		return result;
	}
	
	int StreamBase::write_raw(StreamBase& stream)
	{
		this->_check_availability();
		return this->write_raw(stream, stream.size() - stream.position());
	}
	
	void StreamBase::_update_data_size()
	{
		long position = this->_position();
		this->_seek(0, END);
		this->data_size = this->_position();
		this->_seek(position, START);
	}

	void StreamBase::_check_availability()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->_descriptor());
		}
	}

	void StreamBase::dump(char c)
	{
		this->_check_availability();
		this->_write(&c, 1, 1);
		this->_update_data_size();
	}

	void StreamBase::dump(unsigned char c)
	{
		this->_check_availability();
		this->_write(&c, 1, 1);
		this->_update_data_size();
	}

	void StreamBase::dump(int i)
	{
		this->dump((unsigned int)i);
	}

	void StreamBase::dump(unsigned int i)
	{
		this->_check_availability();
#ifndef __BIG_ENDIAN__
		this->_write(&i, 1, 4);
#else
		unsigned char bytes[4] = {0};
		bytes[3] = (i >> 24) & 0xFF;
		bytes[2] = (i >> 16) & 0xFF;
		bytes[1] = (i >> 8) & 0xFF;
		bytes[0] = i & 0xFF;
		this->_write(bytes, 1, 4);
#endif
		this->_update_data_size();
	}

	void StreamBase::dump(long l)
	{
		this->dump((unsigned long)l);
	}

	void StreamBase::dump(unsigned long l)
	{
		this->_check_availability();
#ifndef __BIG_ENDIAN__
		if (sizeof(unsigned long) == 8) // long should be 8 bytes
		{
			this->_write(&l, 1, 8);
		}
		else if (sizeof(unsigned long) == 4) // will work with 4 bytes as well
		{
			this->_write(&l, 1, 4);
			unsigned char bytes[4] = {0};
			this->_write(bytes, 1, 4);
		}
		else // other sizes are unacceptable
		{
			throw file_long_error(this->_descriptor());
		}
#else
		unsigned char bytes[8] = {0};
		bytes[7] = (l >> 56) & 0xFF;
		bytes[6] = (l >> 48) & 0xFF;
		bytes[5] = (l >> 40) & 0xFF;
		bytes[4] = (l >> 32) & 0xFF;
		bytes[3] = (l >> 24) & 0xFF;
		bytes[2] = (l >> 16) & 0xFF;
		bytes[1] = (l >> 8) & 0xFF;
		bytes[0] = l & 0xFF;
		this->_write(bytes, 1, 8);
#endif
		this->_update_data_size();
	}

	void StreamBase::dump(short s)
	{
		this->dump((unsigned short)s);
	}

	void StreamBase::dump(unsigned short s)
	{
		this->_check_availability();
#ifndef __BIG_ENDIAN__
		this->_write(&s, 1, 2);
#else
		unsigned char bytes[2] = {0};
		bytes[1] = (s >> 8) & 0xFF;
		bytes[0] = s & 0xFF;
		this->_write(bytes, 1, 2);
#endif
		this->_update_data_size();
	}

	void StreamBase::dump(float f)
	{
#ifndef __BIG_ENDIAN__
		this->_check_availability();
		this->_write((unsigned char*)&f, 1, 4);
		this->_update_data_size();
#else
		// some data voodoo magic
		this->dump(*((unsigned int*)&f));
#endif
	}

	void StreamBase::dump(double d)
	{
#ifndef __BIG_ENDIAN__
		this->_check_availability();
		this->_write((unsigned char*)&d, 1, 8);
		this->_update_data_size();
#else
		// some more data voodoo magic, but this time we make 100% sure it uses 8 bytes
		this->dump(*((unsigned int*)&d));
		this->dump(*((unsigned int*)&d + sizeof(d) / 2));
#endif
	}

	void StreamBase::dump(bool b)
	{
		this->_check_availability();
		unsigned char c = (b ? 1 : 0);
		this->_write(&c, 1, 1);
		this->_update_data_size();
	}

	void StreamBase::dump(const String& str)
	{
		this->_check_availability();
		int size = str.size();
		this->dump(size);
		if (size > 0)
		{
			if (this->encryption_offset == 0)
			{
				this->_write(str.c_str(), 1, size);
			}
			else
			{
				const char* string = str.c_str();
				char* c = new char[size];
				memset(c, 0, size * sizeof(char));
				for_iter (i, 0, size)
				{
					c[i] = string[i] - this->encryption_offset;
				}
				this->_write(c, 1, size);
				delete [] c;
			}
		}
		this->_update_data_size();
	}

	void StreamBase::dump(const char* c)
	{
		this->dump(String(c));
	}

	char StreamBase::load_char()
	{
		this->_check_availability();
		char c;
		this->_read(&c, 1, 1);
		return c;
	}

	unsigned char StreamBase::load_uchar()
	{
		this->_check_availability();
		unsigned char c;
		this->_read(&c, 1, 1);
		return c;
	}

	int StreamBase::load_int()
	{
		return (int)this->load_uint();
	}

	unsigned int StreamBase::load_uint()
	{
		this->_check_availability();
		unsigned int i = 0;
#ifndef __BIG_ENDIAN__
		this->_read((unsigned char*)&i, 1, 4);
#else
		unsigned char bytes[4] = {0};
		this->_read(bytes, 4, 1);
		i |= bytes[3] << 24;
		i |= bytes[2] << 16;
		i |= bytes[1] << 8;
		i |= bytes[0];
#endif
		return i;
	}

	long StreamBase::load_long()
	{
		return (long)this->load_ulong();
	}

	unsigned long StreamBase::load_ulong()
	{
		this->_check_availability();
		unsigned long l = 0;
#ifndef __BIG_ENDIAN__
		if (sizeof(unsigned long) == 8) // long should be 8 bytes
		{
			this->_read((unsigned char*)&l, 1, 8);
		}
		else if (sizeof(unsigned long) == 4) // will work with 4 bytes as well
		{
			this->_read((unsigned char*)&l, 1, 4);
			unsigned char bytes[4] = {0};
			this->_read(bytes, 1, 4);
		}
		else // other sizes are unacceptable
		{
			throw file_long_error(this->_descriptor());
		}
#else
		unsigned char bytes[8] = {0};
		this->_read(bytes, 8, 1);
		l |= bytes[7] << 56;
		l |= bytes[6] << 48;
		l |= bytes[5] << 40;
		l |= bytes[4] << 32;
		l |= bytes[3] << 24;
		l |= bytes[2] << 16;
		l |= bytes[1] << 8;
		l |= bytes[0];
#endif
		return l;
	}

	short StreamBase::load_short()
	{
		return (int)this->load_ushort();
	}

	unsigned short StreamBase::load_ushort()
	{
		this->_check_availability();
		unsigned short s = 0;
#ifndef __BIG_ENDIAN__
		this->_read((unsigned char*)&s, 1, 2);
#else
		unsigned char bytes[2] = {0};
		this->_read(bytes, 1, 2);
		s |= bytes[1] << 8;
		s |= bytes[0];
#endif
		return s;
	}

	float StreamBase::load_float()
	{
		float f;
#ifndef __BIG_ENDIAN__
		this->_check_availability();
		this->_read((unsigned char*)&f, 1, 4);
#else
		// data voodoo magic, the float was stored as uint
		unsigned int i = this->load_uint();
		f = *((float*)&i);
#endif
		return f;
	}

	double StreamBase::load_double()
	{
		double d;
#ifndef __BIG_ENDIAN__
		this->_check_availability();
		this->_read((unsigned char*)&d, 1, 8);
#else
		// some more data voodoo magic, the double was stored as 2 uints
		int halfSize = sizeof(d) / 2;
		unsigned int i = this->load_uint();
		memcpy((unsigned char*)&d, &i, halfSize);
		i = this->load_uint();
		memcpy((unsigned char*)&d + halfSize, &i, halfSize);
#endif
		return d;
	}

	bool StreamBase::load_bool()
	{
		this->_check_availability();
		unsigned char c;
		this->_read(&c, 1, 1);
		return (c != 0);
	}

	String StreamBase::load_hstr()
	{
		this->_check_availability();
		int size = this->load_int();
		String str;
		int count = BUFFER_SIZE;
		char c[BUFFER_SIZE + 1];
		while (size > 0)
		{
			memset(&c, 0, (BUFFER_SIZE + 1) * sizeof(char));
			if (size < BUFFER_SIZE)
			{
				count = size;
			}
			this->_read(c, 1, count);
			if (this->encryption_offset != 0)
			{
				for_iter (i, 0, count)
				{
					c[i] += this->encryption_offset;
				}
			}
			size -= BUFFER_SIZE;
			str += String(c);
		}
		return str;
	}

	String StreamBase::load_string()
	{
		return this->load_hstr();
	}

}
