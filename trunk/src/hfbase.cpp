/// @file
/// @author  Boris Mikic
/// @version 1.55
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifndef _WIN32
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "hfbase.h"
#include "hstring.h"
#include "hthread.h"

namespace hltypes
{
	int FileBase::repeats = 0;
	float FileBase::timeout = 100.0f;

	FileBase::FileBase(unsigned char encryption_offset) : StreamBase(encryption_offset), cfile(NULL)
	{
	}
	
	FileBase::~FileBase()
	{
		// this measure is not universal for all derived classes!
		if (this->_fis_open())
		{
			this->_fclose();
		}
	}
	
	hstr FileBase::_descriptor()
	{
		return this->filename;
	}
	
	void FileBase::_fopen(chstr filename, AccessMode access_mode, unsigned char encryption_offset, int repeats, float timeout)
	{
		if (this->is_open())
		{
			this->_fclose();
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
		int attempts = repeats + 1;
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
			Thread::sleep(timeout);
		};
		if (this->cfile == NULL)
		{
			throw file_not_found(this->_descriptor());
		}
		this->_update_data_size();
	}
	
	void FileBase::_fclose()
	{
		this->_check_availability();
		fclose((FILE*)this->cfile);
		this->cfile = NULL;
		this->data_size = 0;
	}
	
	long FileBase::_fread(void* buffer, int size, int count)
	{
		return fread(buffer, size, count, (FILE*)this->cfile);
	}
	
	long FileBase::_fwrite(const void* buffer, int size, int count)
	{
		return fwrite(buffer, size, count, (FILE*)this->cfile);
	}
	
	bool FileBase::_fis_open()
	{
		return (this->cfile != NULL);
	}
	
	long FileBase::_fposition()
	{
		return ftell((FILE*)this->cfile);
	}
	
	void FileBase::_fseek(long offset, SeekMode seek_mode)
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
		fseek((FILE*)this->cfile, offset, mode);
	}
	
	bool FileBase::_fexists(chstr filename)
	{
		hstr name = normalize_path(filename);
		bool result = false;
		FILE* f = fopen(name.c_str(), "rb");
		if (f != NULL)
		{
			fclose(f);
			result = true;
#ifndef _WIN32
			struct stat stats;
			// on UNIX fopen on a directory actually works so this check prevents
			// errorously reporting the existence of a file if it's a directory
			if (stat(name.c_str(), &stats) == 0 && (stats.st_mode & S_IFMT) == S_IFDIR)
			{
				result = false;
			}
#endif
		}
		return result;
	}
	
}
