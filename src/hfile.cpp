/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdio.h>

// prevents recursive calls of hfile::rename and hfile::remove as these functions are called via these pointers
int (*f_rename)(const char* old_name, const char* new_name) = rename;
int (*f_remove)(const char* filename) = remove;

#include "exception.h"
#include "hdir.h"
#include "hfile.h"
#include "hstring.h"
#include "hthread.h"

#define BUFFER_SIZE 4096

namespace hltypes
{
	File::File(const String& filename, AccessMode access_mode, unsigned char encryption_offset) : FileBase(encryption_offset)
	{
		this->filename = normalize_path(filename);
		this->open(filename, access_mode, encryption_offset);
	}
	
	File::File() : FileBase()
	{
	}
	
	File::~File()
	{
		if (this->is_open())
		{
			this->close();
		}
	}
	
	void File::open(const String& filename, AccessMode access_mode, unsigned char encryption_offset)
	{
		this->_fopen(filename, access_mode, encryption_offset, FileBase::repeats, FileBase::timeout);
	}
	
	void File::close()
	{
		this->_fclose();
	}
	
	long File::_read(void* buffer, int size, int count)
	{
		return this->_fread(buffer, size, count);
	}
	
	long File::_write(const void* buffer, int size, int count)
	{
		return this->_fwrite(buffer, size, count);
	}
	
	bool File::_is_open()
	{
		return this->_fis_open();
	}
	
	long File::_position()
	{
		return this->_fposition();
	}
	
	void File::_seek(long offset, SeekMode seek_mode)
	{
		this->_fseek(offset, seek_mode);
	}
	
	bool File::create(const String& filename)
	{
		String name = normalize_path(filename);
		if (!File::exists(name))
		{
			Dir::create_path(name);
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
	
	bool File::create_new(const String& filename)
	{
		return (File::create(filename) || File::clear(filename));
	}
	
	bool File::remove(const String& filename)
	{
		String name = normalize_path(filename);
		return (f_remove(name.c_str()) == 0);
	}
	
	bool File::exists(const String& filename)
	{
		return FileBase::_fexists(filename);
	}
	
	bool File::clear(const String& filename)
	{
		String name = normalize_path(filename);
		if (File::exists(name))
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
	
	bool File::rename(const String& old_filename, const String& new_filename, bool overwrite)
	{
		String old_name = normalize_path(old_filename);
		String new_name = normalize_path(new_filename);
		if (!File::exists(old_name) || !overwrite && File::exists(new_name))
		{
			return false;
		}
		Dir::create_path(new_name);
		return (f_rename(old_name.c_str(), new_name.c_str()) == 0);
	}
	
	bool File::move(const String& filename, const String& path, bool overwrite)
	{
		String name = normalize_path(filename);
		return File::rename(name, path + "/" + name.rsplit("/", 1, false).remove_last(), overwrite);
	}
	
	bool File::copy(const String& old_filename, const String& new_filename, bool overwrite)
	{
		String old_name = normalize_path(old_filename);
		String new_name = normalize_path(new_filename);
		if (!File::exists(old_name) || !overwrite && File::exists(new_name))
		{
			return false;
		}
		Dir::create_path(new_name);
		File old_file(old_name);
		File new_file(new_name, File::WRITE);
		int count;
		unsigned char c[BUFFER_SIZE] = {0};
		while (!old_file.eof())
		{
			count = fread(c, 1, BUFFER_SIZE, (FILE*)old_file.cfile);
			fwrite(c, 1, count, (FILE*)new_file.cfile);
		}
		return true;
	}
	
	long File::hsize(const String& filename)
	{
		return File(filename).size();
	}
	
	String File::hread(const String& filename, int count)
	{
		return File(filename).read(count);
	}
	
	String File::hread(const String& filename, const String& delimiter)
	{
		return File(filename).read(delimiter);
	}
	
	void File::hwrite(const String& filename, const String& text)
	{
		File(filename, WRITE).write(text);
	}
	
	void File::happend(const String& filename, const String& text)
	{
		File(filename, APPEND).write(text);
	}
	
}
