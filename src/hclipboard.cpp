/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "hclipboard.h"
#include "hstring.h"
#include "platform_internal.h"

namespace hltypes
{
	bool Clipboard::clear()
	{
		return _platformClipboardClear();
	}

	bool Clipboard::isString()
	{
		return _platformClipboardIsString();
	}

	bool Clipboard::getString(hstr& string)
	{
		return _platformClipboardGetString(string);
	}

	bool Clipboard::setString(chstr string)
	{
		return _platformClipboardSetString(string);
	}

}
