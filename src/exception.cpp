/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "exception.h"
#include "hltypesUtil.h"
#include "hstring.h"

namespace hltypes
{
	exception::exception(chstr message, const char* source_file, int line_number)
	{
		this->msg = hsprintf("[%s:%d] %s", get_basename(source_file).c_str(), line_number, message.c_str());
	}
	
	exception::~exception()
	{
	}

	_file_not_found::_file_not_found(chstr filename, const char* source_file, int line_number) :
		exception(hsprintf("'%s' not found!", filename.c_str()), source_file, line_number)
	{
	}
	_file_not_found::~_file_not_found()
	{
	}
	
	_file_not_open::_file_not_open(chstr filename, const char* source_file, int line_number) :
		exception(hsprintf("'%s' is not open!", filename.c_str()), source_file, line_number)
	{
	}
	_file_not_open::~_file_not_open()
	{
	}
	
	_file_not_writeable::_file_not_writeable(chstr filename, const char* source_file, int line_number) :
		exception(hsprintf("'%s' is not writeable!", filename.c_str()), source_file, line_number)
	{
	}
	_file_not_writeable::~_file_not_writeable()
	{
	}
	
	_file_long_error::_file_long_error(chstr filename, const char* source_file, int line_number) :
		exception(hsprintf("'%s' tried to dump/load long that is not 8 or 4 bytes!", filename.c_str()), source_file, line_number)
	{
	}
	_file_long_error::~_file_long_error()
	{
	}
	
	_resource_not_exists::_resource_not_exists(chstr type, chstr name, chstr container, const char* source_file, int line_number) :
		exception(hsprintf("'%s' '%s' does not exist in '%s'", name.c_str(), type.c_str(), container.c_str()), source_file, line_number)
	{
	}
	_resource_not_exists::~_resource_not_exists()
	{
	}

	_resource_already_exists::_resource_already_exists(chstr type, chstr name, chstr container, const char* source_file, int line_number) :
		exception(hsprintf("'%s' '%s' already exists in '%s'", name.c_str(), type.c_str(), container.c_str()), source_file, line_number)
	{
	}
	_resource_already_exists::~_resource_already_exists()
	{
	}

	_container_index_error::_container_index_error(int index, const char* source_file, int line_number) :
		exception(hsprintf("index '%d' out of range", index), source_file, line_number)
	{
	}
	_container_index_error::~_container_index_error()
	{
	}
	
	_container_empty_error::_container_empty_error(chstr function_name, const char* source_file, int line_number) :
		exception(hsprintf("'%s' cannot be used on a container with size = 0", function_name.c_str()), source_file, line_number)
	{
	}
	_container_empty_error::~_container_empty_error()
	{
	}
	
	_container_element_not_found::_container_element_not_found(const char* source_file, int line_number) :
		exception("element not found in container	", source_file, line_number)
	{
	}
	_container_element_not_found::~_container_element_not_found()
	{
	}
	
	_container_range_error::_container_range_error(int start, int count, const char* source_file, int line_number) :
		exception(hsprintf("range 'at %d for %d' out of range", start, count), source_file, line_number)
	{
	}
	_container_range_error::~_container_range_error()
	{
	}
	
	_container_key_error::_container_key_error(chstr key, chstr container, const char* source_file, int line_number) :
		exception(hsprintf("key '%s' not found in '%s'", key.c_str(), container.c_str()), source_file, line_number)
	{
	}
	_container_key_error::~_container_key_error()
	{
	}
	
}
