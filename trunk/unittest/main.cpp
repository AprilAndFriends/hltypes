#include <UnitTest++.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To add a test, simply put the following code in the a .cpp file of your choice:
//
// =================================
// Simple Test
// =================================
//
//  TEST(YourTestName)
//  {
//  }
//
// The TEST macro contains enough machinery to turn this slightly odd-looking syntax into legal C++, and automatically register the test in a global list. 
// This test list forms the basis of what is executed by RunAllTests().
//
// If you want to re-use a set of test data for more than one test, or provide setup/teardown for tests, 
// you can use the TEST_FIXTURE macro instead. The macro requires that you pass it a class name that it will instantiate, so any setup and teardown code should be in its constructor and destructor.
//
//  struct SomeFixture
//  {
//    SomeFixture() { /* some setup */ }
//    ~SomeFixture() { /* some teardown */ }
//
//    int testData;
//  };
// 
//  TEST_FIXTURE(SomeFixture, YourTestName)
//  {
//    int temp = testData;
//  }
//
// =================================
// Test Suites
// =================================
// 
// Tests can be grouped into suites, using the SUITE macro. A suite serves as a namespace for test names, so that the same test name can be used in two difference contexts.
//
//  SUITE(YourSuiteName)
//  {
//    TEST(YourTestName)
//    {
//    }
//
//    TEST(YourOtherTestName)
//    {
//    }
//  }
//
// This will place the tests into a C++ namespace called YourSuiteName, and make the suite name available to UnitTest++. 
// RunAllTests() can be called for a specific suite name, so you can use this to build named groups of tests to be run together.
// Note how members of the fixture are used as if they are a part of the test, since the macro-generated test class derives from the provided fixture class.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <hltypes/Array.h>

TEST(Array_Test)
{
	hltypes::Array<int> a;
	a.append(5); a.append(6); a.append(7);
	CHECK(a[0] == 5 && a[1] == 6 && a[2] == 7);
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
	
	hltypes::Array<test_struct> a;
	a.append(test_struct(0,0,0));
	a.append(test_struct(1,2,3));
	a.append(test_struct(6,6,6));
	CHECK(a[0] == test_struct(0,0,0) &&
	      a[1] == test_struct(1,2,3) &&
		  a[2] == test_struct(6,6,6));
}



// run all tests
int main(int argc, char **argv)
{
	return UnitTest::RunAllTests();
}
