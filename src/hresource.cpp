/// @file
/// @version 2.32
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
	bool Resource::zipArchive = false;

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
#ifdef _ZIPRESOURCE
		void* a = zip::open(NULL); // NULL, because this is a static function which will close the archive right after it is done
		if (a != NULL)
		{
			zipArchive = true;
			zip::close(NULL, a);
		}
		else
		{
			zipArchive = false;
		}
#endif
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
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
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
		}
		else
#endif
		{
			this->_fopen(Resource::make_full_path(filename), READ, 0, FileBase::repeats, FileBase::timeout);
		}
	}
	
	void Resource::close()
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
			this->_check_availability();
			zip::fclose(this->cfile);
			this->cfile = NULL;
			zip::close(this, this->archivefile);
			this->archivefile = NULL;
			this->data_size = 0;
		}
		else
#endif
		{
			this->_fclose();
		}
		this->data_position = 0;
	}
	
	bool Resource::hasZip()
	{
#ifdef _ZIPRESOURCE
		return true;
#else
		return false;
#endif
	}

	void Resource::_update_data_size()
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
			this->data_size = zip::fsize(this->archivefile, this->filename);
		}
		else
#endif
		{
			long position = this->_position();
			this->_fseek(0, END);
			this->data_size = this->_position();
			this->_fseek(position, START);
		}
	}

	long Resource::_read(void* buffer, int size, int count)
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
			int read_count = size * count;
			this->data_position += read_count;
			return zip::fread(this->cfile, buffer, read_count);
		}
#endif
		return this->_fread(buffer, size, count);
	}
	
	long Resource::_write(const void* buffer, int size, int count)
	{
		throw file_not_writeable(this->filename);
	}

	bool Resource::_is_open()
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
			return (this->archivefile != NULL && this->cfile != NULL);
		}
#endif
		return this->_fis_open();
	}
	
	long Resource::_position()
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
			return this->data_position;
		}
#endif
		return this->_fposition();
	}
	
	void Resource::_seek(long offset, SeekMode seek_mode)
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
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
		}
		else
#endif
		{
			this->_fseek(offset, seek_mode);
		}
	}
	
	bool Resource::exists(const String& filename, bool case_sensitive)
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
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
		}
#endif
		return FileBase::_fexists(Resource::make_full_path(filename), case_sensitive);
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
#ifdef _ZIPRESOURCE
		if (Resource::zipArchive)
		{
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
		}
#endif
		return File::get_info(Resource::make_full_path(filename));
	}

	String Resource::make_full_path(const String& filename)
	{
		String path = hdir::join_path(Resource::cwd, filename);
		if (!Resource::zipArchive && Resource::archive != "" && Resource::archive != ".")
		{
			path = hdir::join_path(Resource::archive, path);
		}
		return Dir::normalize(path);
	}

}

