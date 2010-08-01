#include "exception.h"
#include "harray.h"

namespace hltypes
{
	exception::exception(hstr message,const char* source_file,int line)
	{
		hstr filename=source_file;
		int index=filename.rfind("/");
		if (index == -1) index=filename.rfind("\\");
		msg="["+filename(index+1,-1)+":"+hstr(line)+"] "+message;
	}
	
	exception::~exception()
	{
		
	}

	string exception::what()
	{
		return this->msg;
	}
/*********************************************************************************/
	_file_not_found::_file_not_found(hstr filename,const char* source_file,int line) :
	    exception("'"+filename+"' not found!",source_file,line)
	{
	}
/*********************************************************************************/
	_key_error::_key_error(hstr key,hstr container,const char* source_file,int line) :
	    exception("key '"+key+"' not found in "+container,source_file,line)
	{
	}
/*********************************************************************************/	
	_resource_error::_resource_error(string type,string name,string container,const char* source_file,int line) :
	    exception("'"+name+"' "+type+" not found in "+container,source_file,line)
	{
	}
}
