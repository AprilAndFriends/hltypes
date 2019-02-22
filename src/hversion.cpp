/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "harray.h"
#include "hltypesUtil.h"
#include "hstring.h"
#include "hversion.h"

namespace hltypes
{
	Version::Version() :
		major(0),
		minor(0),
		revision(0),
		build(0)
	{
	}
	
	Version::Version(unsigned int major, unsigned int minor, unsigned int revision, unsigned int build) :
		major(0),
		minor(0),
		revision(0),
		build(0)
	{
		this->set(major, minor, revision, build);
	}

	Version::Version(const Array<unsigned int>& versions) :
		major(0),
		minor(0),
		revision(0),
		build(0)
	{
		this->set(versions);
	}

	Version::Version(const String& versions) :
		major(0),
		minor(0),
		revision(0),
		build(0)
	{
		this->set(versions);
	}

	bool Version::isValid() const
	{
		return (*this > Version(0, 0, 0, 0));
	}
	
	void Version::set(unsigned int major, unsigned int minor, unsigned int revision, unsigned int build)
	{
		this->major = major;
		this->minor = minor;
		this->revision = revision;
		this->build = build;
	}
	
	void Version::set(const Array<unsigned int>& versions)
	{
		if (versions.size() > 0)
		{
			this->major = versions[0];
			if (versions.size() > 1)
			{
				this->minor = versions[1];
				if (versions.size() > 2)
				{
					this->revision = versions[2];
					if (versions.size() > 3)
					{
						this->build = versions[3];
					}
				}
			}
		}
	}
	
	void Version::set(chstr versions)
	{
		if (Version::isVersionString(versions))
		{
			this->set(versions.split('.', -1, true).cast<unsigned int>());
		}
	}
	
	hstr Version::toString(int count) const
	{
		hstr result = hstr(this->major);
		if (count > 1)
		{
			result += "." + hstr(this->minor);
			if (count > 2)
			{
				result += "." + hstr(this->revision);
				if (count > 3)
				{
					result += "." + hstr(this->build);
				}
			}
		}
		return result;
	}
	
	bool Version::operator<(const Version& other) const
	{
		return (this->major < other.major ||
			(this->major == other.major && (this->minor < other.minor ||
			(this->minor == other.minor && this->revision < other.revision) ||
			(this->revision == other.revision && this->build < other.build))));
	}
	
	bool Version::operator>(const Version& other) const
	{
		return (this->major > other.major ||
			(this->major == other.major && (this->minor > other.minor ||
			(this->minor == other.minor && this->revision > other.revision) ||
			(this->revision == other.revision && this->build > other.build))));
	}
	
	bool Version::operator<=(const Version& other) const
	{
		return (this->operator<(other) || this->operator==(other));
	}
	
	bool Version::operator>=(const Version& other) const
	{
		return (this->operator>(other) || this->operator==(other));
	}
	
	bool Version::operator==(const Version& other) const
	{
		return (this->major == other.major && this->minor == other.minor &&
			this->revision == other.revision && this->build == other.build);
	}
	
	bool Version::operator!=(const Version& other) const
	{
		return (!this->operator==(other));
	}

	bool Version::isVersionString(const String& string)
	{
		harray<String> versions = string.split('.', -1, true);
		if (!hbetweenII(versions.size(), 1, 4))
		{
			return false;
		}
		foreach (String, it, versions)
		{
			if (!(*it).isInt() || (int)(*it) < 0)
			{
				return false;
			}
		}
		return true;
	}
	
}
