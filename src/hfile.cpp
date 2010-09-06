#include <stdio.h>
#include <string.h>

#include "exception.h"
#include "hfile.h"
#include "hstring.h"

#define BUFFER_SIZE (1025)

namespace hltypes
{
/******* CONSTRUCT/DESTRUCT ********************************************/
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
	
/******* METHODS *******************************************************/
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
	
/******* SERIALIZATION DUMP ********************************************/
	void file::dump(unsigned char c)
	{
		fwrite(&c, 1, 1, this->cfile);
	}

	void file::dump(int i)
	{
		this->dump((unsigned int)i);
	}

	void file::dump(unsigned int i)
	{
		unsigned char bytes[4] = {0};
		bytes[0] = i % 256;
		bytes[1] = (i >> 8) % 256;
		bytes[2] = (i >> 16) % 256;
		bytes[3] = (i >> 24) % 256;
		fwrite(bytes, 1, 4, this->cfile);
	}

	void file::dump(float f)
	{
		unsigned int i;
		memcpy(&i, &f, sizeof(f));
		this->dump(i);
	}

	void file::dump(bool b)
	{
		unsigned char c = (b ? 1 : 0);
		fwrite(&c, 1, 1, this->cfile);
	}

	void file::dump(chstr str)
	{
		int size = str.size();
		this->dump(size);
		if (size > 0)
		{
			fwrite(str.c_str(), 1, size, this->cfile);
		}
	}

/******* SERIALIZATION LOAD ********************************************/
	unsigned char file::load_uchar()
	{
		unsigned char c;
		fread(&c, 1, 1, this->cfile);
		return c;
	}

	int file::load_int()
	{
		return (int)this->load_uint();
	}

	unsigned int file::load_uint()
	{
		unsigned char bytes[4] = {0};
		fread(bytes, 1, 4, this->cfile);
		unsigned int i = 0;
		i += bytes[0];
		i += bytes[1] << 8;
		i += bytes[2] << 16;
		i += bytes[3] << 24;
		return i;
	}

	float file::load_float()
	{
		unsigned int i = this->load_uint();
		float f;
		memcpy(&f, &i, sizeof(f));
		return f;
	}

	bool file::load_bool()
	{
		unsigned char c;
		fread(&c, 1, 1, this->cfile);
		return (c != 0);
	}

	hstr file::load_hstr()
	{
		int size = this->load_int();
		hstr str = "";
		int count = BUFFER_SIZE - 1;
		while (size > 0)
		{
			char c[BUFFER_SIZE] = {'\0'};
			if (size <= BUFFER_SIZE - 1)
			{
				count = size;
			}
			fread(c, 1, count, this->cfile);
			size -= BUFFER_SIZE - 1;
			str += hstr(c);
		}
		return str;
	}

	
	
};
