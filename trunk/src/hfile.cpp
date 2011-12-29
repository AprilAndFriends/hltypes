/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 1.5
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
	float File::timeout = 100.0f;
	int File::repeats = 0;
/******* CONSTRUCT/DESTRUCT ********************************************/

	File::File(chstr filename, AccessMode access_mode, unsigned char encryption_offset) : StreamBase(encryption_offset), cfile(NULL)
	{
		this->filename = normalize_path(filename);
#ifdef NO_FS_TREE
		this->filename = this->filename.replace("/", "___");
#endif
		this->open(filename, access_mode, encryption_offset);
	}
	
	File::File() : StreamBase(), filename(""), cfile(NULL)
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
			throw file_not_found(this->_descriptor());
		}
		this->_update_data_size();
	}
	
	void File::close()
	{
		this->_check_availability();
		fclose(this->cfile);
		this->cfile = NULL;
		this->data_size = 0;
	}
	
	hstr File::_descriptor()
	{
		return this->filename;
	}
	
	long File::_fread(void* buffer, int size, int count)
	{
		return fread(buffer, size, count, this->cfile);
	}
	
	long File::_fwrite(const void* buffer, int size, int count)
	{
		return fwrite(buffer, size, count, this->cfile);
	}
	
	bool File::_is_open()
	{
		return (this->cfile != NULL);
	}
	
	long File::_position()
	{
		return ftell(this->cfile);
	}
	
	void File::_seek(long offset, SeekMode seek_mode)
	{
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
