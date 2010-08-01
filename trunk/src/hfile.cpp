#include "hfile.h"
#include "exception.h"
#include <stdio.h>
#include <string.h>

namespace hltypes
{
	file::file(string filename,const char* read_mode)
	{
		this->cfile=fopen(filename.c_str(),read_mode);
		if (!this->cfile) throw file_not_found(filename);
	}
	
	file::file(const char* filename,const char* read_mode)
	{
		this->cfile=fopen(filename,read_mode);
		if (!this->cfile) throw file_not_found(filename);
	}
	
	file::~file()
	{
		if (this->cfile)
			fclose(this->cfile);
	}
	
	string file::read_line()
	{
		hstr result;
		char line[128]="";
		char* ret;
		int len;
		for (;;)
		{
			ret=fgets(line,128,this->cfile);
			len=strlen(line);
			if (!ret || line[len-1] == '\n')
			{
				if (line[len-1] == '\n') line[len-1]=0;
				if (line[len-2] == '\r') line[len-2]=0;
				result+=line;
				break;
			}
			else result+=line;
		}
		return result;
	}
	
	bool file::eof()
	{
		return feof(this->cfile);
	}
	
	void file::close()
	{
		fclose(this->cfile);
		this->cfile=0;
	}
	
	bool file::exists(string filename)
	{
		FILE* f=fopen(filename.c_str(),"r");
		if (f) { fclose(f); return 1; }
		else return 0;
	}
};
