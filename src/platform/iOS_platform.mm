/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#if defined(__APPLE__) && defined(_IOS)
#import <Foundation/Foundation.h>

#include "hlog.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

namespace hltypes
{
	bool _platformClipboardClear()
	{
		Log::error(logTag, CLIPBOARD_ERROR);
		return false;
	}
	
	bool _platformClipboardIsString()
	{
		Log::error(logTag, CLIPBOARD_ERROR);
		return false;
	}
	
	bool _platformClipboardGetString(hstr& string)
	{
		Log::error(logTag, CLIPBOARD_ERROR);
		return false;
	}
	
	bool _platformClipboardSetString(chstr string)
	{
		Log::error(logTag, CLIPBOARD_ERROR);
		return false;
	}
	
}
#endif
