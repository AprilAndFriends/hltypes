/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _ANDROID
#include <android/log.h>

#include "hplatform.h"
#include "hstring.h"

namespace hltypes
{
	void _platform_print(const String& tag, const String& message, int level)
	{
		if (tag != "")
		{
			__android_log_print(level, tag.c_str(), "[%s] %s", tag.c_str(), message.c_str());
		}
		else
		{
			__android_log_write(level, "", message.c_str());
		}
	}

}

#endif
