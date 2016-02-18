/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#define __HL_INCLUDE_PLATFORM_HEADERS
#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hlog.h"
#include "hplatform.h"
#include "hstring.h"
#include "hthread.h"
#include "platform_internal.h"

#define BUFFER_SIZE 65536

namespace hltypes
{
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
	
	void File::open(const String& filename)
	{
		this->_fopen(filename, READ, FileBase::repeats, FileBase::timeout);
		// initial size must be set manually with hinfo(), because _updateDataSize() uses seeking which requires the size again
		this->dataSize = File::hinfo(this->filename).size;
	}

	void File::open(const String& filename, AccessMode accessMode)
	{
		this->_fopen(filename, accessMode, FileBase::repeats, FileBase::timeout);
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
	
	bool File::_isOpen() const
	{
		return this->_fisOpen();
	}
	
	int64_t File::_position() const
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
			_platformFile* file = NULL;
			while (true)
			{
				file = _platformOpenFile(name, "wb");
				if (file != NULL)
				{
					_platformCloseFile(file);
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
		return _platformRemoveFile(Dir::normalize(filename));
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
			int attempts = File::repeats + 1;
			_platformFile* file = NULL;
			while (true)
			{
				file = _platformOpenFile(name, "wb");
				if (file != NULL)
				{
					_platformCloseFile(file);
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
		return _platformRenameFile(oldName, newName);
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
			count = _platformReadFile(c, 1, BUFFER_SIZE, (_platformFile*)oldFile.cfile);
			_platformWriteFile(c, 1, count, (_platformFile*)newFile.cfile);
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
		return _platformStatFile(filename);
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
