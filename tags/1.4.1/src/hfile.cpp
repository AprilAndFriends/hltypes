/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// prevents recursive calls of hfile::rename and hfile::remove as these functions are called via these pointers
int (*f_rename)(const char* old_name, const char* new_name) = rename;
int (*f_remove)(const char* filename) = remove;

#include "exception.h"
#include "harray.h"
#include "hdir.h"
#include "hfile.h"
#include "hmap.h"
#include "hstring.h"
#include "hthread.h"
#include "util.h"

#define BUFFER_SIZE (4096)

namespace hltypes
{
	float File::timeout = 100.0f;
	int File::repeats = 0;
/******* CONSTRUCT/DESTRUCT ********************************************/

	File::File(chstr filename, AccessMode access_mode, unsigned char encryption_offset) : fileSize(0), cfile(NULL)
	{
		this->filename = normalize_path(filename);
#ifdef NO_FS_TREE
		this->filename = this->filename.replace("/", "___");
#endif
		this->encryption_offset = encryption_offset;
		this->open(filename, access_mode, encryption_offset);
	}
	
	File::File() : filename(""), fileSize(0), cfile(NULL), encryption_offset(0)
	{
	}
	
	File::~File()
	{
		if (this->is_open())
		{
			this->close();
		}
	}
	
/******* METHODS *******************************************************/

	void File::open(chstr filename, AccessMode access_mode, unsigned char encryption_offset)
	{
		if (this->is_open())
		{
			this->close();
		}
		this->filename = normalize_path(filename);
#ifdef NO_FS_TREE
		this->filename = this->filename.replace("/", "___");
#endif
		this->encryption_offset = encryption_offset;
		const char* mode = "rb";
		switch (access_mode)
		{
		case READ:
			mode = "rb";
			break;
		case WRITE:
			mode = "wb";
			break;
		case APPEND:
			mode = "ab";
			break;
		case READ_WRITE:
			mode = "r+b";
			break;
		case READ_WRITE_CREATE:
			mode = "w+b";
			break;
		case READ_APPEND:
			mode = "a+b";
			break;
		}
		int attempts = File::repeats + 1;
		while (true)
		{
			this->cfile = fopen(this->filename.c_str(), mode);
			if (this->cfile != NULL)
			{
				break;
			}
			attempts--;
			if (attempts <= 0)
			{
				break;
			}
			Thread::sleep(File::timeout);
		};
		if (this->cfile == NULL)
		{
			throw file_not_found(this->filename.c_str());
		}
		this->fileSize = this->_getFileSize();
	}
	
	hstr File::read(chstr delimiter)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		hstr result;
		harray<hstr> parts;
		int count;
		int index;
		while (!this->eof())
		{
			char c[BUFFER_SIZE + 1] = {'\0'};
			count = fread(c, 1, BUFFER_SIZE, this->cfile);
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
					this->seek(index - result.size() + delimiter.size(), CURRENT);
					result = result(0, index);
					break;
				}
			}
		}
		result = result.replace("\r", "");
		return result;
	}
	
	hstr File::read(int count)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		hstr result;
		int current = BUFFER_SIZE;
		int read;
		while (count > 0)
		{
			char c[BUFFER_SIZE + 1] = {'\0'};
			if (count <= BUFFER_SIZE)
			{
				current = count;
			}
			read = fread(c, 1, current, this->cfile);
			if (read == 0)
			{
				break;
			}
			count -= BUFFER_SIZE;
			result += hstr(c);
		}
		return result;
	}

	hstr File::read_line()
	{
		return this->read("\n");
	}
	
	Array<hstr> File::read_lines()
	{
		return this->read().split("\n", -1, false);
	}
	
	void File::write(chstr text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(text.c_str(), 1, text.size(), this->cfile);
		this->fileSize = this->_getFileSize();
	}
	
	void File::write(const char* text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(text, 1, strlen(text), this->cfile);
		this->fileSize = this->_getFileSize();
	}
	
	void File::write_line(chstr text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite((text + "\n").c_str(), 1, text.size() + 1, this->cfile);
		this->fileSize = this->_getFileSize();
	}
	
	void File::write_line(const char* text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(text, 1, strlen(text), this->cfile);
		fwrite("\n", 1, 1, this->cfile);
		this->fileSize = this->_getFileSize();
	}
	
	void File::writef(const char* format, ...)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		va_list args;
		va_start(args, format);
		hstr result(hvsprintf(format, args));
		va_end(args);
		this->write(result);
	}

	int File::read_raw(void* buffer, int count)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return fread(buffer, 1, count, this->cfile);
	}
	
	int File::write_raw(void* buffer, int count)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int result = fwrite(buffer, 1, count, this->cfile);
		this->fileSize = this->_getFileSize();
		return result;
	}
		
	void File::seek(long offset, SeekMode seek_mode)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int mode = SEEK_CUR;
		switch (seek_mode)
		{
		case CURRENT:
			mode = SEEK_CUR;
			break;
		case START:
			mode = SEEK_SET;
			break;
		case END:
			mode = SEEK_END;
			break;
		}
		fseek(this->cfile, offset, mode);
	}
	
	long File::position()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return ftell(this->cfile);
	}
	
	long File::size()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return this->fileSize;
	}
	
	bool File::is_open()
	{
		return (this->cfile != NULL);
	}
	
	bool File::eof()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		// "feof" doesn't really work if you use a write mode, so we use our own detection
		return (this->position() >= this->fileSize);
	}
	
	long File::_getFileSize()
	{
		long position = this->position();
		this->seek(0, END);
		long size = this->position();
		this->seek(position, START);
		return size;
	}
	
	void File::close()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fclose(this->cfile);
		this->cfile = NULL;
		this->fileSize = 0;
	}
	
/******* SERIALIZATION DUMP ********************************************/

	void File::dump(char c)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(&c, 1, 1, this->cfile);
	}

	void File::dump(unsigned char c)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(&c, 1, 1, this->cfile);
	}

	void File::dump(int i)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		this->dump((unsigned int)i);
	}

	void File::dump(unsigned int i)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
#ifndef __BIG_ENDIAN__
		fwrite(&i, 1, 4, this->cfile);
#else
		unsigned char bytes[4] = {0};
		bytes[3] = (i >> 24) & 0xFF;
		bytes[2] = (i >> 16) & 0xFF;
		bytes[1] = (i >> 8) & 0xFF;
		bytes[0] = i & 0xFF;
		fwrite(bytes, 1, 4, this->cfile);
#endif
	}

	void File::dump(long l)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		this->dump((unsigned long)l);
	}

	void File::dump(unsigned long l)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
#ifndef __BIG_ENDIAN__
		if (sizeof(unsigned long) == 8) // long should be 8 bytes
		{
			fwrite(&l, 1, 8, this->cfile);
		}
		else if (sizeof(unsigned long) == 4) // will work with 4 bytes as well
		{
			fwrite(&l, 1, 4, this->cfile);
			unsigned char bytes[4] = {0};
			fwrite(bytes, 1, 4, this->cfile);
		}
		else // other sizes are unacceptable
		{
			throw file_long_error(this->filename.c_str());
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
		fwrite(bytes, 1, 8, this->cfile);
#endif
	}

	void File::dump(short s)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		this->dump((unsigned short)s);
	}

	void File::dump(unsigned short s)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
#ifndef __BIG_ENDIAN__
		fwrite(&s, 1, 2, this->cfile);
#else
		unsigned char bytes[2] = {0};
		bytes[1] = (s >> 8) & 0xFF;
		bytes[0] = s & 0xFF;
		fwrite(bytes, 1, 2, this->cfile);
#endif
	}

	void File::dump(float f)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
#ifndef __BIG_ENDIAN__
		fwrite((unsigned char*)&f, 1, 4, this->cfile);
#else
		// some data voodoo magic
		this->dump(*((unsigned int*)&f));
#endif
	}

	void File::dump(double d)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
#ifndef __BIG_ENDIAN__
		fwrite((unsigned char*)&d, 1, 8, this->cfile);
#else
		// some more data voodoo magic, but this time we make 100% sure it uses 8 bytes
		int halfSize = sizeof(d) / 2;
		unsigned int i;
		memcpy(&i, &d, halfSize);
		this->dump(i);
		memcpy(&i, (unsigned char*)&d + halfSize, halfSize);
		this->dump(i);
#endif
	}

	void File::dump(bool b)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c = (b ? 1 : 0);
		fwrite(&c, 1, 1, this->cfile);
	}

	void File::dump(chstr str)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int size = str.size();
		this->dump(size);
		if (size > 0)
		{
			if (this->encryption_offset == 0)
			{
				fwrite(str.c_str(), 1, size, this->cfile);
			}
			else
			{
				const char* string = str.c_str();
				char* c = new char[size];
				memset(c, 0, size * sizeof(char));
				for (int i = 0; i < size; i++)
				{
					c[i] = string[i] - this->encryption_offset;
				}
				fwrite(c, 1, size, this->cfile);
				delete c;
			}
		}
	}

	void File::dump(const char* c)
	{
		this->dump(hstr(c));
	}

/******* SERIALIZATION LOAD ********************************************/

	char File::load_char()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		char c;
		fread(&c, 1, 1, this->cfile);
		return c;
	}

	unsigned char File::load_uchar()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c;
		fread(&c, 1, 1, this->cfile);
		return c;
	}

	int File::load_int()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return (int)this->load_uint();
	}

	unsigned int File::load_uint()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned int i = 0;
#ifndef __BIG_ENDIAN__
		fread((unsigned char*)&i, 1, 4, this->cfile);
#else
		unsigned char bytes[4] = {0};
		fread(bytes, 4, 1, this->cfile);
		i |= bytes[3] << 24;
		i |= bytes[2] << 16;
		i |= bytes[1] << 8;
		i |= bytes[0];
#endif
		return i;
	}

	long File::load_long()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return (long)this->load_ulong();
	}

	unsigned long File::load_ulong()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned long l = 0;
#ifndef __BIG_ENDIAN__
		if (sizeof(unsigned long) == 8) // long should be 8 bytes
		{
			fread((unsigned char*)&l, 1, 8, this->cfile);
		}
		else if (sizeof(unsigned long) == 4) // will work with 4 bytes as well
		{
			fread((unsigned char*)&l, 1, 4, this->cfile);
			unsigned char bytes[4] = {0};
			fread(bytes, 1, 4, this->cfile);
		}
		else // other sizes are unacceptable
		{
			throw file_long_error(this->filename.c_str());
		}
#else
		unsigned char bytes[8] = {0};
		fread(bytes, 8, 1, this->cfile);
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

	short File::load_short()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return (int)this->load_ushort();
	}

	unsigned short File::load_ushort()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned short s = 0;
#ifndef __BIG_ENDIAN__
		fread((unsigned char*)&s, 1, 2, this->cfile);
#else
		unsigned char bytes[2] = {0};
		fread(bytes, 1, 2, this->cfile);
		s |= bytes[1] << 8;
		s |= bytes[0];
#endif
		return s;
	}

	float File::load_float()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		float f;
#ifndef __BIG_ENDIAN__
		fread((unsigned char*)&f, 1, 4, this->cfile);
#else
		// data voodoo magic, the float was stored as uint
		unsigned int i = this->load_uint();
		f = *((float*)&i);
#endif
		return f;
	}

	double File::load_double()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		double d;
#ifndef __BIG_ENDIAN__
		fread((unsigned char*)&d, 1, 8, this->cfile);
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

	bool File::load_bool()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c;
		fread(&c, 1, 1, this->cfile);
		return (c != 0);
	}

	hstr File::load_hstr()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int size = this->load_int();
		hstr str;
		int count = BUFFER_SIZE;
		while (size > 0)
		{
			char c[BUFFER_SIZE + 1] = {'\0'};
			if (size < BUFFER_SIZE)
			{
				count = size;
			}
			fread(c, 1, count, this->cfile);
			if (this->encryption_offset != 0)
			{
				for (int i = 0; i < count; i++)
				{
					c[i] += this->encryption_offset;
				}
			}
			size -= BUFFER_SIZE;
			str += hstr(c);
		}
		return str;
	}

	hstr File::load_string()
	{
		return this->load_hstr();
	}

/******* STATIC ********************************************************/

	bool File::create(chstr filename)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		if (!hfile::exists(name))
		{
			hdir::create_path(name);
			int attempts = File::repeats + 1;
			while (true)
			{
				FILE* f = fopen(name.c_str(), "wb");
				if (f != NULL)
				{
					fclose(f);
					return true;
				}
				attempts--;
				if (attempts <= 0)
				{
					break;
				}
				Thread::sleep(File::timeout);
			}
		}
		return false;
	}
	
	bool File::create_new(chstr filename)
	{
		return (hfile::create(filename) || hfile::clear(filename));
	}
	
	bool File::remove(chstr filename)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		return (f_remove(name.c_str()) == 0);
	}
	
	bool File::exists(chstr filename)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		FILE* f = fopen(name.c_str(), "r");
		if (f != NULL)
		{
			fclose(f);
			return true;
		}
		return false;
	}
	
	bool File::clear(chstr filename)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		if (hfile::exists(name))
		{
			FILE* f = fopen(name.c_str(), "wb");
			if (f != NULL)
			{
				fclose(f);
				return true;
			}
		}
		return false;
	}
	
	bool File::rename(chstr old_filename, chstr new_filename)
	{
		hstr old_name = normalize_path(old_filename);
		hstr new_name = normalize_path(new_filename);
#ifdef NO_FS_TREE
		old_name = old_name.replace("/", "___");
		new_name = new_name.replace("/", "___");
#endif
		if (!hfile::exists(old_name) || hfile::exists(new_name))
		{
			return false;
		}
		hdir::create_path(new_name);
		return (f_rename(old_name.c_str(), new_name.c_str()) == 0);
	}
	
	bool File::move(chstr filename, chstr path)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		return hfile::rename(name, path + "/" + name.rsplit("/", 1, false).pop_last());
	}
	
	bool File::copy(chstr old_filename, chstr new_filename)
	{
		hstr old_name = normalize_path(old_filename);
		hstr new_name = normalize_path(new_filename);
#ifdef NO_FS_TREE
		old_name = old_name.replace("/", "___");
		new_name = new_name.replace("/", "___");
#endif
		if (!hfile::exists(old_name) || hfile::exists(new_name))
		{
			return false;
		}
		hdir::create_path(new_name);
		hfile old_file(old_name);
		hfile new_file(new_name, hfile::WRITE);
		int count;
		char c[BUFFER_SIZE] = {'\0'}; // literal buffer, not a string buffer that requires \0 at the end
		while (!old_file.eof())
		{
			count = fread(c, 1, BUFFER_SIZE, old_file.cfile);
			fwrite(c, 1, count, new_file.cfile);
		}
		return true;
	}
	
/******* QUICK READ/WRITE **********************************************/

	long File::hsize(chstr filename)
	{
		hstr name = normalize_path(filename);
		return hfile(name).size();
	}
	
	hstr File::hread(chstr filename, int count)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		return hfile(name).read(count);
	}
	
	hstr File::hread(chstr filename, chstr delimiter)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		return hfile(name).read(delimiter);
	}
	
	void File::hwrite(chstr filename, chstr text)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		hfile(name, WRITE).write(text);
	}
	
	void File::happend(chstr filename, chstr text)
	{
		hstr name = normalize_path(filename);
#ifdef NO_FS_TREE
		name = name.replace("/", "___");
#endif
		hfile(name, APPEND).write(text);
	}
	
}