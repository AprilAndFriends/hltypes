/// @file
/// @version 2.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides functions for accessing ZIP files when using ZIP Resources.

#ifdef _ZIPRESOURCE

#include "harray.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	namespace zip
	{
		void setArchive(const String& value);
		void* open(Resource* resource);
		void close(Resource* resource, void* archive);
		void* fopen(void* archivefile, const String& filename);
		void fclose(void* file);
		int fread(void* file, void* buffer, int count);
		int64_t fsize(void* archivefile, const String& filename);
		void* freopen(void* file, void* archivefile, const String& filename);
		Array<String> getFiles(void* archivefile);
		FileInfo finfo(void* archivefile, const String& filename);

	}

}
#endif