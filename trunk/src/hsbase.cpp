/// @file
/// @version 2.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <string.h>
#include <stdarg.h>

#include "harray.h"
#include "hexception.h"
#include "hfbase.h"
#include "hlog.h"
#include "hsbase.h"
#include "hstream.h"
#include "hstring.h"

#define BUFFER_SIZE 4096

namespace hltypes
{
	StreamBase::StreamBase() : data_size(0)
	{
	}
	
	StreamBase::~StreamBase()
	{
	}
	
	bool StreamBase::is_open()
	{
		return this->_is_open();
	}
	
	bool StreamBase::seek(int64_t offset, SeekMode seek_mode)
	{
		this->_check_availability();
		return this->_seek(offset, seek_mode);
	}
	
	bool StreamBase::rewind()
	{
		return this->seek(0, START);
	}
	
	int64_t StreamBase::position()
	{
		this->_check_availability();
		return this->_position();
	}
	
	int64_t StreamBase::size()
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
		int read;
		int index;
		char c[BUFFER_SIZE + 1];
		while (!this->eof())
		{
			read = this->_read(c, BUFFER_SIZE);
			if (read == 0)
			{
				break;
			}
			c[read] = '\0';
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
		char c[BUFFER_SIZE + 1];
		while (count > 0)
		{
			if (count <= BUFFER_SIZE)
			{
				current = count;
			}
			read = this->_read(c, current);
			if (read == 0)
			{
				break;
			}
			c[read] = '\0';
			result += String(c);
			count -= BUFFER_SIZE;
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
		this->_write(text.c_str(), text.size());
		this->_update_data_size();
	}
	
	void StreamBase::write(const char* text)
	{
		this->_check_availability();
		this->_write(text, strlen(text));
		this->_update_data_size();
	}
	
	void StreamBase::write_line(const String& text)
	{
		this->_check_availability();
		this->_write((text + "\n").c_str(), text.size() + 1);
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
		return this->_read(buffer, count);
	}
	
	int StreamBase::write_raw(void* buffer, int count)
	{
		this->_check_availability();
		int result = this->_write(buffer, count);
		this->_update_data_size();
		return result;
	}
		
	int StreamBase::write_raw(StreamBase& stream, int count)
	{
		this->_check_availability();
		count = (int)hmin((int64_t)count, stream.size() - stream.position());
		unsigned char* buffer = new unsigned char[count];
		stream.read_raw(buffer, count);
		stream.seek(-count);
		int result = this->_write(buffer, count);
		delete[] buffer;
		this->_update_data_size();
		return result;
	}

	int StreamBase::write_raw(StreamBase& stream)
	{
		this->_check_availability();
		if (stream.size() - stream.position() > INT_MAX)
		{
			Log::error(hltypes::logTag, "Data too large for writing in: " + stream._descriptor());
			return 0;
		}
		return this->write_raw(stream, (int)(stream.size() - stream.position()));
	}

	int StreamBase::write_raw(Stream& stream, int count)
	{
		this->_check_availability();
		int64_t position = stream.position();
		count = (int)hmin((int64_t)count, stream.size() - position);
		int result = this->_write(&stream[(int)position], count);
		this->_update_data_size();
		return result;
	}

	int StreamBase::write_raw(Stream& stream)
	{
		this->_check_availability();
		if (stream.size() - stream.position() > INT_MAX)
		{
			Log::error(hltypes::logTag, "Data too large for writing in: " + stream._descriptor());
			return 0;
		}
		return this->write_raw(stream, (int)(stream.size() - stream.position()));
	}

	void StreamBase::_update_data_size()
	{
		int64_t position = this->_position();
		this->_seek(0, END);
		this->data_size = this->_position();
		this->_seek(position, START);
	}

	void StreamBase::_check_availability()
	{
		if (!this->is_open())
		{
			throw FileNotOpenException(this->_descriptor());
		}
	}

	void StreamBase::dump(char c)
	{
		this->dump((unsigned char)c);
	}

	void StreamBase::dump(unsigned char c)
	{
		this->_check_availability();
		this->_write(&c, 1);
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
		this->_write(&s, 2);
#else
		unsigned char bytes[2] = {0};
		bytes[1] = (s >> 8) & 0xFF;
		bytes[0] = s & 0xFF;
		this->_write(bytes, 2);
#endif
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
		this->_write(&i, 4);
#else
		unsigned char bytes[4] = {0};
		bytes[3] = (i >> 24) & 0xFF;
		bytes[2] = (i >> 16) & 0xFF;
		bytes[1] = (i >> 8) & 0xFF;
		bytes[0] = i & 0xFF;
		this->_write(bytes, 4);
#endif
		this->_update_data_size();
	}

	void StreamBase::dump(int64_t l)
	{
		this->dump((uint64_t)l);
	}

	void StreamBase::dump(uint64_t l)
	{
		this->_check_availability();
#ifndef __BIG_ENDIAN__
		this->_write(&l, 8);
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
		this->_write(bytes, 8);
#endif
		this->_update_data_size();
	}

	void StreamBase::dump(float f)
	{
#ifndef __BIG_ENDIAN__
		this->_check_availability();
		this->_write((unsigned char*)&f, 4);
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
		this->_write((unsigned char*)&d, 8);
		this->_update_data_size();
#else
		// some more data voodoo magic
		this->dump(*((uint64_t*)&d));
#endif
	}

	void StreamBase::dump(bool b)
	{
		this->_check_availability();
		unsigned char c = (b ? 1 : 0);
		this->_write(&c, 1);
		this->_update_data_size();
	}

	void StreamBase::dump(const String& str)
	{
		this->_check_availability();
		int size = str.size();
		this->dump(size);
		if (size > 0)
		{
			this->_write(str.c_str(), size);
		}
		this->_update_data_size();
	}

	void StreamBase::dump(const char* c)
	{
		this->dump(String(c));
	}

	char StreamBase::load_int8()
	{
		return (char)this->load_uint8();
	}

	unsigned char StreamBase::load_uint8()
	{
		this->_check_availability();
		unsigned char c;
		this->_read(&c, 1);
		return c;
	}

	short StreamBase::load_int16()
	{
		return (short)this->load_uint16();
	}

	unsigned short StreamBase::load_uint16()
	{
		this->_check_availability();
		unsigned short s = 0;
#ifndef __BIG_ENDIAN__
		this->_read((unsigned char*)&s, 2);
#else
		unsigned char bytes[2] = {0};
		this->_read(bytes, 2);
		s |= bytes[1] << 8;
		s |= bytes[0];
#endif
		return s;
	}

	int StreamBase::load_int32()
	{
		return (int)this->load_uint32();
	}

	unsigned int StreamBase::load_uint32()
	{
		this->_check_availability();
		unsigned int i = 0;
#ifndef __BIG_ENDIAN__
		this->_read((unsigned char*)&i, 4);
#else
		unsigned char bytes[4] = {0};
		this->_read(bytes, 4);
		i |= bytes[3] << 24;
		i |= bytes[2] << 16;
		i |= bytes[1] << 8;
		i |= bytes[0];
#endif
		return i;
	}

	int64_t StreamBase::load_int64()
	{
		return (int64_t)this->load_uint64();
	}

	uint64_t StreamBase::load_uint64()
	{
		this->_check_availability();
		uint64_t l = 0;
#ifndef __BIG_ENDIAN__
		this->_read((unsigned char*)&l, 8);
#else
		unsigned char bytes[8] = {0};
		this->_read(bytes, 8);
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

	float StreamBase::load_float()
	{
		float f;
#ifndef __BIG_ENDIAN__
		this->_check_availability();
		this->_read((unsigned char*)&f, 4);
#else
		// data voodoo magic, the float was stored as uint32
		unsigned int i = this->load_uint32();
		f = *((float*)&i);
#endif
		return f;
	}

	double StreamBase::load_double()
	{
		double d;
#ifndef __BIG_ENDIAN__
		this->_check_availability();
		this->_read((unsigned char*)&d, 8);
#else
		// data voodoo magic, the double was stored as uint64
		uint64_t l = this->load_uint64();
		d = *((double*)&l);
#endif
		return d;
	}

	bool StreamBase::load_bool()
	{
		this->_check_availability();
		unsigned char c;
		this->_read(&c, 1);
		return (c != 0);
	}

	String StreamBase::load_string()
	{
		this->_check_availability();
		return this->read(this->load_int32());
	}

}
