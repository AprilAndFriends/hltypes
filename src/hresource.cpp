/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "hdir.h"
#include "hexception.h"
#include "hfile.h"
#include "hlog.h"
#include "hrdir.h"
#include "hresource.h"
#include "hthread.h"
#include "platform_internal.h"
#ifdef _ZIPRESOURCE
#include "zipaccess.h"
#endif

namespace hltypes
{
	bool Resource::zipMounts = false;
	Map<String, String> Resource::mountedArchives;

	bool Resource::mountArchive(const String& path, const String& archiveFilename, const String& cwd)
	{
		String normalizedPath = ResourceDir::normalize(path);
		if (normalizedPath == ".")
		{
			normalizedPath = "";
		}
		if (normalizedPath != "" && !normalizedPath.endsWith("/"))
		{
			normalizedPath += "/";
		}
		if (normalizedPath != "" && !Resource::zipMounts)
		{
			Log::errorf(logTag, "Cannot mount archive filename '%s' to path '%s', a default path archive required first!", archiveFilename.cStr(), path.cStr());
			return false;
		}
		if (Resource::mountedArchives.hasKey(normalizedPath))
		{
			Log::errorf(logTag, "Cannot mount archive filename '%s' to path '%s', the path is already mounted!", archiveFilename.cStr(), path.cStr());
			return false;
		}
		Array<String> mounts = Resource::mountedArchives.keys() / "";
		foreach (String, it, mounts)
		{
			if (normalizedPath.startsWith(*it))
			{
				Log::errorf(logTag, "Cannot mount archive filename '%s' to path '%s', the prefix path '%s' is already mounted!", archiveFilename.cStr(), path.cStr(), (*it).cStr());
				return false;
			}
			if ((*it).startsWith(normalizedPath))
			{
				Log::errorf(logTag, "Cannot mount archive filename '%s' to path '%s', it's a prefix to already mounted path '%s'!", archiveFilename.cStr(), path.cStr(), (*it).cStr());
				return false;
			}
		}
		String normalizedArchiveFilename = ResourceDir::normalize(archiveFilename);
#ifdef _ZIPRESOURCE
		String normalizedCwd = ResourceDir::normalize(cwd);
		if (normalizedCwd == ".")
		{
			normalizedCwd = "";
		}
		if (normalizedCwd != "" && !normalizedCwd.endsWith("/"))
		{
			normalizedCwd += "/";
		}
		if (!zip::mountArchive(normalizedPath, normalizedArchiveFilename, normalizedCwd))
		{
			Log::errorf(logTag, "Cannot mount archive filename '%s' to path '%s', internal ZIP error!", archiveFilename.cStr(), path.cStr());
			return false;
		}
		// extra mounting has to clear directory / file caches
		ResourceDir::cacheDirectories.clear();
		ResourceDir::cacheFiles.clear();
#endif
		Resource::mountedArchives[normalizedPath] = normalizedArchiveFilename;
		if (normalizedPath == "")
		{
#ifdef _ZIPRESOURCE
			Log::write(logTag, "Mounted default path. ZIP available: yes");
			Resource::zipMounts = zip::isZipMounts();
			return Resource::zipMounts;
#else
			Log::write(logTag, "Mounted default path. ZIP available: no");
#endif
		}
		return true;
	}

	bool Resource::unmountArchive(const String& path)
	{
		String normalizedPath = ResourceDir::normalize(path);
		if (normalizedPath == ".")
		{
			normalizedPath = "";
		}
		if (normalizedPath != "" && !normalizedPath.endsWith("/"))
		{
			normalizedPath += "/";
		}
		if (!Resource::mountedArchives.hasKey(normalizedPath))
		{
			Log::errorf(logTag, "Cannot unmount path '%s', the path is not mounted!", path.cStr());
			return false;
		}
		if (normalizedPath == "" && Resource::mountedArchives.size() > 1)
		{
			Log::error(logTag, "Cannot unmount default path, other paths are still mounted!");
			return false;
		}
#ifdef _ZIPRESOURCE
		if (!zip::unmountArchive(normalizedPath))
		{
			Log::errorf(logTag, "Cannot unmount path '%s', internal ZIP error!", path.cStr());
			return false;
		}
		if (normalizedPath == "")
		{
			Resource::zipMounts = false;
		}
		// unmounting has to clear directory / file caches
		ResourceDir::cacheDirectories.clear();
		ResourceDir::cacheFiles.clear();
#endif
		Resource::mountedArchives.removeKey(normalizedPath);
		return true;
	}

	Resource::Resource() : FileBase(), dataPosition(0)
	{
	}

	Resource::~Resource()
	{
		if (this->_isOpen())
		{
			this->close();
		}
	}
	
	void Resource::open(const String& filename)
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			if (this->_isOpen())
			{
				this->close();
			}
			this->filename = this->resourceFilename = ResourceDir::normalize(filename);
			int attempts = Resource::repeats + 1;
			while (true)
			{
				this->cfile = zip::fopen(this, this->filename);
				if (this->cfile != NULL)
				{
					break;
				}
				--attempts;
				if (attempts <= 0)
				{
					break;
				}
				Thread::sleep(Resource::timeout);
			}
			if (this->cfile == NULL)
			{
				throw ResourceFileCouldNotOpenException(this->_descriptor());
			}
			this->_updateDataSize();
			return;
		}
#endif
		this->_fopen(Resource::_makeNonZipPath(filename), AccessMode::Read, FileBase::repeats, FileBase::timeout);
		this->resourceFilename = Dir::normalize(filename);
		this->_updateDataSize();
	}
	
	void Resource::close()
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			this->_validate();
			if (this->cfile != NULL)
			{
				zip::fclose(this, this->cfile);
				this->cfile = NULL;
			}
			this->dataSize = 0;
			this->dataPosition = 0;
			return;
		}
#endif
		this->_fclose();
		this->dataPosition = 0;
	}
	
	bool Resource::hasZip()
	{
#ifdef _ZIPRESOURCE
		return true;
#else
		return false;
#endif
	}

	void Resource::_updateDataSize()
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			this->dataSize = zip::fsize(this->cfile);
			return;
		}
#endif
		// using hinfo, because the default FileBase implementation with _position() and _seek() can cause problems and Resources are always read-only anyway
		this->dataSize = Resource::hinfo(this->resourceFilename).size;
	}

	int Resource::_read(void* buffer, int count)
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			this->dataPosition += count;
			return zip::fread(this->cfile, buffer, count);
		}
#endif
		return this->_fread(buffer, count);
	}
	
	int Resource::_write(const void* buffer, int count)
	{
		throw FileNotWriteableException(this->filename);
	}

	bool Resource::_isOpen() const
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			return (this->cfile != NULL);
		}
#endif
		return this->_fisOpen();
	}
	
	int64_t Resource::_position() const
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			return zip::fposition(this->cfile);
		}
#endif
		return this->_fposition();
	}
	
	bool Resource::_seek(int64_t offset, SeekMode seekMode)
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			bool result = zip::fseek(this->cfile, offset, seekMode);
			this->dataPosition = zip::fposition(this->cfile);
			return result;
		}
#endif
		return this->_fseek(offset, seekMode);
	}
	
	bool Resource::exists(const String& filename, bool caseSensitive)
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			hstr name = ResourceDir::normalize(filename);
			if (name == "")
			{
				return false;
			}
			bool result = zip::fexists(name);
			if (!result && !caseSensitive)
			{
				String name = filename;
				String baseDir = ResourceDir::baseDir(name);
				String baseName = ResourceDir::baseName(name);
				Array<String> files = ResourceDir::files(baseDir);
				foreach (String, it, files)
				{
					if ((*it).lowered() == baseName.lowered())
					{
						name = ResourceDir::joinPath(baseDir, (*it));
						result = true;
						break;
					}
				}
			}
			return result;
		}
#endif
		return FileBase::_fexists(Resource::_makeNonZipPath(filename), caseSensitive);
	}
	
	String Resource::hread(const String& filename, int count)
	{
		Resource file;
		file.open(filename);
		return file.read(count);
	}
	
	String Resource::hread(const String& filename, const String& delimiter)
	{
		Resource file;
		file.open(filename);
		return file.read(delimiter);
	}

	FileInfo Resource::hinfo(const String& filename)
	{
#ifdef _ZIPRESOURCE
		if (Resource::zipMounts)
		{
			return zip::finfo(ResourceDir::normalize(filename));
		}
#endif
		return File::hinfo(Resource::_makeNonZipPath(filename));
	}

	String Resource::_makeNonZipPath(const String& filename)
	{
		Array<String> segments;
		String platformCwd = _platformResourceCwd();
		if (platformCwd != "")
		{
			segments += platformCwd;
		}
		String defaultArchive = Resource::mountedArchives.tryGet("", "");
		if (defaultArchive != "")
		{
			segments += defaultArchive;
		}
		segments += filename;
		String result = ResourceDir::normalize(ResourceDir::joinPaths(segments));
		if (!result.startsWith('/') && filename.startsWith('/'))
		{
			result = "/" + result;
		}
		return result;
	}

	Resource::Resource(const Resource& other)
	{
		throw ObjectCannotCopyException("hltypes::Resource");
	}

	Resource& Resource::operator=(Resource& other)
	{
		throw ObjectCannotAssignException("hltypes::Resource");
		return (*this);
	}

}

