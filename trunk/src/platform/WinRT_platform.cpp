/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _WINRT
#include "hplatform.h"
#include "hstring.h"

namespace hltypes
{
	void _platform_print(const String& tag, const String& message, int level)
	{
		// using console style printing invalidates WACK certification so it is used only in Debug builds
#ifdef _DEBUG
		if (tag != "")
		{
			OutputDebugString(("[" + tag + "] " + message + "\n").w_str().c_str());
		}
		else
		{
			OutputDebugString((message + "\n").w_str().c_str());
		}
#endif
	}

}

#endif

