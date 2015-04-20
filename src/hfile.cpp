/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdio.h>

// prevents recursive calls of hfile::rename and hfile::remove as these functions are called via these pointers
int (*f_rename)(const char* oldName, const char* newName) = rename;
int (*f_remove)(const char* filename) = remove;

#include "hdir.h"
#include "hexception.h"
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
		hlog::warnf(hltypes::logTag, "Opening file '%s' in hfile constructor is deprecated and unsafe! Use hfile::open() instead.", filename.cStr());
		this->open(filename, access_mode);
	}
	
	File::File() : FileBase()
	{
	}
	
	File::~File()
	{
		if (this->_isOpen())
		{
			this->close();
		}
	}
	
	void File::open(const String& filename, AccessMode access_mode)
	{
		this->_fopen(filename, access_mode, FileBase::repeats, FileBase::timeout);
		// initial size must be set manually with hinfo(), because _updateDataSize() uses seeking which requires the size again
		this->dataSize = File::hinfo(this->filename).size;
	}
	
	void File::close()
	{
		this->_fclose();
	}
	
	int File::_read(void* buffer, int count)
	{
		return this->_fread(buffer, count);
	}
	
	int File::_write(const void* buffer, int count)
	{
		return this->_fwrite(buffer, count);
	}
	
	bool File::_isOpen()
	{
		return this->_fisOpen();
	}
	
	int64_t File::_position()
	{
		return this->_fposition();
	}
	
	bool File::_seek(int64_t offset, SeekMode seekMode)
	{
		return this->_fseek(offset, seekMode);
	}
	
	bool File::create(const String& filename)
	{
		String name = Dir::normalize(filename);
		if (!File::exists(name))
		{
			Dir::create(Dir::baseDir(name));
			int attempts = File::repeats + 1;
			while (true)
			{
#ifdef _WIN32
				FILE* f = _wfopen(name.wStr().c_str(), L"wb");
#else
				FILE* f = fopen(name.cStr(), "wb"); // TODO - should be ported to Unix systems as well
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
	
	bool File::createNew(const String& filename)
	{
		return (File::create(filename) || File::clear(filename));
	}
	
	bool File::remove(const String& filename)
	{
		String name = Dir::normalize(filename);
#ifdef _WIN32
		return (_wremove(name.wStr().c_str()) == 0);
#else
		return (f_remove(name.cStr()) == 0); // TODO - should be ported to Unix systems as well
#endif
	}
	
	bool File::exists(const String& filename, bool caseSensitive) // such an sensitive method
	{
		return FileBase::_fexists(filename, caseSensitive);
	}
	
	bool File::clear(const String& filename)
	{
		String name = Dir::normalize(filename);
		if (File::exists(name))
		{
#ifdef _WIN32
			FILE* f = _wfopen(name.wStr().c_str(), L"wb");
#else
			FILE* f = fopen(name.cStr(), "wb"); // TODO - should be ported to Unix systems as well
#endif
			if (f != NULL)
			{
				fclose(f);
				return true;
			}
		}
		return false;
	}
	
	bool File::rename(const String& oldFilename, const String& newFilename, bool overwrite)
	{
		String oldName = Dir::normalize(oldFilename);
		String newName = Dir::normalize(newFilename);
		if (!File::exists(oldName))
		{
			return false;
		}
		if (File::exists(newName))
		{
			if (!overwrite)
			{
				return false;
			}
			File::remove(newName);
		}
		Dir::create(Dir::baseDir(newName));
#ifdef _WIN32
		return (_wrename(oldName.wStr().c_str(), newName.wStr().c_str()) == 0);
#else
		return (f_rename(oldName.cStr(), newName.cStr()) == 0); // TODO - should be ported to Unix systems as well
#endif
	}
	
	bool File::move(const String& filename, const String& path, bool overwrite)
	{
		String name = Dir::normalize(filename);
		return File::rename(name, Dir::joinPath(path, Dir::baseName(name), false), overwrite);
	}
	
	bool File::copy(const String& oldFilename, const String& newFilename, bool overwrite)
	{
		String oldName = Dir::normalize(oldFilename);
		String newName = Dir::normalize(newFilename);
		if (!File::exists(oldName) || !overwrite && File::exists(newName))
		{
			return false;
		}
		Dir::create(Dir::baseDir(newName));
		File oldFile;
		File newFile;
		oldFile.open(oldName);
		newFile.open(newName, File::WRITE);
		int count = 0;
		unsigned char c[BUFFER_SIZE] = {0};
		while (!oldFile.eof())
		{
			count = (int)fread(c, 1, BUFFER_SIZE, (FILE*)oldFile.cfile);
			fwrite(c, 1, count, (FILE*)newFile.cfile);
		}
		return true;
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

	FileInfo File::hinfo(const String& filename)
	{
		FileInfo info;
#ifdef _WIN32
		WIN32_FILE_ATTRIBUTE_DATA data;
		memset(&data, 0, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
		if (GetFileAttributesExW(filename.wStr().c_str(), GetFileExInfoStandard, &data) != 0)
		{
#define WINDOWS_TICK 10000000ULL
#define SEC_TO_UNIX_EPOCH 11644473600ULL
			info.size = (int64_t)data.nFileSizeLow;
			ULARGE_INTEGER ull;
			ull.LowPart = data.ftCreationTime.dwLowDateTime;
			ull.HighPart = data.ftCreationTime.dwHighDateTime;
			info.creationTime = (int64_t)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
			ull.LowPart = data.ftLastAccessTime.dwLowDateTime;
			ull.HighPart = data.ftLastAccessTime.dwHighDateTime;
			info.accessTime = (int64_t)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
			ull.LowPart = data.ftLastWriteTime.dwLowDateTime;
			ull.HighPart = data.ftLastWriteTime.dwHighDateTime;
			info.modificationTime = (int64_t)(ull.QuadPart / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
		}
#else
		struct stat s;
		int ret = stat(filename.cStr(), &s);
		if (ret != 0)
		{
			if (!File::exists(filename))
			{
				throw FileCouldNotOpenException("stat() failed on '" + filename + "', file not found!");
			}
			else
			{
				throw FileCouldNotOpenException("stat() failed on '" + filename + "'!");
			}
		}
		info.size = (int64_t)s.st_size;
		info.creationTime = (int64_t)s.st_ctime;
		info.accessTime = (int64_t)s.st_atime;
		info.modificationTime = (int64_t)s.st_mtime;
#endif
		return info;
	}

	File::File(const File& other)
	{
		throw ObjectCannotCopyException("hltypes::File");
	}

	File& File::operator=(File& other)
	{
		throw ObjectCannotAssignException("hltypes::File");
		return (*this);
	}

}
