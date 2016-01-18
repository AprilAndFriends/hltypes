/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _ZIPRESOURCE
#include <stdio.h>
#define MINIZ_HEADER_FILE_ONLY
#include <miniz.cpp>

#include "harray.h"
#include "hfbase.h"
#include "hmap.h"
#include "hmutex.h"
#include "hrdir.h"
#include "hresource.h"
#include "hstream.h"
#include "hstring.h"
#include "zipaccess.h"

#include "hlog.h"

#define FILENAME_BUFFER 8192

namespace hltypes
{
	namespace zip
	{
		static Mutex accessMutex;
		static Map<miniz::mz_zip_archive*, Array<Resource*> > activeHandles;
		static miniz::mz_zip_archive* currentArchive = NULL;

		void setArchive(const String& value)
		{
			Mutex::ScopeLock lock(&accessMutex);
			if (currentArchive != NULL && activeHandles[currentArchive].size() == 0)
			{
				miniz::mz_zip_reader_end(currentArchive);
				activeHandles.removeKey(currentArchive);
				delete currentArchive;
				currentArchive = NULL;
			}
			Array<miniz::mz_zip_archive*> handles = activeHandles.keys();
			foreach (miniz::mz_zip_archive*, it, handles)
			{
				if (activeHandles[*it].size() == 0)
				{
					miniz::mz_zip_reader_end(*it);
					activeHandles.removeKey(*it);
					delete (*it);
				}
			}
		}

		void* open(Resource* resource)
		{
			Mutex::ScopeLock lock(&accessMutex);
			if (currentArchive == NULL)
			{
				String archive = Resource::getArchive();
				if (archive == "")
				{
					return NULL;
				}
				currentArchive = new miniz::mz_zip_archive();
				if (!miniz::mz_zip_reader_init_file(currentArchive, archive.cStr(), 0))
				{
					delete currentArchive;
					currentArchive = NULL;
					return NULL;
				}
				activeHandles[currentArchive] = Array<Resource*>();
			}
			activeHandles[currentArchive] += resource;
			return currentArchive;
		}

		void close(Resource* resource, void* archive)
		{
			miniz::mz_zip_archive* zipArchive = (miniz::mz_zip_archive*)archive;
			Mutex::ScopeLock lock(&accessMutex);
			Array<Resource*> references = activeHandles[zipArchive];
			references -= resource;
			activeHandles[zipArchive] = references;
			if (currentArchive != zipArchive && references.size() == 0)
			{
				miniz::mz_zip_reader_end(zipArchive);
				activeHandles.removeKey(zipArchive);
				delete zipArchive;
			}
		}

		void* fopen(void* archiveFile, const String& filename)
		{
			int size = (int)finfo(archiveFile, filename).size;
			if (size <= 0)
			{
				return NULL;
			}
			Stream* stream = new Stream(size);
			stream->prepareManualWriteRaw(size);
			Mutex::ScopeLock lock(&accessMutex);
			bool result = (miniz::mz_zip_reader_extract_file_to_mem((miniz::mz_zip_archive*)archiveFile, filename.cStr(), (unsigned char*)(*stream), size, 0) != MZ_FALSE);
			lock.release();
			if (result)
			{
				stream->rewind();
			}
			else
			{
				delete stream;
				stream = NULL;
			}
			return stream;
		}

		void fclose(void* file)
		{
			delete ((Stream*)file);
		}

		bool fseek(void* file, int64_t offset, StreamBase::SeekMode mode)
		{
			return ((Stream*)file)->seek(offset, mode);
		}

		int64_t fposition(void* file)
		{
			return ((Stream*)file)->position();
		}

		int fread(void* file, void* buffer, int count)
		{
			return ((Stream*)file)->readRaw(buffer, count);
		}

		bool fexists(void* archiveFile, const String& filename)
		{
			miniz::mz_zip_archive* archive = (miniz::mz_zip_archive*)archiveFile;
			Mutex::ScopeLock lock(&accessMutex);
			int index = miniz::mz_zip_reader_locate_file(archive, filename.cStr(), "", 0);
			return (index >= 0 && !miniz::mz_zip_reader_is_file_a_directory(archive, index));
		}

		Array<String> getFiles(void* archiveFile)
		{
			Array<String> result;
			miniz::mz_zip_archive* archive = (miniz::mz_zip_archive*)archiveFile;
			Mutex::ScopeLock lock(&accessMutex);
			int count = miniz::mz_zip_reader_get_num_files(archive);
			char filename[FILENAME_BUFFER] = { 0 };
			unsigned int size = 0;
			for_iter (i, 0, count)
			{
				size = miniz::mz_zip_reader_get_filename(archive, i, filename, FILENAME_BUFFER - 1);
				if (size <= FILENAME_BUFFER - 1)
				{
					filename[size] = '\0';
					result += String(filename);
				}
			}
			return result;
		}

		FileInfo finfo(void* archiveFile, const String& filename)
		{
			FileInfo info;
			miniz::mz_zip_archive* zipArchive = (miniz::mz_zip_archive*)archiveFile;
			Mutex::ScopeLock lock(&accessMutex);
			int index = miniz::mz_zip_reader_locate_file(zipArchive, filename.cStr(), "", miniz::MZ_ZIP_FLAG_CASE_SENSITIVE);
			if (index >= 0)
			{
				miniz::mz_zip_archive_file_stat stat;
				if (miniz::mz_zip_reader_file_stat(zipArchive, index, &stat))
				{
					info.size = stat.m_uncomp_size;
					info.modificationTime = stat.m_time;
				}
			}
			return info;
		}

	}

}
#endif
