/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef _WINRT
#include "hlog.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

namespace hltypes
{
	void _platformPrint(const String& tag, const String& message, int level)
	{
		if (tag != "")
		{
			OutputDebugStringW(("[" + tag + "] " + message + "\n").wStr().c_str());
		}
		else
		{
			OutputDebugStringW((message + "\n").wStr().c_str());
		}
	}

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

