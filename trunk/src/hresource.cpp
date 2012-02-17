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
		this->filename = normalize_path(filename);
		this->open(filename);
	}
	
	Resource::Resource() : StreamBase(), archivefile(NULL), cfile(NULL)
	{
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
				this->cfile = zip_fopen(this->archivefile, this->_get_full_filename().c_str(), 0);
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
		};
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
	}
	
	hstr Resource::_descriptor()
	{
		return this->filename;
	}
	
	hstr Resource::_get_full_filename()
	{
		return (Resource::cwd + "/" + this->filename);
	}
	
	long Resource::_read(void* buffer, int size, int count)
	{
		return zip_fread(this->cfile, buffer, size * count);
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
		struct zip_stat stat;
		stat.size = 0;
		zip_stat(this->archivefile, this->_get_full_filename().c_str(), 0, &stat);
		return stat.size;
	}
	
	void Resource::_seek(long offset, SeekMode seek_mode)
	{
		throw resource_not_seekable(this->filename);
	}
	
	bool Resource::exists(chstr filename)
	{
		hstr name = normalize_path(filename);
		FILE* f = fopen(name.c_str(), "rb");
		if (f != NULL)
		{
			fclose(f);
			return true;
		}
		return false;
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
#endif

}

