/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <string>
#include "hltypesExport.h"

namespace std
{
	template<typename _Tp> class allocator;
	template<typename _Tp, typename _Alloc> class vector;
}

namespace hltypes
{
	class string;
	typedef std::allocator<hltypes::string> _hstring_allocator;
	typedef std::vector<hltypes::string,_hstring_allocator> string_vector;
	
	class hltypesExport string : public std::basic_string<char>
	{
	public:
		string();
		string(const char* s);
		string(const string& s);
		string(const char* s,const int len);
		string(const string& s,const int len);
		
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
		
		void split(const char splitter,string& out_left,string& out_right) const;
		void split(const char* splitter,string& out_left,string& out_right) const;
		void split(const string& splitter,string& out_left,string& out_right) const;
		void rsplit(const char splitter,string& out_left,string& out_right) const;
		void rsplit(const char* splitter,string& out_left,string& out_right) const;
		void rsplit(const string& splitter,string& out_left,string& out_right) const;
		
		string_vector split(const char splitter,unsigned int n=-1) const;
		string_vector split(const char* splitter,unsigned int n=-1) const;
		string_vector split(const string& splitter,unsigned int n=-1) const;
		string_vector rsplit(const char splitter,unsigned int n=-1) const;
		string_vector rsplit(const char* splitter,unsigned int n=-1) const;
		string_vector rsplit(const string& splitter,unsigned int n=-1) const;
		
		bool startswith(const char* s) const;
		bool startswith(const string& s) const;
		bool endswith(const char* s) const;
		bool endswith(const string& s) const;

		string replace(const char* what,const char* with_what) const;
		string replace(const string& what,const char* with_what) const;
		string replace(const char* what,const string& with_what) const;
		string replace(const string& what,const string& with_what) const;
		
		operator float() const;
		operator int() const;
		
		void operator=(const float f);
		void operator=(const int i);
		void operator=(const bool b);
		void operator=(const std::string& s);
		void operator=(const char* s);

		bool operator==(const float f) const;
		bool operator==(const int i) const;
		bool operator==(const bool b) const;
		bool operator==(const char* s) const;
		bool operator==(const std::string& s) const;

	};
}

typedef hltypes::string hstr;
typedef const hltypes::string& hcstr;
