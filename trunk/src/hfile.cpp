/// @file
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>

// prevents recursive calls of hfile::rename and hfile::remove as these functions are called via these pointers
int (*f_rename)(const char* old_name, const char* new_name) = rename;
int (*f_remove)(const char* filename) = remove;

#include "exception.h"
#include "hdir.h"
#include "hfile.h"
#include "hlog.h"
#include "hstring.h"
#include "hthread.h"

#if !defined _WIN32 && !defined(_WINRT)
#include <sys/stat.h>
#endif

#define BUFFER_SIZE 4096

namespace hltypes
{
	File::File(const String& filename, AccessMode access_mode) : FileBase(filename)
	{
		hlog::warnf(hltypes::logTag, "Opening file '%s' in hfile constructor is deprecated and unsafe! Use hfile::open() instead.", filename.c_str());
		this->open(filename, access_mode);
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
	
	void File::open(const String& filename, AccessMode access_mode)
	{
		this->_fopen(filename, access_mode, FileBase::repeats, FileBase::timeout);
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
		String name = Dir::normalize(filename);
		if (!File::exists(name))
		{
			Dir::create(Dir::basedir(name));
			int attempts = File::repeats + 1;
			while (true)
			{
#ifdef _WIN32
				FILE* f = _wfopen(name.w_str().c_str(), L"wb");
#else
				FILE* f = fopen(name.c_str(), "wb"); // TODO - should be ported to Unix systems as well
#endif
				if (f != NULL)
				{
					fclose(f);
					return true;
				}
				--attempts;
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
		String name = Dir::normalize(filename);
#ifdef _WIN32
		return (_wremove(name.w_str().c_str()) == 0);
#else
		return (f_remove(name.c_str()) == 0); // TODO - should be ported to Unix systems as well
#endif
	}
	
	bool File::exists(const String& filename, bool case_sensitive) // such an sensitive method
	{
		return FileBase::_fexists(filename, case_sensitive);
	}
	
	bool File::clear(const String& filename)
	{
		String name = Dir::normalize(filename);
		if (File::exists(name))
		{
#ifdef _WIN32
			FILE* f = _wfopen(name.w_str().c_str(), L"wb");
#else
			FILE* f = fopen(name.c_str(), "wb"); // TODO - should be ported to Unix systems as well
#endif
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
		String old_name = Dir::normalize(old_filename);
		String new_name = Dir::normalize(new_filename);
		if (!File::exists(old_name))
		{
			return false;
		}
		if (File::exists(new_name))
		{
			if (!overwrite)
			{
				return false;
			}
			File::remove(new_name);
		}
		Dir::create(Dir::basedir(new_name));
#ifdef _WIN32
		return (_wrename(old_name.w_str().c_str(), new_name.w_str().c_str()) == 0);
#else
		return (f_rename(old_name.c_str(), new_name.c_str()) == 0); // TODO - should be ported to Unix systems as well
#endif
	}
	
	bool File::move(const String& filename, const String& path, bool overwrite)
	{
		String name = Dir::normalize(filename);
		return File::rename(name, Dir::join_path(path, Dir::basename(name), false), overwrite);
	}
	
	bool File::copy(const String& old_filename, const String& new_filename, bool overwrite)
	{
		String old_name = Dir::normalize(old_filename);
		String new_name = Dir::normalize(new_filename);
		if (!File::exists(old_name) || !overwrite && File::exists(new_name))
		{
			return false;
		}
		Dir::create(Dir::basedir(new_name));
		File old_file;
		File new_file;
		old_file.open(old_name);
		new_file.open(new_name, File::WRITE);
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
		return File::get_info(filename).size; // uses get_info to avoid opening the file
	}
	
	String File::hread(const String& filename, int count)
	{
		File file;
		file.open(filename);
		return file.read(count);
	}
	
	String File::hread(const String& filename, const String& delimiter)
	{
		File file;
		file.open(filename);
		return file.read(delimiter);
	}
	
	void File::hwrite(const String& filename, const String& text)
	{
		File file;
		file.open(filename, WRITE);
		file.write(text);
	}
	
	void File::happend(const String& filename, const String& text)
	{
		File file;
		file.open(filename, APPEND);
		file.write(text);
	}

	FileInfo File::get_info(const String& filename)
	{
		FileInfo info;
#if defined(_WIN32)
		WIN32_FILE_ATTRIBUTE_DATA data;
		memset(&data, 0, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
		if (GetFileAttributesExW(filename.w_str().c_str(), GetFileExInfoStandard, &data) != 0)
		{
			#define WINDOWS_TICK 10000000ULL
			#define SEC_TO_UNIX_EPOCH 11644473600ULL

			info.size = data.nFileSizeLow;
			ULARGE_INTEGER ull;
			ull.LowPart = data.ftCreationTime.dwLowDateTime;
			ull.HighPart = data.ftCreationTime.dwHighDateTime;
			info.creation_time = (unsigned long)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
			ull.LowPart = data.ftLastAccessTime.dwLowDateTime;
			ull.HighPart = data.ftLastAccessTime.dwHighDateTime;
			info.access_time = (unsigned long)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
			ull.LowPart = data.ftLastWriteTime.dwLowDateTime;
			ull.HighPart = data.ftLastWriteTime.dwHighDateTime;
			info.modification_time = (unsigned long)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
		}
#else
		struct stat s;
		stat(filename.c_str(), &s);
		info.size = s.st_size;
		info.creation_time = s.st_ctime;
		info.access_time = s.st_atime;
		info.modification_time = s.st_mtime;
#endif
		return info;
	}

}
