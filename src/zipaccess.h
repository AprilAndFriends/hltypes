/// @file
/// @version 3.1
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
		bool mountArchive(const String& path, const String& archiveFilename, const String& cwd);
		bool unmountArchive(const String& path);
		void* fopen(Resource* resource, const String& filename);
		void fclose(Resource* resource, void* file);
		bool fseek(void* file, int64_t offset, StreamBase::SeekMode mode);
		int64_t fposition(void* file);
		int fread(void* file, void* buffer, int count);
		bool fexists(const String& filename);
		int64_t fsize(void* file);
		FileInfo finfo(const String& filename);
		Array<String> getFiles();
		bool isZipMounts();

	}
}
#endif