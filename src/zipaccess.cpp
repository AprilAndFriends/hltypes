/// @file
/// @version 4.0
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
		// utility class that handles open archive files
		class ArchiveFileHandle
		{
		public:
			String path;
			String filename;
			String cwd;
			miniz::mz_zip_archive* zipArchive;
			Array<String> internalFiles;
			Array<Resource*> accessingResources;

			ArchiveFileHandle(const String& path, const String& filename, const String& cwd) : zipArchive(NULL)
			{
				this->path = path;
				this->filename = filename;
				this->cwd = cwd;
			}

			~ArchiveFileHandle()
			{
				this->tryDeleteZipArchive(true);
			}

			bool ensureCreatedZipArchive()
			{
				if (this->zipArchive == NULL)
				{
					Array<String> segments;
					String platformCwd = _platformResourceCwd();
					if (platformCwd != "")
					{
						segments += platformCwd;
					}
					segments += this->filename;
					this->zipArchive = new miniz::mz_zip_archive();
					if (!miniz::mz_zip_reader_init_file(this->zipArchive, hrdir::normalize(hrdir::joinPaths(segments)).cStr(), 0))
					{
						delete this->zipArchive;
						this->zipArchive = NULL;
						return false;
					}
				}
				return true;
			}

			bool tryDeleteZipArchive(bool force = false)
			{
				if ((force || this->accessingResources.size() == 0) && this->zipArchive != NULL)
				{
					miniz::mz_zip_reader_end(this->zipArchive);
					delete this->zipArchive;
					this->zipArchive = NULL;
					return true;
				}
				return false;
			}

		};

		// utility class that handles open files within archive files
		class FileHandle
		{
		public:
			ArchiveFileHandle* archive;
			Stream* stream;

			FileHandle(ArchiveFileHandle* archive, Stream* stream)
			{
				this->archive = archive;
				this->stream = stream;
			}

		};

		// keeping track of all mounts
		static Mutex accessMutex;
		static Map<String, ArchiveFileHandle*> pathMounts;

		// utility methods
		inline ArchiveFileHandle* _aopen(String& filename)
		{
			String longestPath;
			ArchiveFileHandle* result = NULL;
			foreach_m (ArchiveFileHandle*, it, pathMounts)
			{
				if (it->first != "" && filename.startsWith(it->first) && (result == NULL || it->first.size() > longestPath.size()))
				{
					longestPath = it->first;
					result = it->second;
				}
			}
			if (result == NULL)
			{
				result = pathMounts.tryGet("", NULL);
			}
			if (result != NULL)
			{
				if (!result->ensureCreatedZipArchive())
				{
					result = NULL;
				}
				else
				{
					if (result->path.size() > 0)
					{
						filename = filename(result->path.size(), filename.size() - result->path.size());
					}
					if (result->cwd != "")
					{
						filename = result->cwd + filename;
					}
				}
			}
			return result;
		}

		inline void _aclose(ArchiveFileHandle* archive)
		{
			if (!pathMounts.hasKey(archive->path) && archive->accessingResources.size() == 0)
			{
				delete archive;
			}
		}

		FileInfo _finfo(ArchiveFileHandle* archiveFile, const String& filename)
		{
			FileInfo info;
			int index = miniz::mz_zip_reader_locate_file(archiveFile->zipArchive, filename.cStr(), "", miniz::MZ_ZIP_FLAG_CASE_SENSITIVE);
			if (index >= 0)
			{
				miniz::mz_zip_archive_file_stat stat;
				if (miniz::mz_zip_reader_file_stat(archiveFile->zipArchive, index, &stat))
				{
					info.size = stat.m_uncomp_size;
					info.modificationTime = stat.m_time;
				}
			}
			return info;
		}

		// API methods
		bool mountArchive(const String& path, const String& archiveFilename, const String& cwd)
		{
			Mutex::ScopeLock lock(&accessMutex);
			if (pathMounts.hasKey(path))
			{
				return false;
			}
			pathMounts[path] = new ArchiveFileHandle(path, archiveFilename, cwd);
			return true;
		}

		bool unmountArchive(const String& path)
		{
			Mutex::ScopeLock lock(&accessMutex);
			ArchiveFileHandle* archive = pathMounts.tryGet(path, NULL);
			if (archive == NULL)
			{
				return false;
			}
			pathMounts.removeKey(path);
			if (archive->accessingResources.size() == 0)
			{
				delete archive;
			}
			Array<ArchiveFileHandle*> archives = pathMounts.values();
			foreach (ArchiveFileHandle*, it, archives)
			{
				(*it)->tryDeleteZipArchive();
			}
			return true;
		}

		void* fopen(Resource* resource, const String& filename)
		{
			String realFilename = filename;
			Mutex::ScopeLock lock(&accessMutex);
			ArchiveFileHandle* archive = _aopen(realFilename);
			if (archive == NULL)
			{
				return NULL;
			}
			int size = (int)_finfo(archive, realFilename).size;
			if (size > 0)
			{
				Stream* stream = new Stream(size);
				stream->prepareManualWriteRaw(size);
				if (miniz::mz_zip_reader_extract_file_to_mem(archive->zipArchive, realFilename.cStr(), (unsigned char*)(*stream), size, 0) != MZ_FALSE)
				{
					FileHandle* fileHandle = new FileHandle(archive, stream);
					archive->accessingResources += resource;
					lock.release();
					stream->rewind();
					return fileHandle;
				}
				delete stream;
			}
			_aclose(archive);
			return NULL;
		}

		void fclose(Resource* resource, void* file)
		{
			FileHandle* fileHandle = (FileHandle*)file;
			delete fileHandle->stream;
			Mutex::ScopeLock lock(&accessMutex);
			fileHandle->archive->accessingResources -= resource;
			_aclose(fileHandle->archive);
			lock.release();
			delete fileHandle;
		}

		bool fseek(void* file, int64_t offset, StreamBase::SeekMode mode)
		{
			return ((FileHandle*)file)->stream->seek(offset, mode);
		}

		int64_t fposition(void* file)
		{
			return ((FileHandle*)file)->stream->position();
		}

		int fread(void* file, void* buffer, int count)
		{
			return ((FileHandle*)file)->stream->readRaw(buffer, count);
		}

		bool fexists(const String& filename)
		{
			String realFilename = filename;
			Mutex::ScopeLock lock(&accessMutex);
			ArchiveFileHandle* archive = _aopen(realFilename);
			if (archive == NULL)
			{
				return false;
			}
			int index = miniz::mz_zip_reader_locate_file(archive->zipArchive, realFilename.cStr(), "", 0);
			bool result = (index >= 0 && !miniz::mz_zip_reader_is_file_a_directory(archive->zipArchive, index));
			_aclose(archive);
			return result;
		}

		int64_t fsize(void* file)
		{
			return ((FileHandle*)file)->stream->size();
		}

		FileInfo finfo(const String& filename)
		{
			String realFilename = filename;
			Mutex::ScopeLock lock(&accessMutex);
			ArchiveFileHandle* archive = _aopen(realFilename);
			FileInfo info = _finfo(archive, realFilename);
			_aclose(archive);
			return info;
		}

		Array<String> getFiles()
		{
			Array<String> result;
			int count = 0;
			char filename[FILENAME_BUFFER] = { 0 };
			unsigned int size = 0;
			String path;
			Mutex::ScopeLock lock(&accessMutex);
			foreach_m (ArchiveFileHandle*, it, pathMounts)
			{
				if (it->second->internalFiles.size() == 0)
				{
					if (it->second->ensureCreatedZipArchive())
					{
						count = miniz::mz_zip_reader_get_num_files(it->second->zipArchive);
						memset(filename, FILENAME_BUFFER, 0);
						for_iter (i, 0, count)
						{
							size = miniz::mz_zip_reader_get_filename(it->second->zipArchive, i, filename, FILENAME_BUFFER - 1);
							if (size < FILENAME_BUFFER)
							{
								filename[size] = '\0';
								if (it->second->cwd == "")
								{
									it->second->internalFiles += it->first + String(filename);
								}
								else if ((int)size > it->second->cwd.size())
								{
									path = String(filename);
									if (path.startsWith(it->second->cwd))
									{
										it->second->internalFiles += it->first + path(it->second->cwd.size(), path.size() - it->second->cwd.size());
									}
								}
							}
						}
					}
				}
				result += it->second->internalFiles;
			}
			return result;
		}

		bool isZipMounts()
		{
			Mutex::ScopeLock lock(&accessMutex);
			return (pathMounts.hasKey("") && pathMounts[""]->ensureCreatedZipArchive());
		}

	}

}
#endif
