/// @file
/// @version 2.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hlog.h"
#include "hltypesUtil.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	_Exception::_Exception(const String& message, const char* sourceFile, int lineNumber)
	{
		this->_setInternalMessage(message, sourceFile, lineNumber);
	}
	
	_Exception::~_Exception()
	{
	}

	void _Exception::_setInternalMessage(const String& message, const char* sourceFile, int lineNumber)
	{
		this->message = hsprintf("[%s:%d] %s", Dir::basename(sourceFile).c_str(), lineNumber, message.c_str());
#ifdef _WINRT // because Visual Studio on WinRT cannot properly display exceptions and stack traces for some reason even though it should
		if (Log::isLevelDebug() && message != "")
		{
			hltypes::_platformPrint("FATAL", this->message, 1000);
		}
#endif
	}

	_FileCouldNotOpenException::_FileCouldNotOpenException(const String& filename, bool isResource, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		String message = hsprintf("'%s' could not be opened!", filename.c_str());
		try
		{
			String dir = DirBase::basedir(filename);
			String basename = DirBase::basename(filename);
			Array<String> files = (!isResource ? Dir::files(dir) : ResourceDir::files(dir));
			foreach (String, it, files)
			{
				if ((*it) == basename)
				{
					message += " File appears to be in use.";
					throw _Exception("", "", 0);
				}
				if ((*it).lower() == basename.lower())
				{
					message += " But there is a file with a different case: " + Dir::join_path(dir, (*it));
					throw _Exception("", "", 0);
				}
			}
			Array<String> dirs = Dir::split_path(dir);
			String path;
			Array<String> directories;
			for_iter (i, 0, dirs.size())
			{
				path = Dir::join_paths(dirs(0, dirs.size() - i));
				if (!isResource ? !Dir::exists(path) && Dir::exists(path, false) : !ResourceDir::exists(path) && ResourceDir::exists(path, false))
				{
					dir = Dir::join_paths(dirs(0, dirs.size() - 1 - i));
					basename = dirs[dirs.size() - 1 - i];
					Array<String> directories = (!isResource ? Dir::directories(dir) : ResourceDir::directories(dir));
					foreach (String, it, directories)
					{
						if ((*it).lower() == basename.lower())
						{
							basename = (*it);
							break;
						}
					}
					message += " But part of the path seems to have a different case: " + Dir::join_path(dir, basename);
					throw _Exception("", "", 0);
				}
			}
			message += " File not found!";
		}
		catch (_Exception&) // is this inception or exception, I am confused
		{
		}
#ifdef _WIN32 // could be useful
		message += " System error: " + String(strerror(errno));
#endif
		this->_setInternalMessage(message, sourceFile, lineNumber);
	}
	_FileCouldNotOpenException::~_FileCouldNotOpenException()
	{
	}
	
	_FileNotOpenException::_FileNotOpenException(const String& filename, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("'%s' is not open!", filename.c_str()), sourceFile, lineNumber);
	}
	_FileNotOpenException::~_FileNotOpenException()
	{
	}
	
	_FileNotWriteableException::_FileNotWriteableException(const String& filename, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("'%s' is not writeable!", filename.c_str()), sourceFile, lineNumber);
	}
	_FileNotWriteableException::~_FileNotWriteableException()
	{
	}
	
	_ResourceNotExistsException::_ResourceNotExistsException(const String& type, const String& name, const String& container, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("'%s' '%s' does not exist in '%s'", name.c_str(), type.c_str(), container.c_str()), sourceFile, lineNumber);
	}
	_ResourceNotExistsException::~_ResourceNotExistsException()
	{
	}

	_ResourceAlreadyExistsException::_ResourceAlreadyExistsException(const String& type, const String& name, const String& container, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("'%s' '%s' already exists in '%s'", name.c_str(), type.c_str(), container.c_str()), sourceFile, lineNumber);
	}
	_ResourceAlreadyExistsException::~_ResourceAlreadyExistsException()
	{
	}

	_ContainerIndexException::_ContainerIndexException(int index, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("index '%d' out of range", index), sourceFile, lineNumber);
	}
	_ContainerIndexException::~_ContainerIndexException()
	{
	}
	
	_ContainerEmptyException::_ContainerEmptyException(const String& functionName, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("'%s' cannot be used on a container with size = 0", functionName.c_str()), sourceFile, lineNumber);
	}
	_ContainerEmptyException::~_ContainerEmptyException()
	{
	}
	
	_ContainerElementNotFoundException::_ContainerElementNotFoundException(const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("element not found in container ", sourceFile, lineNumber);
	}
	_ContainerElementNotFoundException::~_ContainerElementNotFoundException()
	{
	}
	
	_ContainerRangeException::_ContainerRangeException(int start, int count, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("range 'at %d for %d' out of range", start, count), sourceFile, lineNumber);
	}
	_ContainerRangeException::~_ContainerRangeException()
	{
	}
	
	_ContainerKeyException::_ContainerKeyException(const String& key, const String& container, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("key '%s' not found in '%s'", key.c_str(), container.c_str()), sourceFile, lineNumber);
	}
	_ContainerKeyException::~_ContainerKeyException()
	{
	}
	
}
