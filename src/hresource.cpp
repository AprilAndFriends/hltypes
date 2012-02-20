/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _ANDROID
#include <stdio.h>
#include <zip/zip.h>
#endif

#include "exception.h"
#include "hdir.h"
#include "hresource.h"
#ifdef _ANDROID
#include "hstring.h"
#include "hthread.h"
#endif

namespace hltypes
{
#ifdef _ANDROID
	float Resource::timeout = 100.0f;
	int Resource::repeats = 0;
	hstr Resource::cwd = "assets";
	hstr Resource::archive = "";

	Resource::Resource(chstr filename) : StreamBase(), archivefile(NULL), cfile(NULL)
	{
		this->data_position = 0;
		this->filename = normalize_path(filename);
		this->open(filename);
	}
	
	Resource::Resource() : StreamBase(), archivefile(NULL), cfile(NULL)
	{
		this->data_position = 0;
	}

	Resource::~Resource()
	{
		if (this->is_open())
		{
			this->close();
		}
	}
#else
	Resource::Resource(chstr filename) : File(filename)
	{
	}
	
	Resource::Resource() : File()
	{
	}

	Resource::~Resource()
	{
	}
#endif
#include <android/log.h>
	void Resource::open(chstr filename)
	{
#ifndef _ANDROID
		File::open(filename);
#else
		if (this->is_open())
		{
			this->close();
		}
		this->filename = normalize_path(filename);
		this->encryption_offset = 0;
		const char* mode = "rb";
		int attempts = Resource::repeats + 1;
		while (true)
		{
			this->archivefile = zip_open(Resource::archive.c_str(), 0, NULL);
			if (this->archivefile != NULL)
			{
				this->cfile = zip_fopen(this->archivefile, Resource::_make_full_filename(this->filename).c_str(), 0);
				if (this->cfile != NULL)
				{
					break;
				}
				// file wasn't found so let's rather close the archive
				zip_close(this->archivefile);
				this->archivefile = NULL;
			}
			attempts--;
			if (attempts <= 0)
			{
				break;
			}
			Thread::sleep(Resource::timeout);
		}
		if (this->archivefile == NULL)
		{
			throw file_not_found(this->_descriptor());
		}
		if (this->cfile == NULL)
		{
			throw file_not_found(this->_descriptor());
		}
		this->_update_data_size();
#endif
	}
	
#ifdef _ANDROID
	void Resource::close()
	{
		this->_check_availability();
		zip_fclose(this->cfile);
		this->cfile = NULL;
		zip_close(this->archivefile);
		this->archivefile = NULL;
		this->data_size = 0;
		this->data_position = 0;
	}
	
	void Resource::_update_data_size()
	{
		struct zip_stat stat;
		stat.size = 0;
		zip_stat(this->archivefile, Resource::_make_full_filename(this->filename).c_str(), 0, &stat);
		this->data_size = stat.size;
	}

	hstr Resource::_descriptor()
	{
		return this->filename;
	}
	
	long Resource::_read(void* buffer, int size, int count)
	{
		int readCount = size * count;
		this->data_position += readCount;
		return zip_fread(this->cfile, buffer, readCount);
	}
#endif
	
	long Resource::_write(const void* buffer, int size, int count)
	{
		throw resource_not_writeable(this->filename);
	}
	
#ifdef _ANDROID
	bool Resource::_is_open()
	{
		return (this->archivefile != NULL && this->cfile != NULL);
	}
	
	long Resource::_position()
	{
		return this->data_position;
	}
	
	void Resource::_seek(long offset, SeekMode seek_mode)
	{
		throw resource_not_seekable(this->filename);
	}
	
	bool Resource::exists(chstr filename)
	{
		bool result = false;
		struct zip* a = zip_open(Resource::archive.c_str(), 0, NULL);
		if (a != NULL)
		{
			hstr name = normalize_path(Resource::_make_full_filename(filename));
			struct zip_file* f = zip_fopen(a, name.c_str(), 0);
			if (f != NULL)
			{
				zip_fclose(f);
				result = true;
			}
			zip_close(a);
		}
		return result;
	}
	
	long Resource::hsize(chstr filename)
	{
		return Resource(filename).size();
	}
	
	hstr Resource::hread(chstr filename, int count)
	{
		return Resource(filename).read(count);
	}
	
	hstr Resource::hread(chstr filename, chstr delimiter)
	{
		return Resource(filename).read(delimiter);
	}

	hstr Resource::_make_full_filename(chstr filename)
	{
		return normalize_path(Resource::cwd + "/" + filename);
	}
#endif

}

