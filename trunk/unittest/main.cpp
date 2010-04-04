/************************************************************************************\
* This source file is part of the High Level C++ types library                       *
* For latest info, see http://libhltypes.sourceforge.net/                            *
**************************************************************************************
* Copyright (c) 2010 Kresimir Spes, Boris Mikic, Domagoj Cerjan                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <unittest++/UnitTest++.h>
#include <hltypes/Array.h>
#include <hltypes/hstring.h>
#include <vector>


TEST(Array_Test)
{
	CHECK(1);
	/*
	hltypes::Array<int> a;
	a.append(5); a.append(6); a.append(7);
	CHECK(a[0] == 5 && a[1] == 6 && a[2] == 7);
*/
}

class test_struct
{
public:
	float x,y,z;
	test_struct() {x=y=z=0; }
	test_struct(float _x,float _y,float _z) { x=_x; y=_y; z=_z; }
	bool operator ==(const test_struct& t) { return x==t.x && y==t.y && z==t.z; }
};

TEST(Array_Struct_Test)
{
	CHECK(1);
	/*
	hltypes::Array<test_struct> a;
	a.append(test_struct(0,0,0));
	a.append(test_struct(1,2,3));
	a.append(test_struct(6,6,6));
	CHECK(a[0] == test_struct(0,0,0) &&
	      a[1] == test_struct(1,2,3) &&
		  a[2] == test_struct(6,6,6));
*/
}


TEST(float_string)
{
	hstr float_string("5.75");
	float f=float_string;
	float_string=6.75f;
	
	CHECK(f == 5.75f && float_string == 6.75f);
}



/******* STRING ****************************************************************************/
TEST(int_string)
{
	hstr int_string("5");
	int f=int_string;
	int_string=6;
	CHECK(f == 5 && int_string == 6);
}

TEST(bool_string1) { hstr b("1");     CHECK(b == true);   }
TEST(bool_string2) { hstr b("0");     CHECK(b == false);  }
TEST(bool_string3) { hstr b("true");  CHECK(b == true);   }
TEST(bool_string4) { hstr b("false"); CHECK(b == false);  }
TEST(bool_string5) { hstr b; b=true;  CHECK(b == true);   }
TEST(bool_string6) { hstr b; b=false; CHECK(b == false);  }

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

TEST(string_startswith)
{
	hstr s1="this is a test";
	
	CHECK(s1.startswith("this") && !s1.startswith("something"));
}

TEST(string_endswith)
{
	hstr s1="this is a test";
	
	CHECK(s1.endswith("test") && !s1.endswith("something"));
}

TEST(string_split1)
{
	hstr s="1,2,3,4,5,6,7,8,9",s2="test",splitter="!";
	std::vector<hstr> v=s.split(",");
	std::vector<hstr> v2=s.split(",",2);

	CHECK(v.size() == 9 && v[0] == "1" && v[1] == "2" && v[2] == "3" && v[3] == "4" &&
	      v[4] == "5" && v[5] == "6" && v[6] == "7" && v[7] == "8" && v[8] == "9" &&
		  s2.split(',')[0] == "test" && s2.split(splitter).size() == 1 &&
		  v2.size() == 3 && v2[0] == "1" && v2[1] == "2" && v2[2] == "3,4,5,6,7,8,9" &&
		  s.split("3,4").size() == 2);
}

TEST(string_rsplit1)
{
	hstr s="1,2,3,4,5,6,7,8,9";
	std::vector<hstr> v=s.rsplit(",");


	CHECK(v.size() == 9 && v[0] == "1" && v[1] == "2" && v[2] == "3" && v[3] == "4" &&
	      v[4] == "5" && v[5] == "6" && v[6] == "7" && v[7] == "8" && v[8] == "9");
}

TEST(string_rsplit2)
{
	hstr s="1,2,3,4,5,6,7,8,9";
	hstr s2="test",splitter="!";
	std::vector<hstr> v2=s.rsplit(",",2);
	std::vector<hstr> v3=s2.rsplit(',');
	CHECK(v3[0] == "test" && s2.rsplit(splitter).size() == 1 &&
		  v2.size() == 3 && v2[0] == "1,2,3,4,5,6,7" && v2[1] == "8" && v2[2] == "9");
}

// run all tests
int main(int argc, char **argv)
{
	return UnitTest::RunAllTests();
}
