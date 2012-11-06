/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _WIN32
#include "hplatform.h"
#if _HL_WINRT
#include <windows.h>

#include "hstring.h"

namespace hltypes
{
	void _platform_print(chstr tag, chstr message, int level)
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
#endif
