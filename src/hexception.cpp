/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef __ANDROID__
	#include <errno.h>
#endif

#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hlog.h"
#include "hltypesUtil.h"
#include "hplatform.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"
#include "platform_internal.h"

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
		this->rawMessage = message;
		this->message = "[" + Dir::baseName(sourceFile) + ":" + hstr(lineNumber) + "] " + message;
		// because Visual Studio on UWP cannot properly display exceptions and stack traces for some reason even though it should
		// because Android doesn't display register data properly if an exception is thrown
#if defined(_WIN32) || defined(__ANDROID__) || defined(_IOS)
		if (Log::isLevelDebug() && message != "")
		{
			hltypes::_platformPrint("FATAL", this->message, Log::LevelError);
		}
#endif
	}

	_FileCouldNotOpenException::_FileCouldNotOpenException(const String& filename, bool isResource, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
#if defined(_WIN32) || defined(__ANDROID__)
		int errnoValue = errno;
#endif
		String message = "'" + filename + "' could not be opened!";
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
		}
		catch (_Exception&) // is this inception or exception, I am confused
		{
		}
#if defined(_WINRT) || defined(_IOS) || defined(_MAC)
		message += " The current platform might use a sandboxed environment and may not be able to access files outside of it.";
#endif
#if defined(_WIN32) || defined(__ANDROID__)
		message += " System error: " + String(strerror(errnoValue));
#else
		message += " File not found!";
#endif
		this->_setInternalMessage(message, sourceFile, lineNumber);
	}
	
	_FileNotOpenException::_FileNotOpenException(const String& filename, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("'" + filename + "' is not open!", sourceFile, lineNumber);
	}
	
	_FileNotWriteableException::_FileNotWriteableException(const String& filename, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("'" + filename + "' is not writeable!", sourceFile, lineNumber);
	}
	
	_ResourceNotExistsException::_ResourceNotExistsException(const String& type, const String& name, const String& container, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("'" + name + "' '" + type + "' does not exist in '" + container + "'", sourceFile, lineNumber);
	}

	_ResourceAlreadyExistsException::_ResourceAlreadyExistsException(const String& type, const String& name, const String& container, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("'" + name + "' '" + type + "' already exists in '" + container + "'", sourceFile, lineNumber);
	}

	_ContainerIndexException::_ContainerIndexException(int index, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("index '" + hstr(lineNumber) + "' out of range", sourceFile, lineNumber);
	}
	
	_ContainerEmptyException::_ContainerEmptyException(const String& functionName, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("'" + functionName + "' cannot be used on a container with size = 0", sourceFile, lineNumber);
	}
	
	_ContainerElementNotFoundException::_ContainerElementNotFoundException(const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("element not found in container", sourceFile, lineNumber);
	}
	
	_ContainerRangeException::_ContainerRangeException(int start, int count, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("range 'at " + hstr(start) + " for " + hstr(count) + "' out of range", sourceFile, lineNumber);
	}
	
	_ContainerKeyException::_ContainerKeyException(const String& key, const String& container, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("key '" + key + "' not found in '" + container + "'", sourceFile, lineNumber);
	}
	
	_ObjectCannotCopyException::_ObjectCannotCopyException(const String& name, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("cannot create copy of object of class '" + name + "'", sourceFile, lineNumber);
	}

	_ObjectCannotAssignException::_ObjectCannotAssignException(const String& name, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("cannot assign object of class '" + name + "'", sourceFile, lineNumber);
	}

	_EnumerationValueNotExistsException::_EnumerationValueNotExistsException(unsigned int value, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("enum value does not exist: " + hstr(value), sourceFile, lineNumber);
	}

	_EnumerationValueAlreadyExistsException::_EnumerationValueAlreadyExistsException(unsigned int value, const char* sourceFile, int lineNumber) :
		_Exception("", sourceFile, lineNumber)
	{
		this->_setInternalMessage("enum value already exists: " + hstr(value), sourceFile, lineNumber);
	}

}
