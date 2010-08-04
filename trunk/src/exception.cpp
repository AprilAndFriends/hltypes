#include "exception.h"
#include "harray.h"

namespace hltypes
{
	exception::exception(chstr message, const char* source_file, int line)
	{
		hstr filename = source_file;
		int index = filename.rfind("/");
		if (index == -1) index = filename.rfind("\\");
		msg = hsprintf("[%s:%s] %s", filename(index + 1, -1).c_str(), line, message.c_str());
	}
	
	exception::~exception()
	{
	}

	hstr exception::message()
	{
		return this->msg;
	}
/*********************************************************************************/
	_file_not_found::_file_not_found(chstr filename, const char* source_file, int line) :
	    exception("'" + filename + "' not found!", source_file, line)
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
