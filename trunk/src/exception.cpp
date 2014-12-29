/// @file
/// @version 2.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "exception.h"
#include "hdir.h"
#include "hfile.h"
#include "hlog.h"
#include "hltypesUtil.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	exception::exception(const String& message, const char* source_file, int line_number)
	{
		this->_setInternalMessage(message, source_file, line_number);
	}
	
	exception::~exception()
	{
	}

	void exception::_setInternalMessage(const String& message, const char* source_file, int line_number)
	{
		this->msg = hsprintf("[%s:%d] %s", Dir::basename(source_file).c_str(), line_number, message.c_str());
#ifdef _WINRT // because Visual Studio on WinRT cannot properly display exceptions and stack traces for some reason even though it should
		if (Log::isLevelDebug() && message != "")
		{
			hltypes::_platform_print("FATAL", this->msg, 1000);
		}
#endif
	}

	_file_could_not_open::_file_could_not_open(const String& filename, bool is_resource, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		String message = hsprintf("'%s' could not be opened!", filename.c_str());
		try
		{
			String dir = DirBase::basedir(filename);
			String basename = DirBase::basename(filename);
			Array<String> files = (!is_resource ? Dir::files(dir) : ResourceDir::files(dir));
			foreach (String, it, files)
			{
				if ((*it) == basename)
				{
					message += " File appears to be in use.";
					throw exception("", "", 0);
				}
				if ((*it).lower() == basename.lower())
				{
					message += " But there is a file with a different case: " + Dir::join_path(dir, (*it));
					throw exception("", "", 0);
				}
			}
			Array<String> dirs = Dir::split_path(dir);
			String path;
			Array<String> directories;
			for_iter (i, 0, dirs.size())
			{
				path = Dir::join_paths(dirs(0, dirs.size() - i));
				if (!is_resource ? !Dir::exists(path) && Dir::exists(path, false) : !ResourceDir::exists(path) && ResourceDir::exists(path, false))
				{
					dir = Dir::join_paths(dirs(0, dirs.size() - 1 - i));
					basename = dirs[dirs.size() - 1 - i];
					Array<String> directories = (!is_resource ? Dir::directories(dir) : ResourceDir::directories(dir));
					foreach (String, it, directories)
					{
						if ((*it).lower() == basename.lower())
						{
							basename = (*it);
							break;
						}
					}
					message += " But part of the path seems to have a different case: " + Dir::join_path(dir, basename);
					throw exception("", "", 0);
				}
			}
			message += " File not found!";
		}
		catch (exception&) // is this inception or exception, I am confused
		{
		}
#ifdef _WIN32 // could be useful
		message += " System error: " + String(strerror(errno));
#endif
		this->_setInternalMessage(message, source_file, line_number);
	}
	_file_could_not_open::~_file_could_not_open()
	{
	}
	
	_file_not_open::_file_not_open(const String& filename, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage(hsprintf("'%s' is not open!", filename.c_str()), source_file, line_number);
	}
	_file_not_open::~_file_not_open()
	{
	}
	
	_file_not_writeable::_file_not_writeable(const String& filename, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage(hsprintf("'%s' is not writeable!", filename.c_str()), source_file, line_number);
	}
	_file_not_writeable::~_file_not_writeable()
	{
	}
	
	_resource_not_exists::_resource_not_exists(const String& type, const String& name, const String& container, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage(hsprintf("'%s' '%s' does not exist in '%s'", name.c_str(), type.c_str(), container.c_str()), source_file, line_number);
	}
	_resource_not_exists::~_resource_not_exists()
	{
	}

	_resource_already_exists::_resource_already_exists(const String& type, const String& name, const String& container, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage(hsprintf("'%s' '%s' already exists in '%s'", name.c_str(), type.c_str(), container.c_str()), source_file, line_number);
	}
	_resource_already_exists::~_resource_already_exists()
	{
	}

	_container_index_error::_container_index_error(int index, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage(hsprintf("index '%d' out of range", index), source_file, line_number);
	}
	_container_index_error::~_container_index_error()
	{
	}
	
	_container_empty_error::_container_empty_error(const String& function_name, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage(hsprintf("'%s' cannot be used on a container with size = 0", function_name.c_str()), source_file, line_number);
	}
	_container_empty_error::~_container_empty_error()
	{
	}
	
	_container_element_not_found::_container_element_not_found(const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage("element not found in container	", source_file, line_number);
	}
	_container_element_not_found::~_container_element_not_found()
	{
	}
	
	_container_range_error::_container_range_error(int start, int count, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage(hsprintf("range 'at %d for %d' out of range", start, count), source_file, line_number);
	}
	_container_range_error::~_container_range_error()
	{
	}
	
	_container_key_error::_container_key_error(const String& key, const String& container, const char* source_file, int line_number) :
		exception("", source_file, line_number)
	{
		this->_setInternalMessage(hsprintf("key '%s' not found in '%s'", key.c_str(), container.c_str()), source_file, line_number);
	}
	_container_key_error::~_container_key_error()
	{
	}
	
}
