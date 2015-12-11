/// @file
/// @version 3.0
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
#include "hplatform.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	_Exception::_Exception(const String& message, const char* sourceFile, int lineNumber)
	{
		this->stackTrace = hstackTrace();
		this->_setInternalMessage(message, sourceFile, lineNumber);
	}
	
	_Exception::~_Exception()
	{
	}

	void _Exception::_setInternalMessage(const String& message, const char* sourceFile, int lineNumber)
	{
		this->message = hsprintf("[%s:%d] %s", Dir::baseName(sourceFile).cStr(), lineNumber, message.cStr());
		// because Visual Studio on WinRT cannot properly display exceptions and stack traces for some reason even though it should
		// because Android doesn't display register data properly if an exception is thrown
#if defined(_WIN32) || defined(_ANDROID)
		if (Log::isLevelDebug() && message != "")
		{
			hltypes::_platformPrint("FATAL", this->message, Log::LevelError);
		}
#endif
	}

	_FileCouldNotOpenException::_FileCouldNotOpenException(const String& filename, bool isResource, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		String message = hsprintf("'%s' could not be opened!", filename.cStr());
		try
		{
			String baseDir = DirBase::baseDir(filename);
			String baseName = DirBase::baseName(filename);
			Array<String> files = (!isResource ? Dir::files(baseDir) : ResourceDir::files(baseDir));
			foreach (String, it, files)
			{
				if ((*it) == baseName)
				{
					message += " File appears to be in use.";
					throw _Exception("", "", 0);
				}
				if ((*it).lowered() == baseName.lowered())
				{
					message += " But there is a file with a different case: " + Dir::joinPath(baseDir, (*it));
					throw _Exception("", "", 0);
				}
			}
			Array<String> dirs = Dir::splitPath(baseDir);
			String path;
			Array<String> directories;
			for_iter (i, 0, dirs.size())
			{
				path = Dir::joinPaths(dirs(0, dirs.size() - i));
				if (!isResource ? !Dir::exists(path) && Dir::exists(path, false) : !ResourceDir::exists(path) && ResourceDir::exists(path, false))
				{
					baseDir = Dir::joinPaths(dirs(0, dirs.size() - 1 - i));
					baseName = dirs[dirs.size() - 1 - i];
					Array<String> directories = (!isResource ? Dir::directories(baseDir) : ResourceDir::directories(baseDir));
					foreach (String, it, directories)
					{
						if ((*it).lowered() == baseName.lowered())
						{
							baseName = (*it);
							break;
						}
					}
					message += " But part of the path seems to have a different case: " + Dir::joinPath(baseDir, baseName);
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
		this->_setInternalMessage(hsprintf("'%s' is not open!", filename.cStr()), sourceFile, lineNumber);
	}
	_FileNotOpenException::~_FileNotOpenException()
	{
	}
	
	_FileNotWriteableException::_FileNotWriteableException(const String& filename, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("'%s' is not writeable!", filename.cStr()), sourceFile, lineNumber);
	}
	_FileNotWriteableException::~_FileNotWriteableException()
	{
	}
	
	_ResourceNotExistsException::_ResourceNotExistsException(const String& type, const String& name, const String& container, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("'%s' '%s' does not exist in '%s'", name.cStr(), type.cStr(), container.cStr()), sourceFile, lineNumber);
	}
	_ResourceNotExistsException::~_ResourceNotExistsException()
	{
	}

	_ResourceAlreadyExistsException::_ResourceAlreadyExistsException(const String& type, const String& name, const String& container, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("'%s' '%s' already exists in '%s'", name.cStr(), type.cStr(), container.cStr()), sourceFile, lineNumber);
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
		this->_setInternalMessage(hsprintf("'%s' cannot be used on a container with size = 0", functionName.cStr()), sourceFile, lineNumber);
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
		this->_setInternalMessage(hsprintf("key '%s' not found in '%s'", key.cStr(), container.cStr()), sourceFile, lineNumber);
	}
	_ContainerKeyException::~_ContainerKeyException()
	{
	}
	
	_ObjectCannotCopyException::_ObjectCannotCopyException(const String& name, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("cannot create copy of object of class '%s'", name.cStr()), sourceFile, lineNumber);
	}
	_ObjectCannotCopyException::~_ObjectCannotCopyException()
	{
	}

	_ObjectCannotAssignException::_ObjectCannotAssignException(const String& name, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("cannot assign object of class '%s'", name.cStr()), sourceFile, lineNumber);
	}
	_ObjectCannotAssignException::~_ObjectCannotAssignException()
	{
	}

	_EnumerationValueNotExistsException::_EnumerationValueNotExistsException(unsigned int value, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("enum value does not exist: %d", value), sourceFile, lineNumber);
	}
	_EnumerationValueNotExistsException::~_EnumerationValueNotExistsException()
	{
	}

	_EnumerationValueAlreadyExistsException::_EnumerationValueAlreadyExistsException(unsigned int value, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage(hsprintf("enum value already exists: %d", value), sourceFile, lineNumber);
	}
	_EnumerationValueAlreadyExistsException::~_EnumerationValueAlreadyExistsException()
	{
	}

}
