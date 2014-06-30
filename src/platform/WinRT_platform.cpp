/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _WINRT
#include "hplatform.h"
#include "hstring.h"

namespace hltypes
{
	void _platform_print(const String& tag, const String& message, int level)
	{
		if (tag != "")
		{
			OutputDebugStringW(("[" + tag + "] " + message + "\n").w_str().c_str());
		}
		else
		{
			OutputDebugStringW((message + "\n").w_str().c_str());
		}
	}

}

#endif

