#include "exception.h"
#include "hconfigfile.h"
#include "hfile.h"
#include "util.h"

namespace hltypes
{
	configfile::configfile(chstr filename)
	{
		read(filename);
	}

	configfile::configfile()
	{
	}
	
	void configfile::read(chstr filename)
	{
		this->entries.clear();
		this->filename = filename;
		harray<hstr> lines = hfile::hread(this->filename).split("\n");
		hstr key, value, prefix;
		foreach (hstr, it, lines)
		{
			if ((*it).starts_with("["))
			{
				prefix = (*it).replace("[", "").replace("]", "") + ".";
				continue;
			}
			if ((*it).starts_with("#") || !(*it).contains(": "))
			{
				continue;
			}
			(*it).split(": ", key, value);
			this->entries[prefix + key] = value;
		}
	}

	configfile::~configfile()
	{
		this->entries.clear();
	}
	
	chstr configfile::operator[](const char* var)
	{
		if (this->entries.find(var) == this->entries.end())
		{
			throw key_error(var, "hltypes::configfile[ " + this->filename + " ]");
		}
		return this->entries[var];
	}

	chstr configfile::operator[](chstr var)
	{
		return this->operator[](var.c_str());
	}
	
	void configfile::set(chstr key, chstr value)
	{
		if (this->entries.find(key) == this->entries.end())
		{
			throw key_error(key, "hltypes::configfile[ " + this->filename + " ]");
		}
		this->entries[key] = value;
	}
	
}