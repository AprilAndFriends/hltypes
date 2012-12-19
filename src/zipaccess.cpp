/// @file
/// @author  Boris Mikic
/// @version 2.01
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef HAVE_ZIPRESOURCE
#include <stdio.h>
#include <zip/zip.h>

#include "harray.h"
#include "hmap.h"
#include "hmutex.h"
#include "hresource.h"
#include "hstring.h"

namespace hltypes
{
	namespace zip
	{
		hmutex access_mutex;
		hmap<void*, harray<hresource*> > activeHandles;
		void* currentArchive = NULL;

		void setArchive(chstr value)
		{
			if (currentArchive != NULL && activeHandles[currentArchive].size() == 0)
			{
				zip_close((struct zip*)currentArchive);
				activeHandles.remove_key(currentArchive);
				currentArchive = NULL;
			}
			harray<void*> handles = activeHandles.keys();
			foreach (void*, it, handles)
			{
				if (activeHandles[*it].size() == 0)
				{
					zip_close((struct zip*)(*it));
					activeHandles.remove_key((*it));
				}
			}
		}

		void* open(hresource* resource)
		{
			if (currentArchive == NULL)
			{
				hstr archive = Resource::getArchive();
				if (archive == "")
				{
					return NULL;
				}
				currentArchive = zip_open(archive.c_str(), 0, NULL);
				if (currentArchive == NULL)
				{
					return NULL;
				}
				activeHandles[currentArchive] = harray<hresource*>();
			}
			activeHandles[currentArchive] += resource;
			return currentArchive;
		}

		void close(hresource* resource, void* archive)
		{
			harray<hresource*> references = activeHandles[archive];
			references -= resource;
			activeHandles[archive] = references;
			if (currentArchive != archive && references.size() == 0)
			{
				zip_close((struct zip*)archive);
				activeHandles.remove_key(archive);
			}
		}

		void* fopen(void* archivefile, chstr filename)
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

		long fsize(void* archivefile, chstr filename)
		{
			struct zip_stat stat;
			stat.size = 0;
			access_mutex.lock();
			zip_stat((struct zip*)archivefile, Resource::make_full_path(filename).c_str(), 0, &stat);
			access_mutex.unlock();
			return stat.size;
		}

		void* freopen(void* file, void* archivefile, chstr filename)
		{
			access_mutex.lock();
			zip_fclose((struct zip_file*)file);
			file = zip_fopen((struct zip*)archivefile, filename.c_str(), 0);
			access_mutex.unlock();
			return file;
		}

		harray<hstr> getFiles(void* archivefile)
		{
			harray<hstr> result;
			struct zip* archive = (struct zip*)archivefile;
			int count = zip_get_num_files(archive);
			for_iter (i, 0, count)
			{
				result += systemize_path(hstr(zip_get_name(archive, i, 0)));
			}
			return result;
		}

	}

}
#endif
