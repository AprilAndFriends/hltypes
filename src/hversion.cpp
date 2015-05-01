/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "harray.h"
#include "hstring.h"
#include "hversion.h"

namespace hltypes
{
	Version::Version()
	{
		this->set(0, 0, 0, 0);
	}
	
	Version::Version(unsigned int major, unsigned int minor, unsigned int revision, unsigned int build)
	{
		this->set(major, minor, revision, build);
	}
	
	Version::~Version()
	{
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
	
	void Version::set(Array<unsigned int> versions)
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
		this->set(versions.split('.').cast<unsigned int>());
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
	
}
