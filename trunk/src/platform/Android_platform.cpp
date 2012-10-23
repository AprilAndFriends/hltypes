/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.0
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
	void _platform_print(chstr tag, chstr message, int level)
	{
		hstr log_message = (tag != "" ? "[" + tag + "] " + message : message);
		__android_log_write(level, tag.c_str(), log_message.c_str());
	}

}

#endif
