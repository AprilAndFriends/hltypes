#include "hconfigfile.h"
#include "hfile.h"
#include "exception.h"

namespace hltypes
{
	config_file::config_file(string filename)
	{
		read(filename);
	}

	config_file::config_file()
	{
		
	}
	
	void config_file::read(string filename)
	{
		this->entries.clear();
		this->filename=filename;
		file f(filename);

		string line,key,value,prefix;
		while (!f.eof())
		{
			line=f.read_line();
			if (line.starts_with("["))
			{
				prefix=line(1,line.find("]")-1)+".";
				continue;
			}
			if (line.starts_with("#") || !line.contains(": ")) continue;
			line.split(": ",key,value);
			this->entries[prefix+key]=value;
		}
		f.close();
	}

	config_file::~config_file()
	{
		this->entries.clear();
	}
	
	chstr config_file::operator[](const char* var)
	{
		if (this->entries.find(var) == this->entries.end()) throw key_error(var,"hltypes::config_file[ "+this->filename+" ]");
		return this->entries[var];
	}

	chstr config_file::operator[](chstr var)
	{
		return this->operator [](var.c_str());
	}
	
	void config_file::set(chstr key,chstr value)
	{
		if (this->entries.find(key) == this->entries.end()) throw key_error(key,"hltypes::config_file[ "+this->filename+" ]");
		this->entries[key]=value;
	}
}