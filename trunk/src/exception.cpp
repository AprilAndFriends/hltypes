/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "exception.h"
#include "hstring.h"

namespace hltypes
{
	exception::exception(chstr message, const char* source_file, int line)
	{
		hstr filename = source_file;
		int index = filename.rfind("/");
		if (index == -1)
		{
			index = filename.rfind("\\");
		}
		msg = "[" + filename(index + 1, -1) + ":" + hstr(line) + "] " + message;
	}
	
	exception::~exception()
	{
	}

	_file_not_found::_file_not_found(chstr filename, const char* source_file, int line) :
	    exception("'" + filename + "' not found!", source_file, line)
	{
	}
	_file_not_found::~_file_not_found()
	{
	}
	
	_file_not_open::_file_not_open(chstr filename, const char* source_file, int line) :
	    exception("'" + filename + "' is not open!", source_file, line)
	{
	}
	_file_not_open::~_file_not_open()
	{
	}
	
	_file_long_error::_file_long_error(chstr filename, const char* source_file, int line) :
	    exception("'" + filename + "' tried to dump/load long that is not 8 or 4 bytes!", source_file, line)
	{
	}
	_file_long_error::~_file_long_error()
	{
	}
	
	_resource_not_writeable::_resource_not_writeable(chstr filename, const char* source_file, int line) :
	    exception("'" + filename + "' is accessed as a resource which is not writeable!", source_file, line)
	{
	}
	_resource_not_writeable::~_resource_not_writeable()
	{
	}

	_resource_only_seekable_forward::_resource_only_seekable_forward(chstr filename, const char* source_file, int line) :
	    exception("'" + filename + "' is accessed as a resource which can only be seeked forward!", source_file, line)
	{
	}
	_resource_only_seekable_forward::~_resource_only_seekable_forward()
	{
	}
	
	_index_error::_index_error(int index, const char* source_file, int line) :
	    exception("index '" + hstr(index) + "' out of range", source_file, line)
	{
	}
	_index_error::~_index_error()
	{
	}
	
	_size_error::_size_error(chstr function_name, const char* source_file, int line) :
	    exception(function_name + " cannot be used on a harray with size = 0", source_file, line)
	{
	}
	_size_error::~_size_error()
	{
	}
	
	_element_not_found_error::_element_not_found_error(const char* source_file, int line) :
	    exception("element not found in array", source_file, line)
	{
	}
	_element_not_found_error::~_element_not_found_error()
	{
	}
	
	_range_error::_range_error(int start, int count, const char* source_file, int line) :
	    exception("range '" + hstr(start) + " by " + hstr(count) + "' out of range", source_file, line)
	{
	}
	_range_error::~_range_error()
	{
	}
	
	_key_error::_key_error(chstr key, chstr container, const char* source_file, int line) :
	    exception("key '" + key + "' not found in " + container, source_file, line)
	{
	}
	_key_error::~_key_error()
	{
	}
	
	_resource_error::_resource_error(chstr type, chstr name, chstr container, const char* source_file, int line) :
	    exception("'" + name + "' " + type + " not found in " + container, source_file, line)
	{
	}
	_resource_error::~_resource_error()
	{
	}

}
