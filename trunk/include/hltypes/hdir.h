/************************************************************************************\
* This source dir is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef HLTYPES_HDIR_H
#define HLTYPES_HDIR_H

#include "hltypesExport.h"
#include "harray.h"
#include "hstring.h"

namespace hltypes
{
	class hltypesExport dir
	{
	public:
		// static
		
		static bool create(chstr dirname);
		static bool create_new(chstr dirname);
		static bool remove(chstr dirname);
		static bool exists(chstr dirname);
		static bool clear(chstr dirname);
		static bool rename(chstr old_dirname, chstr new_dirname);
		static bool move(chstr dirname, chstr path);
		static bool copy(chstr old_dirname, chstr new_dirname);
		static bool create_path(chstr path);
		
		static harray<hstr> entries(chstr dirname, bool prepend_dir = false); // all entries including . and ..
		static harray<hstr> contents(chstr dirname, bool prepend_dir = false); // all contents
		static harray<hstr> directories(chstr dirname, bool prepend_dir = false); // directories only
		static harray<hstr> files(chstr dirname, bool prepend_dir = false); // files only
		
	private:
		dir() { }
		~dir() { }
		
	};
}

typedef hltypes::dir hdir;

#endif

