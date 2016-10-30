/// @file
/// @version 4.0
///
/// @section LICENSE
///
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifdef __APPLE__
#import <Foundation/Foundation.h>

#include "hlog.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

namespace hltypes
{
	void _platformPrint(const String& tag, const String& message, int level)
	{
		// hltypes on Mac supports XCodeColors plugin to colorize output when
		// debugging in XCode :)
		// link: https://github.com/robbiehanson/XcodeColors
		
		static int XCodeColorsInstalled = -1;
		
		if (XCodeColorsInstalled == 0)
		{
			if (tag != "")
			{
				NSLog(@"[%s] %s", tag.cStr(), message.cStr());
			}
			else
			{
				NSLog(@"%s", message.cStr());
			}
		}
		else
		{
			if (XCodeColorsInstalled == -1) // put here for performance reasons
			{
				const char* result = getenv("XcodeColors");
				XCodeColorsInstalled = result != NULL && strcmp(result, "YES") == 0 ? 1 : 0;
				_platformPrint(tag, message, level);
				return;
			}
			
			if (tag != "")
			{
				int i, len = tag.size(), r, g, b;
				unsigned int x = 0;
				const char* s = tag.cStr();
				for (i = 0; i < len; i++)
				{
					x += s[i] * (i + 1) * (i + 2);
				}
				r = (x * 13) % 255;
				g = ((x * 72) / 255) % 255;
				b = (r + g) % 255;
				if (r + b + g > 512) // prevent too bright colors
				{
					r /= 2;
					g /= 2;
					b /= 2;
				}
				
				if      (level == Log::LevelWrite)
				{
					printf("\033[fg%d,%d,%d;[%s]\033[; %s\n", r, g, b, tag.cStr(), message.cStr());
				}
				else if (level == Log::LevelError)
				{
					// red background and yellow foreground
					printf("\033[bg255,0,0;\033[fg255,255,0;[%s] %s\033[;\n", tag.cStr(), message.cStr());
				}
				else if (level == Log::LevelWarn)
				{
					// grey text and grey tag background
					printf("\033[bg230,230,230;[%s]\033[; \033[fg128,128,128;%s\033[;\n", tag.cStr(), message.cStr());
				}
				else if (level == Log::LevelDebug)
				{
					// blue background and dark blue text
					printf("\033[bg204,255,255;\033[fg%d,%d,%d;[%s]\033[;\033[bg153,255,255; \033[fg0,0,255;%s\033[;\n", r, g, b, tag.cStr(), message.cStr());
				}
				else
				{
					// just in case!
					printf("%s\n", message.cStr());
				}
			}
			else
			{
				printf("%s\n", message.cStr());
			}
		}
	}
	
}
#endif
