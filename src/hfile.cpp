#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "exception.h"
#include "harray.h"
#include "hfile.h"
#include "hstring.h"

#define BUFFER_SIZE (1025)

namespace hltypes
{
/******* CONSTRUCT/DESTRUCT ********************************************/

	file::file(chstr filename, AccessMode access_mode) : cfile(NULL)
	{
		this->filename = filename;
		this->open(this->filename, access_mode);
	}
	
	file::file() : filename(""), cfile(NULL)
	{
	}
	
	file::~file()
	{
		if (this->is_open())
		{
			this->close();
		}
	}
	
/******* METHODS *******************************************************/

	void file::open(chstr filename, AccessMode access_mode)
	{
		if (this->is_open())
		{
			this->close();
		}
		this->filename = filename;
		const char* mode = "rb";
		switch (access_mode)
		{
		case READ:
			mode = "rb";
			break;
		case WRITE:
			mode = "wb";
			break;
		case APPEND:
			mode = "ab";
			break;
		case READ_WRITE:
			mode = "r+b";
			break;
		case READ_WRITE_CREATE:
			mode = "w+b";
			break;
		case READ_APPEND:
			mode = "a+b";
			break;
		}
		this->cfile = fopen(this->filename.c_str(), mode);
		if (this->cfile == NULL)
		{
			throw file_not_found(this->filename.c_str());
		}
	}
	
	hstr file::read_line()
	{
		return this->read("\n");
	}
	
	harray<hstr> file::read_lines()
	{
		return this->read().split("\n");
	}
	
	hstr file::read(chstr delimiter)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		hstr result;
		hstr str;
		harray<hstr> parts;
		int count;
		while (!this->eof())
		{
			char c[BUFFER_SIZE] = {'\0'};
			count = fread(c, 1, BUFFER_SIZE, this->cfile);
			if (count == 0)
			{
				break;
			}
			str = hstr(c);
			if (delimiter != "")
			{
				parts = str.split(delimiter, 1);
				if (parts.size() > 1)
				{
					result += parts[0];
					this->seek(-parts[1].size(), CURRENT);
					break;
				}
			}
			result += str;
		}
		result = result.replace("\r", "");
		return result;
	}
	
	hstr file::read(int count)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		hstr result;
		int current = BUFFER_SIZE - 1;
		int read;
		while (count > 0)
		{
			char c[BUFFER_SIZE] = {'\0'};
			if (count <= BUFFER_SIZE - 1)
			{
				current = count;
			}
			read = fread(c, 1, current, this->cfile);
			if (read == 0)
			{
				break;
			}
			count -= BUFFER_SIZE - 1;
			result += hstr(c);
		}
		return result;
	}

	void file::write(chstr text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(text.c_str(), 1, text.size(), this->cfile);
	}
	
	void file::write(const char* text)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(text, 1, strlen(text), this->cfile);
	}
	
	void file::writef(const char* format, ...)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		char c[BUFFER_SIZE] = {'\0'};
		va_list args;
		va_start(args, format);
		vsnprintf(c, BUFFER_SIZE - 1, format, args);
		va_end(args);
		this->write(c);
	}

	void file::seek(long offset, SeekMode seek_mode)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int mode = SEEK_CUR;
		switch (seek_mode)
		{
		case CURRENT:
			mode = SEEK_CUR;
			break;
		case START:
			mode = SEEK_SET;
			break;
		case END:
			mode = SEEK_END;
			break;
		}
		fseek(this->cfile, offset, mode);
	}
	
	long file::position()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return ftell(this->cfile);
	}
	
	long file::size()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		long position = this->position();
		this->seek(0, END);
		long size = this->position();
		this->seek(position, START);
		return size;
	}
	
	bool file::is_open()
	{
		return (this->cfile != NULL);
	}
	
	bool file::eof()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return (feof(this->cfile) != 0);
	}
	
	void file::close()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fclose(this->cfile);
		this->cfile = NULL;
	}
	
/******* SERIALIZATION DUMP ********************************************/

	void file::dump(unsigned char c)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		fwrite(&c, 1, 1, this->cfile);
	}

	void file::dump(int i)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		this->dump((unsigned int)i);
	}

	void file::dump(unsigned int i)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char bytes[4] = {0};
		bytes[0] = (i >> 24) % 256;
		bytes[1] = (i >> 16) % 256;
		bytes[2] = (i >> 8) % 256;
		bytes[3] = i % 256;
		fwrite(bytes, 1, 4, this->cfile);
	}

	void file::dump(float f)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned int i;
		memcpy(&i, &f, sizeof(f));
		this->dump(i);
	}

	void file::dump(bool b)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c = (b ? 1 : 0);
		fwrite(&c, 1, 1, this->cfile);
	}

	void file::dump(chstr str)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int size = str.size();
		this->dump(size);
		if (size > 0)
		{
			fwrite(str.c_str(), 1, size, this->cfile);
		}
	}

	void file::dump(chstr str, int offset)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int size = str.size();
		this->dump(size);
		if (size > 0)
		{
			const char* string = str.c_str();
			char c[1024] = {'\0'};
			for (int i = 0; i < size; i++)
			{
				c[i] = string[i] - offset;
			}
			fwrite(c, 1, size, this->cfile);
		}
	}
/******* SERIALIZATION LOAD ********************************************/

	unsigned char file::load_uchar()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c;
		fread(&c, 1, 1, this->cfile);
		return c;
	}

	int file::load_int()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		return (int)this->load_uint();
	}

	unsigned int file::load_uint()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char bytes[4] = {0};
		fread(bytes, 1, 4, this->cfile);
		unsigned int i = 0;
		i += bytes[0] << 24;
		i += bytes[1] << 16;
		i += bytes[2] << 8;
		i += bytes[3];
		return i;
	}

	float file::load_float()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned int i = this->load_uint();
		float f;
		memcpy(&f, &i, sizeof(f));
		return f;
	}

	bool file::load_bool()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		unsigned char c;
		fread(&c, 1, 1, this->cfile);
		return (c != 0);
	}

	hstr file::load_hstr()
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int size = this->load_int();
		hstr str;
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

	hstr file::load_hstr(int offset)
	{
		if (!this->is_open())
		{
			throw file_not_open(this->filename.c_str());
		}
		int size = this->load_int();
		hstr str;
		int count = BUFFER_SIZE - 1;
		while (size > 0)
		{
			char c[BUFFER_SIZE] = {'\0'};
			if (size <= BUFFER_SIZE - 1)
			{
				count = size;
			}
			fread(c, 1, count, this->cfile);
			for (int i = 0; i < count; i++)
			{
				c[i] += offset;
			}
			size -= BUFFER_SIZE - 1;
			str += hstr(c);
		}
		return str;
	}

/******* STATIC ********************************************************/

	bool file::exists(chstr filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		if (f != NULL)
		{
			fclose(f);
			return true;
		}
		return false;
	}
	
	bool file::create(chstr filename)
	{
		if (!exists(filename))
		{
			FILE* f = fopen(filename.c_str(), "w");
			if (f != NULL)
			{
				fclose(f);
				return true;
			}
		}
		return false;
	}
	
	bool file::empty(chstr filename)
	{
		if (exists(filename))
		{
			FILE* f = fopen(filename.c_str(), "w");
			if (f != NULL)
			{
				fclose(f);
				return true;
			}
		}
		return false;
	}
	
	/* //2DO - not working for some odd reason
	bool file::remove(chstr filename)
	{
		if (exists(filename) && remove(filename.c_str()) == 0)
		{
			return true;
		}
		return false;
	}
	//*/
	
	hstr file::hread(chstr filename, int count)
	{
		return hfile(filename).read(count);
	}
	
	hstr file::hread(chstr filename, chstr delimiter)
	{
		return hfile(filename).read(delimiter);
	}
	
	void file::hwrite(chstr filename, chstr text)
	{
		hfile(filename, APPEND).write(text);
	}
	
	long file::hsize(chstr filename)
	{
		return hfile(filename).size();
	}
	
};
