/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef _HLTYPES_EXCEPTION
#define _HLTYPES_EXCEPTION

#include "hltypesExport.h"
#include "hstring.h"

namespace hltypes
{
	class hltypesExport exception
	{
	protected:
		string msg;
	public:
		exception(string message,const char* source_file,int line);
		virtual ~exception();
		virtual string what();
	};
	#define hl_exception(msg) hltypes::exception(msg,__FILE__,__LINE__)
/*********************************************************************************/
	class hltypesExport _file_not_found : public exception
	{
	public:
		_file_not_found(string filename,const char* source_file,int line);
	};
	#define file_not_found(filename) hltypes::_file_not_found(filename,__FILE__,__LINE__)
/*********************************************************************************/
	class hltypesExport _key_error : public exception
	{
	public:
		_key_error(string key,string container,const char* source_file,int line);
	};
	#define key_error(filename,container) hltypes::_key_error(filename,container,__FILE__,__LINE__)
/*********************************************************************************/
	class hltypesExport _resource_error : public exception
	{
	public:
		_resource_error(string type,string name,string container,const char* source_file,int line);
	};
	#define resource_error(type,name,container) hltypes::_resource_error(type,name,container,__FILE__,__LINE__)
}

//typedef hltypes::config_file hcfgfile;

#endif
