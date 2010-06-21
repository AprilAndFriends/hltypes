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

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

/******* STRING ****************************************************************************/

TEST(float_string)
{
	hstr float_string("5.75");
	float f = float_string;
	float_string = 6.75f;
	
	CHECK(f == 5.75f && float_string == 6.75f);
}

TEST(int_string)
{
	hstr int_string("5");
	int f = int_string;
	int_string = 6;
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
	std::string str1, str2 = "text2";
	hstr hs1 = "text1", hs2, hs3("text1");
	str1 = hs1;
	hs2 = str2;
	
	CHECK(hs1 == str1 && hs1 == "text1" &&
	      hs2 == str2 && hs2 == "text2" &&
		  hs1 != hs2 && hs1 == hs3);
}

TEST(string_cstr_compatibilty)
{
	hstr s1, s2;
	char cstr[64] = "text2";
	s1 = "text1";
	s2 = cstr;
	
	CHECK(s1 == "text1" && s2 == cstr && s2 == "text2");
}

TEST(string_replace)
{
	hstr s1 = "1 2 3 4 5 6 7 8 9", s2 = "101101010100011010", s3 = "test", s4 = "0";
	CHECK(s1.replace(" ", "") == "123456789" &&
	      s2.replace(s4, "") == "111111111" &&
		  s3.replace("a", "b") == "test" &&
		  s3.replace("es", "his is a tes") == "this is a test");
}

TEST(string_starts_with)
{
	hstr s1 = "this is a test";
	
	CHECK(s1.starts_with("this") && !s1.starts_with("something"));
}

TEST(string_ends_with)
{
	hstr s1 = "this is a test";
	
	CHECK(s1.ends_with("test") && !s1.ends_with("something"));
}

TEST(string_split1)
{
	hstr s = "1,2,3,4,5,6,7,8,9", s2 = "test", splitter = "!";
	harray<hstr> ary1 = s.split(",");
	harray<hstr> ary2 = s.split(",", 2);

	CHECK(ary1.size() == 9 && ary1[0] == "1" && ary1[1] == "2" && ary1[2] == "3" && ary1[3] == "4" &&
	      ary1[4] == "5" && ary1[5] == "6" && ary1[6] == "7" && ary1[7] == "8" && ary1[8] == "9" &&
		  s2.split(',')[0] == "test" && s2.split(splitter).size() == 1 &&
		  ary2.size() == 3 && ary2[0] == "1" && ary2[1] == "2" && ary2[2] == "3,4,5,6,7,8,9" &&
		  s.split("3,4").size() == 2);
}

TEST(string_split2)
{
	hstr s = "1,2,3";
	harray<hstr> ary = s.split(",");
	CHECK(ary.size() == 3 && ary[0] == "1" && ary[1] == "2" && ary[2] == "3");
	s = "1,2,";
	ary = s.split(",");
	CHECK(ary.size() == 3 && ary[0] == "1" && ary[1] == "2" && ary[2] == "");
	s = "1,,3";
	ary = s.split(",");
	CHECK(ary.size() == 3 && ary[0] == "1" && ary[1] == "" && ary[2] == "3");
}

TEST(string_rsplit1)
{
	hstr s = "1,2,3,4,5,6,7,8,9";
	harray<hstr> ary1 = s.rsplit(",");

	CHECK(ary1.size() == 9);
	CHECK(ary1[0] == "1" && ary1[1] == "2" && ary1[2] == "3" && ary1[3] == "4" &&
	      ary1[4] == "5" && ary1[5] == "6" && ary1[6] == "7" && ary1[7] == "8" && ary1[8] == "9");
}

TEST(string_rsplit2)
{
	hstr s = "1,2,3,4,5,6,7,8,9";
	hstr s2 = "test", splitter="!";
	harray<hstr> ary1 = s.rsplit(",", 2);
	harray<hstr> ary2 = s2.rsplit(',');
	CHECK(ary2[0] == "test" && s2.rsplit(splitter).size() == 1 &&
		  ary1.size() == 3 && ary1[0] == "1,2,3,4,5,6,7" && ary1[1] == "8" && ary1[2] == "9");
}

TEST(string_rsplit3)
{
	hstr s = "1,2,3";
	harray<hstr> ary = s.rsplit(",");
	CHECK(ary.size() == 3 && ary[0] == "1" && ary[1] == "2" && ary[2] == "3");
	s = "1,2,";
	ary = s.rsplit(",");
	CHECK(ary.size() == 3 && ary[0] == "1" && ary[1] == "2" && ary[2] == "");
	s = "1,,3";
	ary = s.rsplit(",");
	CHECK(ary.size() == 3 && ary[0] == "1" && ary[1] == "" && ary[2] == "3");
}

TEST(strin_substr_operator)
{
	hstr s = "1234567890";
	CHECK(s(1, 2) == "23");
	CHECK(s(6, 4) == "7890");
	CHECK(s(0, 4, 2) == "13");
	CHECK(s(1, 5, 2) == "246");
	CHECK(s(6) == "7");
	CHECK(s[3] == '4');
}

