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
		this->Major = major;
		this->Minor = minor;
		this->Revision = revision;
		this->Build = build;
	}
	
	void Version::set(harray<unsigned int> versions)
	{
		if (versions.size() > 0)
		{
			this->Major = versions[0];
			if (versions.size() > 1)
			{
				this->Minor = versions[1];
				if (versions.size() > 2)
				{
					this->Revision = versions[2];
					if (versions.size() > 3)
					{
						this->Build = versions[3];
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
		hstr result = hstr(this->Major);
		if (count > 1)
		{
			result += "." + hstr(this->Minor);
			if (count > 2)
			{
				result += "." + hstr(this->Revision);
				if (count > 3)
				{
					result += "." + hstr(this->Build);
				}
			}
		}
		return result;
	}
	
	bool Version::operator<(const Version& other) const
	{
		return (this->Major < other.Major ||
			(this->Major == other.Major && (this->Minor < other.Minor ||
			(this->Minor == other.Minor && this->Revision < other.Revision) ||
			(this->Revision == other.Revision && this->Build < other.Build))));
	}
	
	bool Version::operator>(const Version& other) const
	{
		return (this->Major > other.Major ||
			(this->Major == other.Major && (this->Minor > other.Minor ||
			(this->Minor == other.Minor && this->Revision > other.Revision) ||
			(this->Revision == other.Revision && this->Build > other.Build))));
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
		return (this->Major == other.Major && this->Minor == other.Minor &&
			this->Revision == other.Revision && this->Build == other.Build);
	}
	
	bool Version::operator!=(const Version& other) const
	{
		return (!this->operator==(other));
	}
	
}
