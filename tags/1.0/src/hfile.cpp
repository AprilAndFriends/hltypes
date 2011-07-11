/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
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
#include "util.h"

#define BUFFER_SIZE (4096)

namespace hltypes
{
/******* CONSTRUCT/DESTRUCT ********************************************/

	file::file(chstr filename, AccessMode access_mode, int encryption_offset) : cfile(NULL)
	{
		this->filename = filename;
		this->encryption_offset = encryption_offset;
		this->open(filename, access_mode, encryption_offset);
	}
	
	file::file() : filename(""), cfile(NULL), encryption_offset(0)
	{
	}
	
	file::~file()
	{
		if (this->is_open())
		{
			this->close();
		}
	}
	
/******* METHODS *******************************************************/

	void file::open(chstr filename, AccessMode access_mode, int encryption_offset)
	{
		if (this->is_open())
		{
			this->close();
		}
		this->filename = normalize_path(filename);
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
		this->cfile = fopen(this->filename.c_str(), mode);
		if (this->cfile == NULL)
		{
			throw file_not_found(this->filename.c_str());
		}
	}
	
	hstr file::read_line()
	{
		return this->read("\n");
	}
	
	harray<hstr> file::read_lines()
	{
		return this->read().split("\n");
	}
	
	hstr file::read(chstr delimiter)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		hstr result;
		hstr str;
		harray<hstr> parts;
		int count;
		while (!this->eof())
		{
			char c[BUFFER_SIZE + 1] = {'\0'};
			count = fread(c, 1, BUFFER_SIZE, this->cfile);
			if (count == 0)
			{
				break;
			}
			str = hstr(c);
			if (delimiter != "")
			{
				parts = str.split(delimiter, 1);
				if (parts.size() > 1)
				{
					result += parts[0];
					this->seek(-parts[1].size(), CURRENT);
					break;
				}
			}
			result += str;
		}
		result = result.replace("\r", "");
		return result;
	}
	
	hstr file::read(int count)
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

	void file::write(chstr text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(text.c_str(), 1, text.size(), this->cfile);
	}
	
	void file::write(const char* text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(text, 1, strlen(text), this->cfile);
	}
	
	void file::write_line(chstr text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite((text + "\n").c_str(), 1, text.size() + 1, this->cfile);
	}
	
	void file::write_line(const char* text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(text, 1, strlen(text), this->cfile);
		fwrite("\n", 1, 1, this->cfile);
	}
	
	void file::writef(const char* format, ...)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		char c[BUFFER_SIZE + 1] = {'\0'};
		va_list args;
		va_start(args, format);
		vsnprintf(c, BUFFER_SIZE, format, args);
		va_end(args);
		this->write(c);
	}

	void file::read_raw(void* buffer, int count)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fread(buffer, 1, count, this->cfile);
	}
	
	void file::write_raw(void* buffer, int count)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(buffer, 1, count, this->cfile);
	}
		
	void file::seek(long offset, SeekMode seek_mode)
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
	
	long file::position()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return ftell(this->cfile);
	}
	
	long file::size()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		long position = this->position();
		this->seek(0, END);
		long size = this->position();
		this->seek(position, START);
		return size;
	}
	
	bool file::is_open()
	{
		return (this->cfile != NULL);
	}
	
	bool file::eof()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return (feof(this->cfile) != 0);
	}
	
	void file::close()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fclose(this->cfile);
		this->cfile = NULL;
	}
	
/******* SERIALIZATION DUMP ********************************************/

	void file::dump(unsigned char c)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(&c, 1, 1, this->cfile);
	}

	void file::dump(int i)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		this->dump((unsigned int)i);
	}

	void file::dump(unsigned int i)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
#ifndef __BIG_ENDIAN__
		fwrite(&i, 1, 4, this->cfile);
#else
		unsigned char bytes[4] = {0};
		bytes[0] = (i >> 24) & 0xFF;
		bytes[1] = (i >> 16) & 0xFF;
		bytes[2] = (i >> 8) & 0xFF;
		bytes[3] = i & 0xFF;
		fwrite(bytes, 1, 4, this->cfile);
#endif
	}

	void file::dump(short s)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		this->dump((unsigned short)s);
	}

	void file::dump(unsigned short s)
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

	void file::dump(float f)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
#ifndef __BIG_ENDIAN__
		fwrite((unsigned char*)&f, 1, 4, this->cfile);
#else
		unsigned int i;
		memcpy(&i, &f, sizeof(f));
		this->dump(i);
#endif
	}

	void file::dump(double d)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
#ifndef __BIG_ENDIAN__
		fwrite((unsigned char*)&d, 1, 8, this->cfile);
#else
		int halfSize = sizeof(d) / 2;
		unsigned int i;
		memcpy(&i, &d, halfSize);
		this->dump(i);
		memcpy(&i, (unsigned char*)&d + halfSize, halfSize);
		this->dump(i);
#endif
	}

	void file::dump(bool b)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c = (b ? 1 : 0);
		fwrite(&c, 1, 1, this->cfile);
	}

	void file::dump(chstr str)
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
				char c[1024] = {'\0'};
				for (int i = 0; i < size; i++)
				{
					c[i] = string[i] - this->encryption_offset;
				}
				fwrite(c, 1, size, this->cfile);
			}
		}
	}

	void file::dump(const char* c)
	{
		this->dump(hstr(c));
	}

/******* SERIALIZATION LOAD ********************************************/

	unsigned char file::load_uchar()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c;
		fread(&c, 1, 1, this->cfile);
		return c;
	}

	int file::load_int()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return (int)this->load_uint();
	}

	unsigned int file::load_uint()
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

	short file::load_short()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return (int)this->load_ushort();
	}

	unsigned short file::load_ushort()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned short s = 0;
#ifndef __BIG_ENDIAN__
		fread((unsigned char*)&s, 1, 2, this->cfile);
#else
		unsigned char bytes[4] = {0};
		fread(bytes, 1, 2, this->cfile);
		s |= bytes[1] << 8;
		s |= bytes[0];
#endif
		return s;
	}

	float file::load_float()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		float f;
#ifndef __BIG_ENDIAN__
		fread((unsigned char*)&f, 1, 4, this->cfile);
#else
		unsigned int i = this->load_uint();
		memcpy(&f, &i, sizeof(f));
#endif
		return f;
	}

	double file::load_double()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		double d;
#ifndef __BIG_ENDIAN__
		fread((unsigned char*)&d, 1, 8, this->cfile);
#else
		int halfSize = sizeof(d) / 2;
		unsigned int i = this->load_uint();
		memcpy((unsigned char*)&d, &i, halfSize);
		i = this->load_uint();
		memcpy((unsigned char*)&d + halfSize, &i, halfSize);
#endif
		return d;
	}

	bool file::load_bool()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c;
		fread(&c, 1, 1, this->cfile);
		return (c != 0);
	}

	hstr file::load_hstr()
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

/******* STATIC ********************************************************/

	bool file::create(chstr filename)
	{
		hstr name = normalize_path(filename);
		if (!hfile::exists(name))
		{
			hdir::create_path(name);
			FILE* f = fopen(name.c_str(), "wb");
			if (f != NULL)
			{
				fclose(f);
				return true;
			}
		}
		return false;
	}
	
	bool file::create_new(chstr filename)
	{
		return (hfile::create(filename) || hfile::clear(filename));
	}
	
	bool file::remove(chstr filename)
	{
		hstr name = normalize_path(filename);
		return (f_remove(name.c_str()) == 0);
	}
	
	bool file::exists(chstr filename)
	{
		hstr name = normalize_path(filename);
		FILE* f = fopen(name.c_str(), "r");
		if (f != NULL)
		{
			fclose(f);
			return true;
		}
		return false;
	}
	
	bool file::clear(chstr filename)
	{
		hstr name = normalize_path(filename);
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
	
	bool file::rename(chstr old_filename, chstr new_filename)
	{
		hstr old_name = normalize_path(old_filename);
		hstr new_name = normalize_path(new_filename);
		if (!hfile::exists(old_name) || hfile::exists(new_name))
		{
			return false;
		}
		hdir::create_path(new_name);
		return (f_rename(old_name.c_str(), new_name.c_str()) == 0);
	}
	
	bool file::move(chstr filename, chstr path)
	{
		hstr name = normalize_path(filename);
		return hfile::rename(name, path + "/" + name.rsplit("/", 1).pop_back());
	}
	
	bool file::copy(chstr old_filename, chstr new_filename)
	{
		hstr old_name = normalize_path(old_filename);
		hstr new_name = normalize_path(new_filename);
		if (!hfile::exists(old_name) || hfile::exists(new_name))
		{
			return false;
		}
		hdir::create_path(new_name);
		hfile old_file(old_name);
		hfile new_file(new_name, hltypes::WRITE);
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

	hstr file::hread(chstr filename, int count)
	{
		hstr name = normalize_path(filename);
		return hfile(name).read(count);
	}
	
	hstr file::hread(chstr filename, chstr delimiter)
	{
		hstr name = normalize_path(filename);
		return hfile(name).read(delimiter);
	}
	
	void file::hwrite(chstr filename, chstr text)
	{
		hstr name = normalize_path(filename);
		hfile(name, WRITE).write(text);
	}
	
	void file::happend(chstr filename, chstr text)
	{
		hstr name = normalize_path(filename);
		hfile(name, APPEND).write(text);
	}
	
	long file::hsize(chstr filename)
	{
		hstr name = normalize_path(filename);
		return hfile(name).size();
	}
	
}