/// @file
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

TEST(String_float)
{
	hstr floatString("5.75");
	float f = floatString;
	floatString = 6.75f;
	
	CHECK(f == 5.75f);
	CHECK(floatString == 6.75f);
}

TEST(String_int)
{
	hstr intString("5");
	int f = intString;
	intString = 6;
	CHECK(f == 5);
	CHECK(intString == 6);
}

TEST(String_bool)
{
	hstr b;
	b = "1";
	CHECK(b == true);
	b = "0";
	CHECK(b == false);
	b = "true";
	CHECK(b == true);
	b = "false";
	CHECK(b == false);
	b = true;
	CHECK(b == true);
	b = false;
	CHECK(b == false);
}

TEST(String_stdstr_compatibility)
{
	std::string str1, str2 = "text2";
	hstr hs1 = "text1", hs2, hs3("text1");
	str1 = hs1;
	hs2 = str2;
	CHECK(hs1 == str1);
	CHECK(hs1 == "text1");
	CHECK(hs2 == str2);
	CHECK(hs2 == "text2");
	CHECK(hs1 != hs2);
	CHECK(hs1 == hs3);
}

TEST(String_cstr_compatibilty)
{
	hstr s1, s2;
	char cstr[64] = "text2";
	s1 = "text1";
	s2 = cstr;
	CHECK(s1 == "text1");
	CHECK(s2 == cstr);
	CHECK(s2 == "text2");
}

TEST(String_replace)
{
	hstr s1 = "1 2 3 4 5 6 7 8 9", s2 = "101101010100011010", s3 = "test", s4 = "0";
	CHECK(s1.replace(" ", "") == "123456789");
	CHECK(s2.replace(s4, "") == "111111111");
	CHECK(s3.replace("a", "b") == "test");
	CHECK(s3.replace("es", "his is a tes") == "this is a test");
}

TEST(String_starts_with)
{
	hstr s1 = "this is a test";
	CHECK(s1.starts_with("this"));
	CHECK(!s1.starts_with("something"));
}

TEST(String_ends_with)
{
	hstr s1 = "this is a test";
	CHECK(s1.ends_with("test"));
	CHECK(!s1.ends_with("something"));
}

TEST(String_split1)
{
	hstr s = "1,2,3,4,5,6,7,8,9", s2 = "test", splitter = "!";
	harray<hstr> ary1 = s.split(",");
	harray<hstr> ary2 = s.split(",", 2);
	CHECK(ary1.size() == 9);
	CHECK(ary1[0] == "1");
	CHECK(ary1[1] == "2");
	CHECK(ary1[2] == "3");
	CHECK(ary1[3] == "4");
	CHECK(ary1[4] == "5");
	CHECK(ary1[5] == "6");
	CHECK(ary1[6] == "7");
	CHECK(ary1[7] == "8");
	CHECK(ary1[8] == "9");
	CHECK(s2.split(',')[0] == "test");
	CHECK(s2.split(splitter).size() == 1);
	CHECK(ary2.size() == 3);
	CHECK(ary2[0] == "1");
	CHECK(ary2[1] == "2");
	CHECK(ary2[2] == "3,4,5,6,7,8,9");
	CHECK(s.split("3,4").size() == 2);
}

TEST(String_split2)
{
	hstr s = "1,2,3";
	harray<hstr> ary = s.split(",");
	CHECK(ary.size() == 3);
	CHECK(ary[0] == "1");
	CHECK(ary[1] == "2");
	CHECK(ary[2] == "3");
	s = "1,2,";
	ary = s.split(",");
	CHECK(ary.size() == 3);
	CHECK(ary[0] == "1");
	CHECK(ary[1] == "2");
	CHECK(ary[2] == "");
	s = "1,,3";
	ary = s.split(",");
	CHECK(ary.size() == 3);
	CHECK(ary[0] == "1");
	CHECK(ary[1] == "");
	CHECK(ary[2] == "3");
}

TEST(String_split3)
{
	hstr s = "1,2,3";
	harray<hstr> ary = s.split("-");
	CHECK(ary.size() == 1);
	CHECK(ary[0] == "1,2,3");
}

TEST(String_rsplit1)
{
	hstr s = "1,2,3,4,5,6,7,8,9";
	harray<hstr> ary1 = s.rsplit(",");

	CHECK(ary1.size() == 9);
	CHECK(ary1[0] == "1");
	CHECK(ary1[1] == "2");
	CHECK(ary1[2] == "3");
	CHECK(ary1[3] == "4");
	CHECK(ary1[4] == "5");
	CHECK(ary1[5] == "6");
	CHECK(ary1[6] == "7");
	CHECK(ary1[7] == "8");
	CHECK(ary1[8] == "9");
}

TEST(String_rsplit2)
{
	hstr s = "1,2,3,4,5,6,7,8,9";
	hstr s2 = "test", splitter="!";
	harray<hstr> ary1 = s.rsplit(",", 2);
	harray<hstr> ary2 = s2.rsplit(',');
	CHECK(ary2[0] == "test");
	CHECK(s2.rsplit(splitter).size() == 1);
	CHECK(ary1.size() == 3);
	CHECK(ary1[0] == "1,2,3,4,5,6,7");
	CHECK(ary1[1] == "8");
	CHECK(ary1[2] == "9");
}

TEST(String_rsplit3)
{
	hstr s = "1,2,3";
	harray<hstr> ary = s.rsplit(",");
	CHECK(ary.size() == 3);
	CHECK(ary[0] == "1");
	CHECK(ary[1] == "2");
	CHECK(ary[2] == "3");
	s = "1,2,";
	ary = s.rsplit(",");
	CHECK(ary.size() == 3);
	CHECK(ary[0] == "1");
	CHECK(ary[1] == "2");
	CHECK(ary[2] == "");
	s = "1,,3";
	ary = s.rsplit(",");
	CHECK(ary.size() == 3);
	CHECK(ary[0] == "1");
	CHECK(ary[1] == "");
	CHECK(ary[2] == "3");
}

TEST(String_substr_operator)
{
	hstr s = "1234567890";
	CHECK(s(1, 2) == "23");
	CHECK(s(6, 4) == "7890");
	CHECK(s(0, 4, 2) == "13");
	CHECK(s(1, 5, 2) == "246");
	CHECK(s(6) == "7");
	CHECK(s[3] == '4');
}

TEST(String_count)
{
	hstr s = "11122223";
	CHECK(s.count("22") == 2);
	CHECK(s.count("1") == 3);
	CHECK(s.count('1') == 3);
	CHECK(s.count('5') == 0);
}

TEST(String_caps)
{
	hstr s = "thIS T3Xt w4s wRITtEn in vARiaNT cAP5";
	CHECK(s.lower() == "this t3xt w4s written in variant cap5");
	CHECK(s.upper() == "THIS T3XT W4S WRITTEN IN VARIANT CAP5");
}

TEST(String_reverse)
{
	hstr s = "this text will be reversed";
	CHECK(s.reverse() == "desrever eb lliw txet siht");
}

TEST(String_is_numeric)
{
	hstr text1 = "7";
	CHECK(text1.is_digit());
	CHECK(text1.is_int());
	CHECK(!text1.is_float());
	CHECK(text1.is_number());
	CHECK(text1.is_hex());
	hstr text2 = "1234567890";
	CHECK(!text2.is_digit());
	CHECK(text2.is_int());
	CHECK(!text2.is_float());
	CHECK(text2.is_number());
	CHECK(text2.is_hex());
	hstr text3 = "12345h67890";
	CHECK(!text3.is_digit());
	CHECK(!text3.is_int());
	CHECK(!text3.is_float());
	CHECK(!text3.is_number());
	CHECK(!text3.is_hex());
	hstr text4 = "1.234";
	CHECK(!text4.is_digit());
	CHECK(!text4.is_int());
	CHECK(text4.is_float());
	CHECK(text4.is_number());
	CHECK(!text4.is_hex());
	hstr text5 = "1.234.5";
	CHECK(!text5.is_digit());
	CHECK(!text5.is_int());
	CHECK(!text5.is_float());
	CHECK(!text5.is_number());
	CHECK(!text5.is_hex());
	hstr text6 = "CAFEbabe1234567890";
	CHECK(!text6.is_digit());
	CHECK(!text6.is_int());
	CHECK(!text6.is_float());
	CHECK(!text6.is_number());
	CHECK(text6.is_hex());
}

TEST(String_trim)
{
	hstr s1 = "123 456 789 0";
	CHECK(s1.rtrim() == "123 456 789 0");
	CHECK(s1.ltrim() == "123 456 789 0");
	CHECK(s1.trim() == "123 456 789 0");
	CHECK(s1.trim('0') == "123 456 789 ");
	hstr s2 = "   123 456 789 0";
	CHECK(s2.rtrim() == "   123 456 789 0");
	CHECK(s2.ltrim() == "123 456 789 0");
	CHECK(s2.trim() == "123 456 789 0");
	CHECK(s2.trim('0') == "   123 456 789 ");
	hstr s3 = "123 456 789 0   ";
	CHECK(s3.rtrim() == "123 456 789 0");
	CHECK(s3.ltrim() == "123 456 789 0   ");
	CHECK(s3.trim() == "123 456 789 0");
	CHECK(s3.trim('0') == "123 456 789 0   ");
	hstr s4 = "   123 456 789 0   ";
	CHECK(s4.rtrim() == "   123 456 789 0");
	CHECK(s4.ltrim() == "123 456 789 0   ");
	CHECK(s4.trim() == "123 456 789 0");
	CHECK(s4.trim('0') == "   123 456 789 0   ");
}

TEST(String_hsprintf)
{
	hstr text = hsprintf("This is a %d %s %4.2f %s.", 15, "formatted", 3.14f, "text");
	CHECK(text == "This is a 15 formatted 3.14 text.");
}

