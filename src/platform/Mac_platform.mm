/// @file
/// @version 4.0
///
/// @section LICENSE
///
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#if defined(__APPLE__) && defined(_MAC)
#import <AppKit/Appkit.h>
#import <Foundation/Foundation.h>

#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

namespace hltypes
{
	bool _platformClipboardClear()
	{
		[[NSPasteboard generalPasteboard] clearContents];
		return true;
	}
	
	bool _platformClipboardIsString()
	{
		return false;
	}
	
	bool _platformClipboardGetString(hstr& string)
	{
		string = hstr([[[NSPasteboard generalPasteboard] stringForType:NSStringPboardType] UTF8String]);
		return true;
	}
	
	bool _platformClipboardSetString(chstr string)
	{
		[[NSPasteboard generalPasteboard] declareTypes:[NSArray arrayWithObject:NSPasteboardTypeString] owner:nil];
		[[NSPasteboard generalPasteboard] setString:[NSString stringWithUTF8String:string.cStr()] forType:NSPasteboardTypeString];
		return true;
	}
	
}
#endif
