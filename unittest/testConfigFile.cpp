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

#include <hltypes/exception.h>
#include <hltypes/hconfigfile.h>
#include <hltypes/hfile.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

/******* FILE ******************************************************************************/
TEST(CfgFile_Read)
{
	hstr filename = "test.cfg";
	hfile f(filename, hltypes::WRITE);
	f.write_line("[test]");
	f.write_line("A: 1");
	f.write_line("B: gvec2");
	f.write_line("C: lol");
	f.write_line("D: ._.;");
	f.close();
	hcfgfile cf(filename);
	hmap<hstr, hstr> entries = cf._getInternalMap();
	foreach_in_map (hstr, it, entries)
	{
		printf("%s %s\n", it->first.c_str(), it->second.c_str());
	}
	CHECK(true);
}

