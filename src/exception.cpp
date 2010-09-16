#include "exception.h"
#include "harray.h"
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

/*********************************************************************************/
	_file_not_found::_file_not_found(chstr filename, const char* source_file, int line) :
	    exception("'" + filename + "' not found!", source_file, line)
	{
	}
/*********************************************************************************/
	_file_not_open::_file_not_open(chstr filename, const char* source_file, int line) :
	    exception("'" + filename + "' is not open!", source_file, line)
	{
	}
/*********************************************************************************/
	_index_error::_index_error(int index, const char* source_file, int line) :
	    exception("index '" + hstr(index) + "' out of range", source_file, line)
	{
	}
/*********************************************************************************/
	_range_error::_range_error(int start, int count, const char* source_file, int line) :
	    exception("range '" + hstr(start) + " by " + hstr(count) + "' out of range", source_file, line)
	{
	}
/*********************************************************************************/
	_key_error::_key_error(chstr key, chstr container, const char* source_file, int line) :
	    exception("key '" + key + "' not found in " + container, source_file, line)
	{
	}
/*********************************************************************************/	
	_resource_error::_resource_error(chstr type, chstr name, chstr container, const char* source_file, int line) :
	    exception("'" + name + "' " + type + " not found in " + container, source_file, line)
	{
	}
}
