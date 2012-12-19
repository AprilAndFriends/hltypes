/// @file
/// @author  Boris Mikic
/// @version 2.01
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Provides functions for accessing ZIP files when using ZIP Resources.

#ifdef HAVE_ZIPRESOURCE

#include "harray.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	namespace zip
	{
		void setArchive(chstr value);
		void* open(hresource* resource);
		void close(hresource* resource, void* archive);
		void* fopen(void* archivefile, chstr filename);
		void fclose(void* file);
		long fread(void* file, void* buffer, int count);
		long fsize(void* archivefile, chstr filename);
		void* freopen(void* file, void* archivefile, chstr filename);
		harray<hstr> getFiles(void* archivefile);

	}

}
#endif