/// @file
/// @author  Boris Mikic
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "exception.h"
#include "hdir.h"
#include "hfile.h"
#include "hrdir.h"
#include "hresource.h"
#include "hthread.h"
#ifdef _ZIPRESOURCE
#include "zipaccess.h"
#endif

namespace hltypes
{
#ifndef _ZIPRESOURCE
	String Resource::cwd = ".";
#else
	String Resource::cwd = "assets";
#define READ_BUFFER_SIZE 65536
	static unsigned char _read_buffer[READ_BUFFER_SIZE] = {0};
#endif
	String Resource::archive = "";

	void Resource::setArchive(const String& value)
	{
#ifdef _ZIPRESOURCE
		if (archive != value)
		{
			hrdir::cacheDirectories.clear();
			hrdir::cacheFiles.clear();
		}
		zip::setArchive(value);
#endif
		archive = value;
	}

	Resource::Resource(const String& filename) : FileBase(filename), data_position(0), archivefile(NULL)
	{
		this->open(this->filename);
	}
	
	Resource::Resource() : FileBase(), data_position(0), archivefile(NULL)
	{
	}

	Resource::~Resource()
	{
		if (this->is_open())
		{
			this->close();
		}
	}
	
	void Resource::open(const String& filename)
	{
#ifndef _ZIPRESOURCE
		this->_fopen(Resource::make_full_path(filename), READ, 0, FileBase::repeats, FileBase::timeout);
#else
		if (this->is_open())
		{
			this->close();
		}
		this->filename = Dir::normalize(filename);
		this->encryption_offset = 0;
		int attempts = Resource::repeats + 1;
		while (true)
		{
			this->archivefile = zip::open(this);
			if (this->archivefile != NULL)
			{
				this->cfile = zip::fopen(this->archivefile, Resource::make_full_path(this->filename));
				if (this->cfile != NULL)
				{
					break;
				}
				// file wasn't found so let's rather close the archive
				zip::close(this, this->archivefile);
				this->archivefile = NULL;
			}
			--attempts;
			if (attempts <= 0)
			{
				break;
			}
			Thread::sleep(Resource::timeout);
		}
		if (this->archivefile == NULL)
		{
			throw resource_not_found(this->_descriptor());
		}
		if (this->cfile == NULL)
		{
			throw resource_not_found(this->_descriptor());
		}
		this->_update_data_size();
#endif
	}
	
	void Resource::close()
	{
#ifndef _ZIPRESOURCE
		this->_fclose();
#else
		this->_check_availability();
		zip::fclose(this->cfile);
		this->cfile = NULL;
		zip::close(this, this->archivefile);
		this->archivefile = NULL;
		this->data_size = 0;
#endif
		this->data_position = 0;
	}
	
	bool Resource::hasZip()
	{
#ifndef _ZIPRESOURCE
		return false;
#else
		return true;
#endif
	}

	void Resource::_update_data_size()
	{
#ifndef _ZIPRESOURCE
		long position = this->_position();
		this->_fseek(0, END);
		this->data_size = this->_position();
		this->_fseek(position, START);
#else
		this->data_size = zip::fsize(this->archivefile, this->filename);
#endif
	}

	long Resource::_read(void* buffer, int size, int count)
	{
#ifndef _ZIPRESOURCE
		return this->_fread(buffer, size, count);
#else
		int read_count = size * count;
		this->data_position += read_count;
		return zip::fread(this->cfile, buffer, read_count);
#endif
	}
	
	long Resource::_write(const void* buffer, int size, int count)
	{
		throw file_not_writeable(this->filename);
	}

	bool Resource::_is_open()
	{
#ifndef _ZIPRESOURCE
		return this->_fis_open();
#else
		return (this->archivefile != NULL && this->cfile != NULL);
#endif
	}
	
	long Resource::_position()
	{
#ifndef _ZIPRESOURCE
		return this->_fposition();
#else
		return this->data_position;
#endif
	}
	
	void Resource::_seek(long offset, SeekMode seek_mode)
	{
#ifndef _ZIPRESOURCE
		this->_fseek(offset, seek_mode);
#else
		// zip can only read forward and doesn't really have seeking
		long target = offset;
		switch (seek_mode)
		{
		case CURRENT:
			target = offset + this->data_position;
			break;
		case START:
			target = offset;
			break;
		case END:
			target = this->data_size + offset;
			break;
		}
		if (target >= this->data_position)
		{
			target -= this->data_position;
		}
		else
		{
			// reopening the file as the target position was already passed
			this->cfile = zip::freopen(this->cfile, this->archivefile, Resource::make_full_path(this->filename));
			this->data_position = 0;
		}
		if (target > 0)
		{
			// seeking in a compressed stream is not possible so the data has to be read and then discarded
			// the buffer can be static, because this data isn't used so there will be no threading problems
			unsigned char* buffer = _read_buffer;
			if (target > READ_BUFFER_SIZE)
			{
				buffer = new unsigned char[target];
			}
			this->_read(buffer, 1, target);
			if (target > READ_BUFFER_SIZE)
			{
				delete [] buffer;
			}
		}
#endif
	}
	
	bool Resource::exists(const String& filename, bool case_sensitive)
	{
#ifndef _ZIPRESOURCE
		return FileBase::_fexists(Resource::make_full_path(filename), case_sensitive);
#else
		bool result = false;
		void* a = zip::open(NULL); // NULL, because this is a static function which will close the archive right after it is done
		if (a != NULL)
		{
			void* f = zip::fopen(a, Resource::make_full_path(filename));
			if (f != NULL)
			{
				zip::fclose(f);
				result = true;
			}
			if (!result && !case_sensitive)
			{
				hstr name = filename;
				hstr basedir = ResourceDir::basedir(name);
				hstr basename = ResourceDir::basename(name);
				Array<String> files = ResourceDir::files(basedir);
				foreach (String, it, files)
				{
					if ((*it).lower() == basename.lower())
					{
						name = ResourceDir::join_path(basedir, (*it));
						result = true;
						break;
					}
				}
			}
			zip::close(NULL, a);
		}
		return result;
#endif
	}
	
	long Resource::hsize(const String& filename)
	{
		return Resource(filename).size();
	}
	
	String Resource::hread(const String& filename, int count)
	{
		return Resource(filename).read(count);
	}
	
	String Resource::hread(const String& filename, const String& delimiter)
	{
		return Resource(filename).read(delimiter);
	}

	FileInfo Resource::get_info(const String& filename)
	{
#ifndef _ZIPRESOURCE
		return File::get_info(Resource::make_full_path(filename));
#else
		FileInfo info;
		bool result = false;
		void* a = zip::open(NULL); // NULL, because this is a static function which will close the archive right after it is done
		if (a != NULL)
		{
			info = zip::finfo(a, Resource::make_full_path(filename));
			zip::close(NULL, a);
			FileInfo archive = File::get_info(Resource::archive);
			info.creation_time = archive.creation_time;
			info.access_time = archive.access_time;
		}
		return info;
#endif
	}

	String Resource::make_full_path(const String& filename)
	{
		return Dir::normalize(hdir::join_path(Resource::cwd, filename));
	}

}

