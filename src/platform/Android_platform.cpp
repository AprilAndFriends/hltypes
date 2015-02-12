/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _ANDROID
#include <android/log.h>

#include "hplatform.h"
#include "hstring.h"

namespace hltypes
{
	void _platformPrint(const String& tag, const String& message, int level)
	{
		if (tag != "")
		{
			__android_log_print(level, tag.cStr(), "[%s] %s", tag.cStr(), message.cStr());
		}
		else
		{
			__android_log_write(level, "", message.cStr());
		}
	}

}

#endif
