#define __HL_UT_LIB_NAME hltypes
#ifdef __APPLE__
#import "hunittest.h"
#else
#include "hunittest.h"
#endif

#include "harray.h"
#include "hstring.h"

HL_UT_TEST_CLASS(String)
{
	HL_UT_TEST_FUNCTION(floatingPoint)
	{
		hstr floatString("5.75");
		float f = floatString;
		floatString = 6.75f;

		HL_UT_ASSERT(f == 5.75f, "floatingPoint1");
		HL_UT_ASSERT(floatString == 6.75f, "floatingPoint2");
	}

	HL_UT_TEST_FUNCTION(integer)
	{
		hstr intString("5");
		int f = intString;
		intString = 6;
		HL_UT_ASSERT(f == 5, "integer1");
		HL_UT_ASSERT(intString == 6, "integer2");
	}

	HL_UT_TEST_FUNCTION(boolean)
	{
		hstr b;
		b = "1";
		HL_UT_ASSERT(b == true, "boolean1");
		b = "0";
		HL_UT_ASSERT(b == false, "boolean2");
		b = "true";
		HL_UT_ASSERT(b == true, "boolean3");
		b = "false";
		HL_UT_ASSERT(b == false, "boolean4");
		b = true;
		HL_UT_ASSERT(b == true, "boolean5");
		b = false;
		HL_UT_ASSERT(b == false, "boolean6");
	}

	HL_UT_TEST_FUNCTION(stdstrCompatibility)
	{
		//todo - fix this in base code

		/*std::string str1, str2 = "text2";
		hstr hs1 = "text1", hs2, hs3("text1");
		str1 = hs1;
		hs2 = str2;
		HL_UT_ASSERT(hs1 == str1, "compatibility1");
		HL_UT_ASSERT(hs1 == "text1", "compatibility2");
		HL_UT_ASSERT(hs2 == str2, "compatibility3");
		HL_UT_ASSERT(hs2 == "text2", "compatibility4");
		HL_UT_ASSERT(hs1 != hs2, "compatibility5");
		HL_UT_ASSERT(hs1 == hs3, "compatibility6");*/
	}

	HL_UT_TEST_FUNCTION(cstrCompatibility)
	{
		hstr s1, s2;
		char cstr[64] = "text2";
		s1 = "text1";
		s2 = cstr;
		HL_UT_ASSERT(s1 == "text1", "cstrCompatibility1");
		HL_UT_ASSERT(s2 == cstr, "cstrCompatibility2");
		HL_UT_ASSERT(s2 == "text2", "cstrCompatibility3");
	}

	HL_UT_TEST_FUNCTION(replace)
	{
		hstr s1 = "1 2 3 4 5 6 7 8 9", s2 = "101101010100011010", s3 = "test", s4 = "0";
		s1.replace(" ", "");		
		HL_UT_ASSERT(s1 == "123456789", "replace1");
		s2.replace(s4, "");
		HL_UT_ASSERT(s2 == "111111111", "replace2");
		s3.replace("a", "b");
		HL_UT_ASSERT(s3 == "test", "replace3");
		s3.replace("es", "his is a tes");
		HL_UT_ASSERT(s3 == "this is a test", "replace4");
	}

	HL_UT_TEST_FUNCTION(startsWith)
	{
		hstr s1 = "this is a test";
		HL_UT_ASSERT(s1.startsWith("this"), "startsWith1");
		HL_UT_ASSERT(!s1.startsWith("something"), "startsWith2");
	}

	HL_UT_TEST_FUNCTION(endsWith)
	{
		hstr s1 = "this is a test";
		HL_UT_ASSERT(s1.endsWith("test"), "endsWith1");
		HL_UT_ASSERT(!s1.endsWith("something"), "endsWith2");
	}

	HL_UT_TEST_FUNCTION(split1)
	{
		hstr s = "1,2,3,4,5,6,7,8,9", s2 = "test", splitter = "!";
		harray<hstr> ary1 = s.split(",");
		harray<hstr> ary2 = s.split(",", 2);
		HL_UT_ASSERT(ary1.size() == 9, "split1");
		HL_UT_ASSERT(ary1[0] == "1", "split2");
		HL_UT_ASSERT(ary1[1] == "2", "split3");
		HL_UT_ASSERT(ary1[2] == "3", "split4");
		HL_UT_ASSERT(ary1[3] == "4", "split5");
		HL_UT_ASSERT(ary1[4] == "5", "split6");
		HL_UT_ASSERT(ary1[5] == "6", "split7");
		HL_UT_ASSERT(ary1[6] == "7", "split8");
		HL_UT_ASSERT(ary1[7] == "8", "split9");
		HL_UT_ASSERT(ary1[8] == "9", "split10");
		HL_UT_ASSERT(s2.split(',')[0] == "test", "split11");
		HL_UT_ASSERT(s2.split(splitter).size() == 1, "split12");
		HL_UT_ASSERT(ary2.size() == 3, "split13");
		HL_UT_ASSERT(ary2[0] == "1", "split14");
		HL_UT_ASSERT(ary2[1] == "2", "split15");
		HL_UT_ASSERT(ary2[2] == "3,4,5,6,7,8,9", "split16");
		HL_UT_ASSERT(s.split("3,4").size() == 2, "split17");
	}

	HL_UT_TEST_FUNCTION(split2)
	{
		hstr s = "1,2,3";
		harray<hstr> ary = s.split(",");
		HL_UT_ASSERT(ary.size() == 3, "split1");
		HL_UT_ASSERT(ary[0] == "1", "split2");
		HL_UT_ASSERT(ary[1] == "2", "split3");
		HL_UT_ASSERT(ary[2] == "3", "split4");
		s = "1,2,";
		ary = s.split(",");
		HL_UT_ASSERT(ary.size() == 3, "split5");
		HL_UT_ASSERT(ary[0] == "1", "split6");
		HL_UT_ASSERT(ary[1] == "2", "split7");
		HL_UT_ASSERT(ary[2] == "", "split8");
		s = "1,,3";
		ary = s.split(",");
		HL_UT_ASSERT(ary.size() == 3, "split9");
		HL_UT_ASSERT(ary[0] == "1", "split10");
		HL_UT_ASSERT(ary[1] == "", "split11");
		HL_UT_ASSERT(ary[2] == "3", "split12");
	}

	HL_UT_TEST_FUNCTION(split3)
	{
		hstr s = "1,2,3";
		harray<hstr> ary = s.split("-");
		HL_UT_ASSERT(ary.size() == 1, "split1");
		HL_UT_ASSERT(ary[0] == "1,2,3", "split2");
	}

	HL_UT_TEST_FUNCTION(rsplit1)
	{
		hstr s = "1,2,3,4,5,6,7,8,9";
		harray<hstr> ary1 = s.rsplit(",");

		HL_UT_ASSERT(ary1.size() == 9, "rsplit1");
		HL_UT_ASSERT(ary1[0] == "1", "rsplit2");
		HL_UT_ASSERT(ary1[1] == "2", "rsplit3");
		HL_UT_ASSERT(ary1[2] == "3", "rsplit4");
		HL_UT_ASSERT(ary1[3] == "4", "rsplit5");
		HL_UT_ASSERT(ary1[4] == "5", "rsplit6");
		HL_UT_ASSERT(ary1[5] == "6", "rsplit7");
		HL_UT_ASSERT(ary1[6] == "7", "rsplit8");
		HL_UT_ASSERT(ary1[7] == "8", "rsplit9");
		HL_UT_ASSERT(ary1[8] == "9", "rsplit10");
	}

	HL_UT_TEST_FUNCTION(rsplit2)
	{
		hstr s = "1,2,3,4,5,6,7,8,9";
		hstr s2 = "test", splitter = "!";
		harray<hstr> ary1 = s.rsplit(",", 2);
		harray<hstr> ary2 = s2.rsplit(',');
		HL_UT_ASSERT(ary2[0] == "test", "rsplit1");
		HL_UT_ASSERT(s2.rsplit(splitter).size() == 1, "rsplit2");
		HL_UT_ASSERT(ary1.size() == 3, "rsplit3");
		HL_UT_ASSERT(ary1[0] == "1,2,3,4,5,6,7", "rsplit4");
		HL_UT_ASSERT(ary1[1] == "8", "rsplit5");
		HL_UT_ASSERT(ary1[2] == "9", "rsplit6");
	}

	HL_UT_TEST_FUNCTION(rsplit3)
	{
		hstr s = "1,2,3";
		harray<hstr> ary = s.rsplit(",");
		HL_UT_ASSERT(ary.size() == 3, "rsplit1");
		HL_UT_ASSERT(ary[0] == "1", "rsplit2");
		HL_UT_ASSERT(ary[1] == "2", "rsplit3");
		HL_UT_ASSERT(ary[2] == "3", "rsplit4");
		s = "1,2,";
		ary = s.rsplit(",");
		HL_UT_ASSERT(ary.size() == 3, "rsplit5");
		HL_UT_ASSERT(ary[0] == "1", "rsplit6");
		HL_UT_ASSERT(ary[1] == "2", "rsplit7");
		HL_UT_ASSERT(ary[2] == "", "rsplit8");
		s = "1,,3";
		ary = s.rsplit(",");
		HL_UT_ASSERT(ary.size() == 3, "rsplit9");
		HL_UT_ASSERT(ary[0] == "1", "rsplit10");
		HL_UT_ASSERT(ary[1] == "", "rsplit11");
		HL_UT_ASSERT(ary[2] == "3", "rsplit12");
	}

	HL_UT_TEST_FUNCTION(substrOperator)
	{
		hstr s = "1234567890";
		HL_UT_ASSERT(s(1, 2) == "23", "substrOperator1");
		HL_UT_ASSERT(s(6, 4) == "7890", "substrOperator2");
		HL_UT_ASSERT(s(0, 4, 2) == "13", "substrOperator3");
		HL_UT_ASSERT(s(1, 5, 2) == "246", "substrOperator4");
		HL_UT_ASSERT(s(6) == "7", "substrOperator5");
		HL_UT_ASSERT(s[3] == '4', "substrOperator6");
	}

	HL_UT_TEST_FUNCTION(count)
	{
		hstr s = "11122223";
		HL_UT_ASSERT(s.count("22") == 2, "count1");
		HL_UT_ASSERT(s.count("1") == 3, "count2");
		HL_UT_ASSERT(s.count('1') == 3, "count3");
		HL_UT_ASSERT(s.count('5') == 0, "count4");
	}

	HL_UT_TEST_FUNCTION(lowered)
	{
		hstr s1 = "thIS T3Xt w4s wRITtEn in vARiaNT cAP5";
		HL_UT_ASSERT(s1.lowered() == "this t3xt w4s written in variant cap5", "lowered1");
		hstr s2 = "";
		HL_UT_ASSERT(s2.lowered() == "", "lowered2");
	}

	HL_UT_TEST_FUNCTION(uppered)
	{
		hstr s1 = "thIS T3Xt w4s wRITtEn in vARiaNT cAP5";
		HL_UT_ASSERT(s1.uppered() == "THIS T3XT W4S WRITTEN IN VARIANT CAP5", "uppered1");
		hstr s2 = "";
		HL_UT_ASSERT(s2.uppered() == "", "uppered2");
	}

	HL_UT_TEST_FUNCTION(reverse)
	{
		hstr s = "this text will be reversed";
		HL_UT_ASSERT(s.reversed() == "desrever eb lliw txet siht", "reverse");
	}

	HL_UT_TEST_FUNCTION(isNumeric)
	{
		hstr text1 = "7";
		HL_UT_ASSERT(text1.isDigit(), "isNumeric1");
		HL_UT_ASSERT(text1.isInt(), "isNumeric2");
		HL_UT_ASSERT(!text1.isFloat(), "isNumeric3");
		HL_UT_ASSERT(text1.isNumber(), "isNumeric4");
		HL_UT_ASSERT(text1.isHex(), "isNumeric5");
		hstr text2 = "1234567890";
		HL_UT_ASSERT(!text2.isDigit(), "isNumeric6");
		HL_UT_ASSERT(text2.isInt(), "isNumeric7");
		HL_UT_ASSERT(!text2.isFloat(), "isNumeric8");
		HL_UT_ASSERT(text2.isNumber(), "isNumeric9");
		HL_UT_ASSERT(text2.isHex(), "isNumeric10");
		hstr text3 = "12345h67890";
		HL_UT_ASSERT(!text3.isDigit(), "isNumeric11");
		HL_UT_ASSERT(!text3.isInt(), "isNumeric12");
		HL_UT_ASSERT(!text3.isFloat(), "isNumeric13");
		HL_UT_ASSERT(!text3.isNumber(), "isNumeric14");
		HL_UT_ASSERT(!text3.isHex(), "isNumeric15");
		hstr text4 = "1.234";
		HL_UT_ASSERT(!text4.isDigit(), "isNumeric16");
		HL_UT_ASSERT(!text4.isInt(), "isNumeric17");
		HL_UT_ASSERT(text4.isFloat(), "isNumeric18");
		HL_UT_ASSERT(text4.isNumber(), "isNumeric19");
		HL_UT_ASSERT(!text4.isHex(), "isNumeric20");
		hstr text5 = "1.234.5";
		HL_UT_ASSERT(!text5.isDigit(), "isNumeric21");
		HL_UT_ASSERT(!text5.isInt(), "isNumeric22");
		HL_UT_ASSERT(!text5.isFloat(), "isNumeric23");
		HL_UT_ASSERT(!text5.isNumber(), "isNumeric24");
		HL_UT_ASSERT(!text5.isHex(), "isNumeric25");
		hstr text6 = "CAFEbabe1234567890";
		HL_UT_ASSERT(!text6.isDigit(), "isNumeric26");
		HL_UT_ASSERT(!text6.isInt(), "isNumeric27");
		HL_UT_ASSERT(!text6.isFloat(), "isNumeric28");
		HL_UT_ASSERT(!text6.isNumber(), "isNumeric29");
		HL_UT_ASSERT(text6.isHex(), "isNumeric30");
	}

	HL_UT_TEST_FUNCTION(trim)
	{
		hstr s1 = "123 456 789 0";
		HL_UT_ASSERT(s1.trimmedRight() == "123 456 789 0", "trim1");
		HL_UT_ASSERT(s1.trimmedLeft() == "123 456 789 0", "trim2");
		HL_UT_ASSERT(s1.trimmed() == "123 456 789 0", "trim3");
		HL_UT_ASSERT(s1.trimmed('0') == "123 456 789 ", "trim4");
		hstr s2 = "   123 456 789 0";
		HL_UT_ASSERT(s2.trimmedRight() == "   123 456 789 0", "trim5");
		HL_UT_ASSERT(s2.trimmedLeft() == "123 456 789 0", "trim6");
		HL_UT_ASSERT(s2.trimmed() == "123 456 789 0", "trim7");
		HL_UT_ASSERT(s2.trimmed('0') == "   123 456 789 ", "trim8");
		hstr s3 = "123 456 789 0   ";
		HL_UT_ASSERT(s3.trimmedRight() == "123 456 789 0", "trim9");
		HL_UT_ASSERT(s3.trimmedLeft() == "123 456 789 0   ", "trim10");
		HL_UT_ASSERT(s3.trimmed() == "123 456 789 0", "trim11");
		HL_UT_ASSERT(s3.trimmed('0') == "123 456 789 0   ", "trim12");
		hstr s4 = "   123 456 789 0   ";
		HL_UT_ASSERT(s4.trimmedRight() == "   123 456 789 0", "trim13");
		HL_UT_ASSERT(s4.trimmedLeft() == "123 456 789 0   ", "trim14");
		HL_UT_ASSERT(s4.trimmed() == "123 456 789 0", "trim15");
		HL_UT_ASSERT(s4.trimmed('0') == "   123 456 789 0   ", "trim16");
	}

	HL_UT_TEST_FUNCTION(formatting)
	{
		hstr text = hsprintf("This is a %d %s %4.2f %s.", 15, "formatted", 3.14f, "text");
		HL_UT_ASSERT(text == "This is a 15 formatted 3.14 text.", "hsprintf");
	}
}