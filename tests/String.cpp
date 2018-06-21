#define __HTEST_LIB_NAME hltypes
#include "htest.h"
#include "harray.h"
#include "hstring.h"

HTEST_CLASS(String)
{
	HTEST_FUNCTION(floatingPoint)
	{
		hstr floatString("5.75");
		float f = floatString;
		floatString = 6.75f;
		HTEST_ASSERT(f == 5.75f, "floatingPoint1");
		HTEST_ASSERT(floatString == 6.75f, "floatingPoint2");
	}

	HTEST_FUNCTION(integer)
	{
		hstr intString("5");
		int f = intString;
		intString = 6;
		HTEST_ASSERT(f == 5, "integer1");
		HTEST_ASSERT(intString == 6, "integer2");
	}

	HTEST_FUNCTION(boolean)
	{
		hstr b;
		b = "1";
		HTEST_ASSERT(b == true, "boolean1");
		b = "0";
		HTEST_ASSERT(b == false, "boolean2");
		b = "true";
		HTEST_ASSERT(b == true, "boolean3");
		b = "false";
		HTEST_ASSERT(b == false, "boolean4");
		b = true;
		HTEST_ASSERT(b == true, "boolean5");
		b = false;
		HTEST_ASSERT(b == false, "boolean6");
	}

	HTEST_FUNCTION(cstrCompatibility)
	{
		hstr s1;
		hstr s2;
		char cstr[64] = "text2";
		s1 = "text1";
		s2 = cstr;
		HTEST_ASSERT(s1 == "text1", "cstrCompatibility1");
		HTEST_ASSERT(s2 == cstr, "cstrCompatibility2");
		HTEST_ASSERT(s2 == "text2", "cstrCompatibility3");
	}

	HTEST_FUNCTION(lowered)
	{
		hstr s1 = "thIS T3Xt w4s wRITtEn in vARiaNT cAP5";
		HTEST_ASSERT(s1.lowered() == "this t3xt w4s written in variant cap5", "lowered1");
		hstr s2 = "";
		HTEST_ASSERT(s2.lowered() == "", "lowered2");
	}

	HTEST_FUNCTION(uppered)
	{
		hstr s1 = "thIS T3Xt w4s wRITtEn in vARiaNT cAP5";
		HTEST_ASSERT(s1.uppered() == "THIS T3XT W4S WRITTEN IN VARIANT CAP5", "uppered1");
		hstr s2 = "";
		HTEST_ASSERT(s2.uppered() == "", "uppered2");
	}

	HTEST_FUNCTION(reverse)
	{
		hstr s = "this text will be reversed";
		HTEST_ASSERT(s.reversed() == "desrever eb lliw txet siht", "reverse");
	}

	HTEST_FUNCTION(trim)
	{
		hstr s1 = "123 456 789 0";
		HTEST_ASSERT(s1.trimmedRight() == "123 456 789 0", "trim1");
		HTEST_ASSERT(s1.trimmedLeft() == "123 456 789 0", "trim2");
		HTEST_ASSERT(s1.trimmed() == "123 456 789 0", "trim3");
		HTEST_ASSERT(s1.trimmed('0') == "123 456 789 ", "trim4");
		hstr s2 = "   123 456 789 0";
		HTEST_ASSERT(s2.trimmedRight() == "   123 456 789 0", "trim5");
		HTEST_ASSERT(s2.trimmedLeft() == "123 456 789 0", "trim6");
		HTEST_ASSERT(s2.trimmed() == "123 456 789 0", "trim7");
		HTEST_ASSERT(s2.trimmed('0') == "   123 456 789 ", "trim8");
		hstr s3 = "123 456 789 0   ";
		HTEST_ASSERT(s3.trimmedRight() == "123 456 789 0", "trim9");
		HTEST_ASSERT(s3.trimmedLeft() == "123 456 789 0   ", "trim10");
		HTEST_ASSERT(s3.trimmed() == "123 456 789 0", "trim11");
		HTEST_ASSERT(s3.trimmed('0') == "123 456 789 0   ", "trim12");
		hstr s4 = "   123 456 789 0   ";
		HTEST_ASSERT(s4.trimmedRight() == "   123 456 789 0", "trim13");
		HTEST_ASSERT(s4.trimmedLeft() == "123 456 789 0   ", "trim14");
		HTEST_ASSERT(s4.trimmed() == "123 456 789 0", "trim15");
		HTEST_ASSERT(s4.trimmed('0') == "   123 456 789 0   ", "trim16");
	}

	HTEST_FUNCTION(replace)
	{
		hstr s1 = "1 2 3 4 5 6 7 8 9";
		hstr s2 = "101101010100011010";
		hstr s3 = "test";
		hstr s4 = "0";
		s1.replace(" ", "");
		HTEST_ASSERT(s1 == "123456789", "replace1");
		s2.replace(s4, "");
		HTEST_ASSERT(s2 == "111111111", "replace2");
		s3.replace("a", "b");
		HTEST_ASSERT(s3 == "test", "replace3");
		s3.replace("es", "his is a tes");
		HTEST_ASSERT(s3 == "this is a test", "replace4");
	}

	HTEST_FUNCTION(insertAt)
	{
		hstr s1 = "123456789";
		hstr s2 = "111222";
		hstr s3 = "test";
		hstr s4 = "0";
		s1.insertAt(1, "");		
		HTEST_ASSERT(s1 == "123456789", "insertAt1");
		s2.insertAt(2, '3');
		HTEST_ASSERT(s2 == "1131222", "insertAt2");
		s3.insertAt(0, "ba");
		HTEST_ASSERT(s3 == "batest", "insertAt3");
		s4.insertAt(1, '3', 3);
		HTEST_ASSERT(s4 == "0333", "insertAt4");
		s4.insertAt(4, "1");
		HTEST_ASSERT(s4 == "03331", "insertAt5");
		s4.insertAt(10, "123");
		HTEST_ASSERT(s4 == "03331", "insertAt6");
	}

	HTEST_FUNCTION(split1)
	{
		hstr s = "1,2,3,4,5,6,7,8,9";
		hstr s2 = "test";
		hstr delimiter1 = "!";
		char delimiter2 = '!';
		harray<hstr> ary1 = s.split(",");
		harray<hstr> ary2 = s.split(",", 2);
		harray<hstr> ary3 = s.split(',', 2);
		harray<hstr> ary4 = s2.split("");
		harray<hstr> ary5 = s2.split("", 2);
		harray<hstr> ary6 = s2.split("", 3);
		HTEST_ASSERT(ary1.size() == 9, "split1");
		HTEST_ASSERT(ary1[0] == "1", "split2");
		HTEST_ASSERT(ary1[1] == "2", "split3");
		HTEST_ASSERT(ary1[2] == "3", "split4");
		HTEST_ASSERT(ary1[3] == "4", "split5");
		HTEST_ASSERT(ary1[4] == "5", "split6");
		HTEST_ASSERT(ary1[5] == "6", "split7");
		HTEST_ASSERT(ary1[6] == "7", "split8");
		HTEST_ASSERT(ary1[7] == "8", "split9");
		HTEST_ASSERT(ary1[8] == "9", "split10");
		HTEST_ASSERT(s2.split(',')[0] == "test", "split11");
		HTEST_ASSERT(s2.split(delimiter1).size() == 1, "split12");
		HTEST_ASSERT(s2.split(delimiter2).size() == 1, "split12");
		HTEST_ASSERT(ary2.size() == 3, "split13");
		HTEST_ASSERT(ary2[0] == "1", "split14");
		HTEST_ASSERT(ary2[1] == "2", "split15");
		HTEST_ASSERT(ary2[2] == "3,4,5,6,7,8,9", "split16");
		HTEST_ASSERT(ary3.size() == 3, "split17");
		HTEST_ASSERT(ary3[0] == "1", "split18");
		HTEST_ASSERT(ary3[1] == "2", "split19");
		HTEST_ASSERT(ary3[2] == "3,4,5,6,7,8,9", "split20");
		HTEST_ASSERT(s.split("3,4").size() == 2, "split21");
		HTEST_ASSERT(ary4.size() == 4, "split22");
		HTEST_ASSERT(ary4[0] == "t", "split23");
		HTEST_ASSERT(ary4[1] == "e", "split24");
		HTEST_ASSERT(ary4[2] == "s", "split25");
		HTEST_ASSERT(ary4[3] == "t", "split26");
		HTEST_ASSERT(ary5.size() == 3, "split27");
		HTEST_ASSERT(ary5[0] == "t", "split28");
		HTEST_ASSERT(ary5[1] == "e", "split29");
		HTEST_ASSERT(ary5[2] == "st", "split30");
		HTEST_ASSERT(ary6.size() == 4, "split31");
		HTEST_ASSERT(ary6[0] == "t", "split32");
		HTEST_ASSERT(ary6[1] == "e", "split33");
		HTEST_ASSERT(ary6[2] == "s", "split34");
		HTEST_ASSERT(ary6[3] == "t", "split35");
	}

	HTEST_FUNCTION(split2)
	{
		hstr s = "1,2,3";
		harray<hstr> ary = s.split(",");
		HTEST_ASSERT(ary.size() == 3, "split1");
		HTEST_ASSERT(ary[0] == "1", "split2");
		HTEST_ASSERT(ary[1] == "2", "split3");
		HTEST_ASSERT(ary[2] == "3", "split4");
		s = "1,2,";
		ary = s.split(",");
		HTEST_ASSERT(ary.size() == 3, "split5");
		HTEST_ASSERT(ary[0] == "1", "split6");
		HTEST_ASSERT(ary[1] == "2", "split7");
		HTEST_ASSERT(ary[2] == "", "split8");
		s = "1,,3";
		ary = s.split(",");
		HTEST_ASSERT(ary.size() == 3, "split9");
		HTEST_ASSERT(ary[0] == "1", "split10");
		HTEST_ASSERT(ary[1] == "", "split11");
		HTEST_ASSERT(ary[2] == "3", "split12");
	}

	HTEST_FUNCTION(split3)
	{
		hstr s = "1,2,3";
		harray<hstr> ary = s.split("-");
		HTEST_ASSERT(ary.size() == 1, "split1");
		HTEST_ASSERT(ary[0] == "1,2,3", "split2");
	}

	HTEST_FUNCTION(split4)
	{
		bool result = false;
		hstr s = "1,,2";
		hstr left;
		hstr right;
		result = s.split(',', left, right);
		HTEST_ASSERT(result, "split1");
		HTEST_ASSERT(left == "1", "split2");
		HTEST_ASSERT(right == ",2", "split3");
		result = s.split(",,", left, right);
		HTEST_ASSERT(result, "split4");
		HTEST_ASSERT(left == "1", "split5");
		HTEST_ASSERT(right == "2", "split6");
		result = s.split(",,,", left, right);
		HTEST_ASSERT(!result, "split7");
	}

	HTEST_FUNCTION(rsplit1)
	{
		hstr s = "1,2,3,4,5,6,7,8,9";
		harray<hstr> ary1 = s.rsplit(",");
		HTEST_ASSERT(ary1.size() == 9, "rsplit1");
		HTEST_ASSERT(ary1[0] == "1", "rsplit2");
		HTEST_ASSERT(ary1[1] == "2", "rsplit3");
		HTEST_ASSERT(ary1[2] == "3", "rsplit4");
		HTEST_ASSERT(ary1[3] == "4", "rsplit5");
		HTEST_ASSERT(ary1[4] == "5", "rsplit6");
		HTEST_ASSERT(ary1[5] == "6", "rsplit7");
		HTEST_ASSERT(ary1[6] == "7", "rsplit8");
		HTEST_ASSERT(ary1[7] == "8", "rsplit9");
		HTEST_ASSERT(ary1[8] == "9", "rsplit10");
	}

	HTEST_FUNCTION(rsplit2)
	{
		hstr s = "1,2,3,4,5,6,7,8,9";
		hstr s2 = "test";
		hstr splitter = "!";
		harray<hstr> ary1 = s.rsplit(",", 2);
		harray<hstr> ary2 = s2.rsplit(',');
		harray<hstr> ary3 = s2.rsplit("");
		harray<hstr> ary4 = s2.rsplit("", 2);
		harray<hstr> ary5 = s2.rsplit("", 3);
		HTEST_ASSERT(ary2[0] == "test", "rsplit1");
		HTEST_ASSERT(s2.rsplit(splitter).size() == 1, "rsplit2");
		HTEST_ASSERT(ary1.size() == 3, "rsplit3");
		HTEST_ASSERT(ary1[0] == "1,2,3,4,5,6,7", "rsplit4");
		HTEST_ASSERT(ary1[1] == "8", "rsplit5");
		HTEST_ASSERT(ary1[2] == "9", "rsplit6");
		HTEST_ASSERT(ary3.size() == 4, "rsplit7");
		HTEST_ASSERT(ary3[0] == "t", "rsplit8");
		HTEST_ASSERT(ary3[1] == "e", "rsplit9");
		HTEST_ASSERT(ary3[2] == "s", "rsplit10");
		HTEST_ASSERT(ary3[3] == "t", "rslpit11");
		HTEST_ASSERT(ary4.size() == 3, "rsplit12");
		HTEST_ASSERT(ary4[0] == "te", "rsplit13");
		HTEST_ASSERT(ary4[1] == "s", "rsplit14");
		HTEST_ASSERT(ary4[2] == "t", "rsplit15");
		HTEST_ASSERT(ary5.size() == 4, "rsplit16");
		HTEST_ASSERT(ary5[0] == "t", "rsplit17");
		HTEST_ASSERT(ary5[1] == "e", "rsplit18");
		HTEST_ASSERT(ary5[2] == "s", "rsplit19");
		HTEST_ASSERT(ary5[3] == "t", "rsplit20");
	}

	HTEST_FUNCTION(rsplit3)
	{
		hstr s = "1,2,3";
		harray<hstr> ary = s.rsplit(",");
		HTEST_ASSERT(ary.size() == 3, "rsplit1");
		HTEST_ASSERT(ary[0] == "1", "rsplit2");
		HTEST_ASSERT(ary[1] == "2", "rsplit3");
		HTEST_ASSERT(ary[2] == "3", "rsplit4");
		s = "1,2,";
		ary = s.rsplit(",");
		HTEST_ASSERT(ary.size() == 3, "rsplit5");
		HTEST_ASSERT(ary[0] == "1", "rsplit6");
		HTEST_ASSERT(ary[1] == "2", "rsplit7");
		HTEST_ASSERT(ary[2] == "", "rsplit8");
		s = "1,,3";
		ary = s.rsplit(",");
		HTEST_ASSERT(ary.size() == 3, "rsplit9");
		HTEST_ASSERT(ary[0] == "1", "rsplit10");
		HTEST_ASSERT(ary[1] == "", "rsplit11");
		HTEST_ASSERT(ary[2] == "3", "rsplit12");
	}

	HTEST_FUNCTION(rsplit4)
	{
		bool result = false;
		hstr s = "1,,2";
		hstr left;
		hstr right;
		result = s.rsplit(',', left, right);
		HTEST_ASSERT(result, "rsplit1");
		HTEST_ASSERT(left == "1,", "rsplit2");
		HTEST_ASSERT(right == "2", "rsplit3");
		result = s.rsplit(",,", left, right);
		HTEST_ASSERT(result, "rsplit4");
		HTEST_ASSERT(left == "1", "rsplit5");
		HTEST_ASSERT(right == "2", "rsplit6");
		result = s.rsplit(",,,", left, right);
		HTEST_ASSERT(!result, "rsplit7");
	}

	HTEST_FUNCTION(indexOf)
	{
		hstr s = "1,2,3,4,5";
		HTEST_ASSERT(s.indexOf(',') == 1, "indexOf1");
		HTEST_ASSERT(s.indexOf('3') == 4, "indexOf2");
		HTEST_ASSERT(s.indexOf(' ') == -1, "indexOf3");
		HTEST_ASSERT(s.indexOf("2,3") == 2, "indexOf4");
		HTEST_ASSERT(s.indexOf("2,3,") == 2, "indexOf5");
		HTEST_ASSERT(s.indexOf(",5,") == -1, "indexOf6");
		HTEST_ASSERT(s.indexOf("  ") == -1, "indexOf7");
	}

	HTEST_FUNCTION(rindexOf)
	{
		hstr s = "1,2,3,4,5";
		HTEST_ASSERT(s.rindexOf(',') == 7, "rindexOf1");
		HTEST_ASSERT(s.rindexOf('3') == 4, "rindexOf2");
		HTEST_ASSERT(s.rindexOf(' ') == -1, "rindexOf3");
		HTEST_ASSERT(s.rindexOf("2,3") == 2, "rindexOf4");
		HTEST_ASSERT(s.rindexOf("2,3,") == 2, "rindexOf5");
		HTEST_ASSERT(s.rindexOf(",5,") == -1, "rindexOf6");
		HTEST_ASSERT(s.rindexOf("  ") == -1, "rindexOf7");
	}

	HTEST_FUNCTION(indexOfAny)
	{
		hstr s = "1,2,3,4,5";
		HTEST_ASSERT(s.indexOfAny(",") == 1, "indexOfAny1");
		HTEST_ASSERT(s.indexOfAny("2,") == 1, "indexOfAny2");
	}

	HTEST_FUNCTION(rindexOfAny)
	{
		hstr s = "1,2,3,4,5";
		HTEST_ASSERT(s.rindexOfAny("3") == 4, "rindexOfAny1");
		HTEST_ASSERT(s.rindexOfAny("2,") == 7, "rindexOfAny2");
	}

	HTEST_FUNCTION(count)
	{
		hstr s = "11122223";
		HTEST_ASSERT(s.count("22") == 2, "count1");
		HTEST_ASSERT(s.count("1") == 3, "count2");
		HTEST_ASSERT(s.count('1') == 3, "count3");
		HTEST_ASSERT(s.count('5') == 0, "count4");
	}

	HTEST_FUNCTION(startsWith)
	{
		hstr s1 = "this is a test";
		HTEST_ASSERT(s1.startsWith("this"), "startsWith1");
		HTEST_ASSERT(!s1.startsWith("something"), "startsWith2");
	}

	HTEST_FUNCTION(endsWith)
	{
		hstr s1 = "this is a test";
		HTEST_ASSERT(s1.endsWith("test"), "endsWith1");
		HTEST_ASSERT(!s1.endsWith("something"), "endsWith2");
	}

	HTEST_FUNCTION(contains)
	{
		hstr s1 = "this is a test";
		HTEST_ASSERT(s1.contains("test"), "contains1");
		HTEST_ASSERT(!s1.contains("notest"), "contains2");
		HTEST_ASSERT(s1.contains('t'), "contains3");
		HTEST_ASSERT(!s1.contains('x'), "contains4");
		HTEST_ASSERT(s1.containsAny("asdfghjkl"), "contains5");
		HTEST_ASSERT(!s1.containsAny("dfgjkl"), "contains6");
		HTEST_ASSERT(s1.containsAll("tisae"), "contains7");
		HTEST_ASSERT(!s1.containsAll("dfgjkl"), "contains8");
	}

	HTEST_FUNCTION(isNumeric)
	{
		hstr text1 = "7";
		HTEST_ASSERT(text1.isDigit(), "isNumeric1");
		HTEST_ASSERT(text1.isInt(), "isNumeric2");
		HTEST_ASSERT(!text1.isFloat(), "isNumeric3");
		HTEST_ASSERT(text1.isNumber(), "isNumeric4");
		HTEST_ASSERT(text1.isHex(), "isNumeric5");
		hstr text2 = "1234567890";
		HTEST_ASSERT(!text2.isDigit(), "isNumeric6");
		HTEST_ASSERT(text2.isInt(), "isNumeric7");
		HTEST_ASSERT(!text2.isFloat(), "isNumeric8");
		HTEST_ASSERT(text2.isNumber(), "isNumeric9");
		HTEST_ASSERT(text2.isHex(), "isNumeric10");
		hstr text3 = "12345h67890";
		HTEST_ASSERT(!text3.isDigit(), "isNumeric11");
		HTEST_ASSERT(!text3.isInt(), "isNumeric12");
		HTEST_ASSERT(!text3.isFloat(), "isNumeric13");
		HTEST_ASSERT(!text3.isNumber(), "isNumeric14");
		HTEST_ASSERT(!text3.isHex(), "isNumeric15");
		hstr text4 = "1.234";
		HTEST_ASSERT(!text4.isDigit(), "isNumeric16");
		HTEST_ASSERT(!text4.isInt(), "isNumeric17");
		HTEST_ASSERT(text4.isFloat(), "isNumeric18");
		HTEST_ASSERT(text4.isNumber(), "isNumeric19");
		HTEST_ASSERT(!text4.isHex(), "isNumeric20");
		hstr text5 = "1.234.5";
		HTEST_ASSERT(!text5.isDigit(), "isNumeric21");
		HTEST_ASSERT(!text5.isInt(), "isNumeric22");
		HTEST_ASSERT(!text5.isFloat(), "isNumeric23");
		HTEST_ASSERT(!text5.isNumber(), "isNumeric24");
		HTEST_ASSERT(!text5.isHex(), "isNumeric25");
		hstr text6 = "CAFEbabe1234567890";
		HTEST_ASSERT(!text6.isDigit(), "isNumeric26");
		HTEST_ASSERT(!text6.isInt(), "isNumeric27");
		HTEST_ASSERT(!text6.isFloat(), "isNumeric28");
		HTEST_ASSERT(!text6.isNumber(), "isNumeric29");
		HTEST_ASSERT(text6.isHex(), "isNumeric30");
	}

	HTEST_FUNCTION(substrOperator)
	{
		hstr s = "1234567890";
		HTEST_ASSERT(s(1, 2) == "23", "substrOperator1");
		HTEST_ASSERT(s(6, 4) == "7890", "substrOperator2");
		HTEST_ASSERT(s(0, 4, 2) == "13", "substrOperator3");
		HTEST_ASSERT(s(1, 5, 2) == "246", "substrOperator4");
		HTEST_ASSERT(s(6) == "7", "substrOperator5");
		HTEST_ASSERT(s[3] == '4', "substrOperator6");
		HTEST_ASSERT(s(4, 0) == "", "substrOperator7");
	}

	HTEST_FUNCTION(hex)
	{
		hstr s1 = "this is a test";
		hstr s2 = "74657374";
		HTEST_ASSERT(!s1.isHex(), "hex1");
		HTEST_ASSERT(s1.toHex().isHex(), "hex2");
		HTEST_ASSERT(s1.toHex() == "7468697320697320612074657374", "hex3");
		HTEST_ASSERT(s2.isHex(), "hex4");
		HTEST_ASSERT(s2.unhex() == 0x74657374, "hex5");
	}

	HTEST_FUNCTION(formatting)
	{
		hstr text = hsprintf("This is a %d %s %4.2f %s.", 15, "formatted", 3.14f, "text");
		HTEST_ASSERT(text == "This is a 15 formatted 3.14 text.", "hsprintf");
	}

}
