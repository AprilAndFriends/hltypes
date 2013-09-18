/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _ZIPRESOURCE
#include <stdio.h>
#include <zip.h>

#include "harray.h"
#include "hmap.h"
#include "hmutex.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	namespace zip
	{
		Mutex access_mutex;
		Map<void*, Array<Resource*> > activeHandles;
		void* currentArchive = NULL;

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
			access_mutex.lock();
			void* file = zip_fopen((struct zip*)archivefile, filename.c_str(), 0);
			access_mutex.unlock();
			return file;
		}

		void fclose(void* file)
		{
			access_mutex.lock();
			zip_fclose((struct zip_file*)file);
			access_mutex.unlock();
		}

		long fread(void* file, void* buffer, int count)
		{
			access_mutex.lock();
			long result = zip_fread((struct zip_file*)file, buffer, count);
			access_mutex.unlock();
			return result;
		}

		long fsize(void* archivefile, const String& filename)
		{
			struct zip_stat stat;
			stat.size = 0;
			access_mutex.lock();
			zip_stat((struct zip*)archivefile, Resource::make_full_path(filename).c_str(), 0, &stat);
			access_mutex.unlock();
			return stat.size;
		}

		void* freopen(void* file, void* archivefile, const String& filename)
		{
			access_mutex.lock();
			zip_fclose((struct zip_file*)file);
			file = zip_fopen((struct zip*)archivefile, filename.c_str(), 0);
			access_mutex.unlock();
			return file;
		}

		Array<String> getFiles(void* archivefile)
		{
			Array<String> result;
			struct zip* archive = (struct zip*)archivefile;
			int count = zip_get_num_files(archive);
			for_iter (i, 0, count)
			{
				result += systemize_path(String(zip_get_name(archive, i, 0)));
			}
			return result;
		}

	}

}
#endif
