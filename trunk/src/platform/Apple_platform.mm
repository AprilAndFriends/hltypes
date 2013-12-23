/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.2
///
/// @section LICENSE
///
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#import <Foundation/Foundation.h>

#include "hplatform.h"
#include "hstring.h"
#include "hlog.h"

namespace hltypes
{
	void _platform_print(const String& tag, const String& message, int level)
	{
        // hltypes on Mac supports XCodeColors plugin to colorize output when
        // debugging in XCode :)
        // link: https://github.com/robbiehanson/XcodeColors
        
        static int XCodeColorsInstalled = -1;
        
        if (XCodeColorsInstalled == 0)
        {
            if (tag != "")
            {
                NSLog(@"[%s] %s", tag.c_str(), message.c_str());
            }
            else
            {
                NSLog(@"%s", message.c_str());
            }
        }
        else
        {
            if (XCodeColorsInstalled == -1) // put here for performance reasons
            {
                const char* result = getenv("XcodeColors");
                XCodeColorsInstalled = result != NULL && strcmp(result, "YES") == 0 ? 1 : 0;
                _platform_print(tag, message, level);
                return;
            }
            
            if (tag != "")
            {
                int i, len = tag.length(), r, g, b;
                unsigned int x = 0;
                const char* s = tag.c_str();
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
                    printf("\033[fg%d,%d,%d;[%s]\033[; %s\n", r, g, b, tag.c_str(), message.c_str());
                }
                else if (level == Log::LevelError)
                {
                    // red background and yellow foreground
                    printf("\033[bg255,0,0;\033[fg255,255,0;[%s] %s\033[;\n", tag.c_str(), message.c_str());
                }
                else if (level == Log::LevelWarn)
                {
                    // grey text and grey tag background
                    printf("\033[bg230,230,230;[%s]\033[; \033[fg128,128,128;%s\033[;\n", tag.c_str(), message.c_str());
                }
                else if (level == Log::LevelDebug)
                {
                    // blue background and dark blue text
                    printf("\033[bg204,255,255;\033[fg%d,%d,%d;[%s]\033[;\033[bg153,255,255; \033[fg0,0,255;%s\033[;\n", r, g, b, tag.c_str(), message.c_str());
                }
                else
                {
                    // just in case!
                    printf("%s\n", message.c_str());
                }
            }
            else
            {
                printf("%s\n", message.c_str());
            }
        }
	}
	
}
