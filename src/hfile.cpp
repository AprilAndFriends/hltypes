#include <stdio.h>
#include <string.h>

#include "exception.h"
#include "hfile.h"
#include "hstring.h"

namespace hltypes
{
	file::file(chstr filename, const char* access_mode)
	{
		this->open(filename.c_str(), access_mode);
	}
	
	file::file(const char* filename, const char* access_mode)
	{
		this->open(filename, access_mode);
	}
	
	file::file()
	{
		this->cfile = NULL;
	}
	
	file::~file()
	{
		if (this->cfile)
		{
			fclose(this->cfile);
		}
	}
	
	void file::open(const char* filename, const char* access_mode)
	{
		this->cfile = fopen(filename, access_mode);
		if (this->cfile == NULL)
		{
			throw file_not_found(filename);
		}
	}
	
	hstr file::read_line()
	{
		hstr result;
		char line[128] = "";
		char* ret;
		int len;
		while (!this->eof())
		{
			ret = fgets(line, 128, this->cfile);
			len = strlen(line);
			if (!ret || line[len - 1] == '\n')
			{
				if (line[len - 1] == '\n')
				{
					line[len - 1] = '\0';
				}
				if (line[len - 2] == '\r')
				{
					line[len - 2] = '\0';
				}
				result += line;
				break;
			}
			result += line;
		}
		return result;
	}
	
	bool file::eof()
	{
		return (feof(this->cfile) != 0);
	}
	
	void file::close()
	{
		fclose(this->cfile);
		this->cfile = NULL;
	}
	
	bool file::exists(chstr filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		if (f)
		{
			fclose(f);
			return true;
		}
		return false;
	}
	
};
