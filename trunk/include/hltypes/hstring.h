/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef _HLTYPES_HSTRING
#define _HLTYPES_HSTRING

#include <string>

#include "hltypesExport.h"

namespace hltypes
{
	template <class T> class Array;
	class string;
	typedef hltypes::Array<hltypes::string> harray_hstr;
	
	class hltypesExport string : public std::basic_string<char>
	{
	public:
		string();
		string(const char c);
		string(const char c, const int len);
		string(const char* s);
		string(const string& s);
		string(const std::string& s);
		string(const char* s, const int len);
		string(const string& s, const int len);
		string(const int i);
		string(const float f);
		
		/* TODO:
		 * joining (as a global function)
		 * count: counts number of subsbstrings in str
		 * void operator()(int a,int b) - substring operator similar to python
		 * reversed
		 * capitalize
		 * upper
		 * lower
		 * swapcase
		 * utf8_len
		 * splitlines
		 * strip,rstrip
		 * constructor with printf format style [ string(const char* format,...) ]
		 * scanf
		 * +,+= operators
		 * */
		
		bool split(const char splitter, string& out_left, string& out_right) const;
		bool split(const char* splitter, string& out_left, string& out_right) const;
		bool split(const string& splitter, string& out_left, string& out_right) const;
		bool rsplit(const char splitter, string& out_left, string& out_right) const;
		bool rsplit(const char* splitter, string& out_left, string& out_right) const;
		bool rsplit(const string& splitter, string& out_left, string& out_right) const;
		
		harray_hstr split(const char splitter, unsigned int n = -1) const;
		harray_hstr split(const char* splitter, unsigned int n = -1) const;
		harray_hstr split(const string& splitter, unsigned int n = -1) const;
		harray_hstr rsplit(const char splitter, unsigned int n = -1) const;
		harray_hstr rsplit(const char* splitter, unsigned int n = -1) const;
		harray_hstr rsplit(const string& splitter, unsigned int n = -1) const;
		
		bool starts_with(const char* s) const;
		bool starts_with(const string& s) const;
		bool ends_with(const char* s) const;
		bool ends_with(const string& s) const;
		
		string lower() const;
		string upper() const;

		string replace(const char* what, const char* with_what) const;
		string replace(const string& what, const char* with_what) const;
		string replace(const char* what, const string& with_what) const;
		string replace(const string& what, const string& with_what) const;
		string replace(size_t pos1, size_t n1, const string& str);
		string replace(size_t pos1, size_t n1, const string& str, size_t pos2, size_t n2);
		string replace(size_t pos1, size_t n1, const char* s, size_t n2);
		string replace(size_t pos1, size_t n1, const char* s);
		string replace(size_t pos1, size_t n1, size_t n2, char c);
		
		bool contains(const char* s) const;
		bool contains(const char c) const;
		bool contains(const string& s) const;
		
		int size() const;

		operator float() const;
		operator int() const;
		operator unsigned int() const;
		operator bool() const;
		
		void operator=(const float f);
		void operator=(const int i);
		void operator=(const bool b);
		void operator=(const std::string& s);
		void operator=(const char* s);
		
		void operator+=(const float f);
		void operator+=(const int i);
		void operator+=(const bool b);
		void operator+=(const char c);
		void operator+=(const std::string& s);
		void operator+=(const char* s);
		
		string operator+(const char* s1) const;
		string operator+(const char c) const;
		string operator+(char* s1) const;
		string operator+(const string& s1) const;
		string operator+(const std::string& s1) const;

		bool operator==(const float f) const;
		bool operator==(const int i) const;
		bool operator==(const bool b) const;
		bool operator==(const char* s) const;
		bool operator==(const std::string& s) const;
		
		string operator()(int start, int count) const;
		string operator()(int start, int count, int step) const;
		string operator()(int index) const;
		char& operator[](int index);
	//	string operator()(int a,int b) const;
		//string operator()(int a,int b,int step) const;

	};
}

typedef hltypes::string hstr;
typedef const hltypes::string& chstr;

hstr hltypesFnExport operator+(const char* s1, chstr s2);
hstr hltypesFnExport operator+(char* s1, chstr s2);
hstr hltypesFnExport hsprintf(const char* format, ...);
hstr hltypesFnExport hsprintf_s(const char* format, ...);

#endif

