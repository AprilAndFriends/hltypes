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
#include "hstring.h"

namespace hltypes
{
	void _platform_print(chstr tag, chstr message, int level)
	{
		// WinRT does not support console style printing
	}

}

#endif
#endif
