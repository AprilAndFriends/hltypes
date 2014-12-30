/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _ZIPRESOURCE
#include <stdio.h>
#include <zip.h>

#include "harray.h"
#include "hmap.h"
#include "hmutex.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	namespace zip
	{
		static Mutex access_mutex;
		static Map<void*, Array<Resource*> > activeHandles;
		static void* currentArchive = NULL;

		void setArchive(const String& value)
		{
			if (currentArchive != NULL && activeHandles[currentArchive].size() == 0)
			{
				zip_close((struct zip*)currentArchive);
				activeHandles.remove_key(currentArchive);
				currentArchive = NULL;
			}
			Array<void*> handles = activeHandles.keys();
			foreach (void*, it, handles)
			{
				if (activeHandles[*it].size() == 0)
				{
					zip_close((struct zip*)(*it));
					activeHandles.remove_key((*it));
				}
			}
		}

		void* open(Resource* resource)
		{
			if (currentArchive == NULL)
			{
				String archive = Resource::getArchive();
				if (archive == "")
				{
					return NULL;
				}
				currentArchive = zip_open(archive.c_str(), 0, NULL);
				if (currentArchive == NULL)
				{
					return NULL;
				}
				activeHandles[currentArchive] = Array<Resource*>();
			}
			activeHandles[currentArchive] += resource;
			return currentArchive;
		}

		void close(Resource* resource, void* archive)
		{
			Array<Resource*> references = activeHandles[archive];
			references -= resource;
			activeHandles[archive] = references;
			if (currentArchive != archive && references.size() == 0)
			{
				zip_close((struct zip*)archive);
				activeHandles.remove_key(archive);
			}
		}

		void* fopen(void* archivefile, const String& filename)
		{
			Mutex::ScopeLock lock(&access_mutex);
			return zip_fopen((struct zip*)archivefile, filename.c_str(), 0);
		}

		void fclose(void* file)
		{
			Mutex::ScopeLock lock(&access_mutex);
			zip_fclose((struct zip_file*)file);
		}

		int fread(void* file, void* buffer, int count)
		{
			Mutex::ScopeLock lock(&access_mutex);
			return (int)zip_fread((struct zip_file*)file, buffer, count);
		}

		int64_t fsize(void* archivefile, const String& filename)
		{
			struct zip_stat stat;
			stat.size = 0;
			Mutex::ScopeLock lock(&access_mutex);
			zip_stat((struct zip*)archivefile, Resource::make_full_path(filename).c_str(), 0, &stat);
			return (int64_t)stat.size;
		}

		void* freopen(void* file, void* archivefile, const String& filename)
		{
			Mutex::ScopeLock lock(&access_mutex);
			zip_fclose((struct zip_file*)file);
			return zip_fopen((struct zip*)archivefile, filename.c_str(), 0);
		}

		Array<String> getFiles(void* archivefile)
		{
			Array<String> result;
			struct zip* archive = (struct zip*)archivefile;
			int count = zip_get_num_files(archive);
			for_iter (i, 0, count)
			{
				result += ResourceDir::systemize(String(zip_get_name(archive, i, 0)));
			}
			return result;
		}

		FileInfo finfo(void* archivefile, const String& filename)
		{
			struct zip_stat stat;
			stat.size = 0;
			stat.mtime = 0;
			Mutex::ScopeLock lock(&access_mutex);
			zip_stat((struct zip*)archivefile, Resource::make_full_path(filename).c_str(), 0, &stat);
			FileInfo info;
			info.size = stat.size;
			info.modification_time = stat.mtime;
			return info;
		}

	}

}
#endif
