/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "hdir.h"
#include "hfbase.h"
#include "hstring.h"
#include "hthread.h"
#include "platform_internal.h"

namespace hltypes
{
	FileInfo::FileInfo() :
		size(-1LL),
		creationTime(0LL),
		accessTime(0LL),
		modificationTime(0LL)
	{
	}

	FileInfo::~FileInfo()
	{
	}

	HL_ENUM_CLASS_DEFINE(FileBase::AccessMode,
	(
		HL_ENUM_DEFINE(FileBase::AccessMode, Read);
		HL_ENUM_DEFINE(FileBase::AccessMode, Write);
		HL_ENUM_DEFINE(FileBase::AccessMode, Append);
		HL_ENUM_DEFINE(FileBase::AccessMode, ReadWrite);
		HL_ENUM_DEFINE(FileBase::AccessMode, ReadWriteCreate);
		HL_ENUM_DEFINE(FileBase::AccessMode, ReadAppend);
	));

	int FileBase::repeats = 0;
	float FileBase::timeout = 100.0f;

	void FileBase::setRepeats(int value)
	{
		FileBase::repeats = value;
	}

	void FileBase::setTimeout(float value)
	{
		FileBase::timeout = value;
	}

	FileBase::FileBase(const String& filename) :
		StreamBase(),
		cfile(NULL)
	{
		this->filename = Dir::normalize(filename);
	}
	
	FileBase::FileBase() :
		StreamBase(),
		cfile(NULL)
	{
	}
	
	FileBase::~FileBase()
	{
		// this measure is not universal for all derived classes!
		if (this->_fisOpen())
		{
			this->_fclose();
		}
	}

	String FileBase::extensionOf(const String& path)
	{
		// TODOhl - not UTF8 safe
		if (Dir::baseName(path).contains('.'))
		{
			int index = path.rindexOf('.');
			if (index >= 0)
			{
				return path(index + 1, -1);
			}
		}
		return "";
	}

	String FileBase::withoutExtension(const String& path)
	{
		// TODOhl - not UTF8 safe
		if (Dir::baseName(path).contains('.'))
		{
			int index = path.rindexOf('.');
			if (index >= 0)
			{
				return path.subString(0, index);
			}
		}
		return path;
	}
	
	String FileBase::_descriptor() const
	{
		return this->filename;
	}
	
	void FileBase::_fopen(const String& filename, AccessMode accessMode, int repeats, float timeout)
	{
		if (this->_isOpen())
		{
			this->_fclose();
		}
		this->filename = Dir::normalize(filename);
		String mode = "rb";
		if (accessMode == AccessMode::Read)
		{
			mode = "rb";
		}
		else if (accessMode == AccessMode::Write)
		{
			mode = "wb";
		}
		else if (accessMode == AccessMode::Append)
		{
			mode = "ab";
		}
		else if (accessMode == AccessMode::ReadWrite)
		{
			mode = "r+b";
		}
		else if (accessMode == AccessMode::ReadWriteCreate)
		{
			mode = "w+b";
		}
		else if (accessMode == AccessMode::ReadAppend)
		{
			mode = "a+b";
		}
		int attempts = repeats + 1;
		while (true)
		{
			this->cfile = _platformOpenFile(this->filename, mode);
			if (this->cfile != NULL)
			{
				break;
			}
			--attempts;
			if (attempts <= 0)
			{
				break;
			}
			Thread::sleep(timeout);
		}
		if (this->cfile == NULL)
		{
			throw FileCouldNotOpenException(this->_descriptor());
		}
	}
	
	void FileBase::_fclose()
	{
		this->_validate();
		_platformCloseFile(this->cfile);
		this->cfile = NULL;
		this->dataSize = 0;
	}
	
	int FileBase::_fread(void* buffer, int count)
	{
		return _platformReadFile(buffer, 1, count, this->cfile);
	}
	
	int FileBase::_fwrite(const void* buffer, int count)
	{
		int result = (int)_platformWriteFile(buffer, 1, count, (FILE*)this->cfile);
		this->dataSize = hmax(this->dataSize, this->_fposition());
		return result;
	}
	
	bool FileBase::_fisOpen() const
	{
		return (this->cfile != NULL);
	}
	
	int64_t FileBase::_fposition() const
	{
		return _platformGetFilePosition((_platformFile*)this->cfile);
	}
	
	bool FileBase::_fseek(int64_t offset, SeekMode seekMode)
	{
		return _platformSeekFile((_platformFile*)this->cfile, this->size(), this->_fposition(), offset, seekMode);
	}
	
	bool FileBase::_fexists(const String& filename, bool caseSensitive)
	{
		String name = Dir::normalize(filename);
		if (name == "")
		{
			return false;
		}
		if (_platformFileExists(name))
		{
			return true;
		}
		if (!caseSensitive)
		{
			String baseName = Dir::baseName(name).lowered();
			Array<String> files = Dir::files(Dir::baseDir(name));
			foreach (String, it, files)
			{
				if ((*it).lowered() == baseName)
				{
					return true;
				}
			}
		}
		return false;
	}
	
	FileBase::FileBase(const FileBase& other)
	{
		throw ObjectCannotCopyException("hltypes::FileBase");
	}

	FileBase& FileBase::operator=(FileBase& other)
	{
		throw ObjectCannotAssignException("hltypes::FileBase");
		return (*this);
	}

}
