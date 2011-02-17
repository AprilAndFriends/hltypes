/**
 * @file
 * @author  Kresimir Spes
 * @author  Boris Mikic
 * @author  Ivan Vucica
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
 *
 * @section DESCRIPTION
 *
 * Provides high level file handling.
 */
#ifndef HLTYPES_FILE_H
#define HLTYPES_FILE_H

#include <stdio.h>

#include "hstring.h"

#include "hltypesExport.h"

namespace hltypes
{
	template <class T> class Array;
	/**
	 * @brief Provides high level file handling.
	 * @author Kresimir Spes
	 * @author Boris Mikic
	 * @author Ivan Vucica
	 */
	class hltypesExport File
	{
	public:
		/**
		 * @brief Defines file access modes.
		 */
		enum AccessMode
		{
			/**
			 * @brief Read-only file mode. ("r")
			 */
			READ,
			/**
			 * @brief Write-only file mode. ("w")
			 */
			WRITE,
			/**
			 * @brief Write and append file mode. ("a")
			 */
			APPEND,
			/**
			 * @brief Read and write file mode. ("r+")
			 */
			READ_WRITE,
			/**
			 * @brief Read, write and create file mode. ("w+")
			 */
			READ_WRITE_CREATE,
			/**
			 * @brief Read and append file mode. ("a+")
			 */
			READ_APPEND
		};
		
		/**
		 * @brief Defines file seek modes.
		 */
		enum SeekMode
		{
			/**
			 * @brief Seek from current position. (SEEK_CUR)
			 */
			CURRENT,
			/**
			 * @brief Seek from start position. (SEEK_SET)
			 */
			START,
			/**
			 * @brief Seek from current position. (SEEK_END)
			 */
			END
		};
		
		File(chstr filename, AccessMode access_mode = READ, int encryption_offset = 0);
		File();
		~File();
		
		void open(chstr filename, AccessMode access_mode = READ, int encryption_offset = 0);
		hstr read_line();
		Array<hstr> read_lines();
		hstr read(chstr delimiter = "");
		hstr read(int count);
		void write(chstr text);
		void write(const char* text);
		void write_line(chstr text);
		void write_line(const char* text);
		void writef(const char* format, ...);
		void read_raw(void* buffer, int count);
		void write_raw(void* buffer, int count);
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
		void dump(short s);
		void dump(unsigned short s);
		void dump(float f);
		void dump(double d);
		void dump(bool b);
		void dump(chstr str);
		void dump(const char* c);
		
		unsigned char load_uchar();
		int load_int();
		unsigned int load_uint();
		short load_short();
		unsigned short load_ushort();
		float load_float();
		double load_double();
		bool load_bool();
		hstr load_hstr();
		
		// static
		
		static bool create(chstr filename);
		static bool create_new(chstr filename);
		static bool remove(chstr filename);
		static bool exists(chstr filename);
		static bool clear(chstr filename);
		static bool rename(chstr old_filename, chstr new_filename);
		static bool move(chstr filename, chstr path);
		static bool copy(chstr old_filename, chstr new_filename);
		
		static long hsize(chstr filename);
		static hstr hread(chstr filename, int count);
		static hstr hread(chstr filename, chstr delimiter = "");
		static void hwrite(chstr filename, chstr text);
		static void happend(chstr filename, chstr text);
		
	protected:
		hstr filename;
		FILE* cfile;
		int encryption_offset;
		
	};
}

/**
 * @brief Alias for simpler code.
 */
typedef hltypes::File hfile;

#endif

