/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef _HLTYPES_HFILE
#define _HLTYPES_HFILE

#include <stdio.h>

#include "hltypesExport.h"
#include "harray.h"
#include "hmap.h"
#include "hstring.h"

namespace hltypes
{
	enum AccessMode
	{
		READ, // r
		WRITE, // w
		APPEND, // a
		READ_WRITE, // r+
		READ_WRITE_CREATE, // w+
		READ_APPEND, // a+
	};
	
	enum SeekMode
	{
		CURRENT, // SEEK_CUR
		START, // SEEK_SET
		END // SEEK_END
	};
	
	class hltypesExport file
	{
	public:
		file(chstr filename, AccessMode access_mode = READ, int encryption_offset = 0);
		file();
		~file();
		
		void open(chstr filename, AccessMode access_mode = READ, int encryption_offset = 0);
		hstr read_line();
		harray<hstr> read_lines();
		hstr read(chstr delimiter = "");
		hstr read(int count);
		void write(chstr text);
		void write(const char* text);
		void write_line(chstr text);
		void write_line(const char* text);
		void writef(const char* format, ...);
		void seek(long offset, SeekMode seek_mode);
		long position();
		long size();
		bool is_open();
		bool eof();
		void close();
		
		// serialization
		
		void dump(unsigned char c);
		void dump(int i);
		void dump(unsigned int i);
		void dump(float f);
		void dump(bool b);
		void dump(chstr str);
		void dump(const char* c);
		
		unsigned char load_uchar();
		int load_int();
		unsigned int load_uint();
		float load_float();
		bool load_bool();
		hstr load_hstr();
		
		// static
		
		static bool exists(chstr filename);
		static bool create(chstr filename);
		static bool create_new(chstr filename);
		static bool empty(chstr filename);
		static bool rename(chstr old_filename, chstr new_filename);
		static bool move(chstr filename, chstr path);
		static bool remove(chstr filename);
		static bool copy(chstr old_filename, chstr new_filename);
		static long hsize(chstr filename);
		static hstr hread(chstr filename, int count);
		static hstr hread(chstr filename, chstr delimiter = "");
		static void hwrite(chstr filename, chstr text);
		static void happend(chstr filename, chstr text);
		
		static hmap<hstr, hstr> read_cfg(chstr filename);
		
	protected:
		hstr filename;
		FILE* cfile;
		int encryption_offset;
		
	};
}

typedef hltypes::file hfile;

#endif

