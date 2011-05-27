/// @file
/// @author  Kresimir Spes
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

class test_struct
{
public:
	float x,y,z;
	test_struct() { x=y=z=0; }
	test_struct(float _x,float _y,float _z) { x=_x; y=_y; z=_z; }
	bool operator ==(const test_struct& t) { return x==t.x && y==t.y && z==t.z; }
};

// run all tests
int main(int argc, char **argv)
{
	return UnitTest::RunAllTests();
}
