/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <hltypes/Array.h>
#include <hltypes/hstring.h>

/******* STRING ****************************************************************************/

TEST(float_string)
{
	hstr float_string("5.75");
	float f=float_string;
	float_string=6.75f;
	
	CHECK(f == 5.75f && float_string == 6.75f);
}

TEST(int_string)
{
	hstr int_string("5");
	int f=int_string;
	int_string=6;
	CHECK(f == 5 && int_string == 6);
}

TEST(bool_string1) { hstr b("1");     CHECK(b == true);  }
TEST(bool_string2) { hstr b("0");     CHECK(b == false); }
TEST(bool_string3) { hstr b("true");  CHECK(b == true);  }
TEST(bool_string4) { hstr b("false"); CHECK(b == false); }
TEST(bool_string5) { hstr b; b=true;  CHECK(b == true);  }
TEST(bool_string6) { hstr b; b=false; CHECK(b == false); }

TEST(string_stdstr_compatibility)
{
	std::string str1,str2="text2";
	hstr hs1="text1",hs2,hs3("text1");
	str1=hs1;
	hs2=str2;
	
	CHECK(hs1 == str1 && hs1 == "text1" &&
	      hs2 == str2 && hs2 == "text2" &&
		  hs1 != hs2 && hs1 == hs3);
}

TEST(string_cstr_compatibilty)
{
	hstr s1,s2;
	char cstr[64]="text2";
	s1="text1";
	s2=cstr;
	
	CHECK(s1 == "text1" && s2 == cstr && s2 == "text2");
}

TEST(string_replace)
{
	hstr s1="1 2 3 4 5 6 7 8 9",s2="101101010100011010",s3="test",s4="0";
	CHECK(s1.replace(" ","") == "123456789" &&
	      s2.replace(s4,"") == "111111111" &&
		  s3.replace("a","b") == "test" &&
		  s3.replace("es","his is a tes") == "this is a test");
}

TEST(string_starts_with)
{
	hstr s1="this is a test";
	
	CHECK(s1.starts_with("this") && !s1.starts_with("something"));
}

TEST(string_ends_with)
{
	hstr s1="this is a test";
	
	CHECK(s1.ends_with("test") && !s1.ends_with("something"));
}

TEST(string_split1)
{
	hstr s="1,2,3,4,5,6,7,8,9",s2="test",splitter="!";
	harray<hstr> v=s.split(",");
	harray<hstr> v2=s.split(",",2);

	CHECK(v.size() == 9 && v[0] == "1" && v[1] == "2" && v[2] == "3" && v[3] == "4" &&
	      v[4] == "5" && v[5] == "6" && v[6] == "7" && v[7] == "8" && v[8] == "9" &&
		  s2.split(',')[0] == "test" && s2.split(splitter).size() == 1 &&
		  v2.size() == 3 && v2[0] == "1" && v2[1] == "2" && v2[2] == "3,4,5,6,7,8,9" &&
		  s.split("3,4").size() == 2);
}

TEST(string_rsplit1)
{
	hstr s="1,2,3,4,5,6,7,8,9";
	harray<hstr> v=s.rsplit(",");

	CHECK(v.size() == 9);
	CHECK(v[0] == "1" && v[1] == "2" && v[2] == "3" && v[3] == "4" &&
	      v[4] == "5" && v[5] == "6" && v[6] == "7" && v[7] == "8" && v[8] == "9");
}

TEST(string_rsplit2)
{
	hstr s="1,2,3,4,5,6,7,8,9";
	hstr s2="test",splitter="!";
	harray<hstr> v2=s.rsplit(",",2);
	harray<hstr> v3=s2.rsplit(',');
	CHECK(v3[0] == "test" && s2.rsplit(splitter).size() == 1 &&
		  v2.size() == 3 && v2[0] == "1,2,3,4,5,6,7" && v2[1] == "8" && v2[2] == "9");
}
