/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#if defined(_WIN32) && !defined(_WINRT)
#include <iostream>

#include "hlog.h"
#include "hplatform.h"
#include "hstring.h"

namespace hltypes
{
	void _setLevelColor(int level)
	{
		static HANDLE consoleHandle = NULL;
		if (consoleHandle == NULL)
		{
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		}
		int color = 15; // white
		if (level == Log::LevelError)
		{
			color = 12; // red
		}
		else if (level == Log::LevelWarn)
		{
			color = 14; // yellow
		}
		else if (level == Log::LevelDebug)
		{
			color = 10; // green
		}
		SetConsoleTextAttribute(consoleHandle, color);
	}

	void _platform_print(const String& tag, const String& message, int level)
	{
		_setLevelColor(level);
		if (tag != "")
		{
			printf("[%s] %s\n", tag.c_str(), message.c_str());
#if defined(_MSC_VER) && defined(_DEBUG) // prints in the debug output panel of Visual Studio
			OutputDebugStringW(("[" + tag + "] " + message + "\n").w_str().c_str());
#endif
		}
		else
		{
			printf("%s\n", message.c_str());
#if defined(_MSC_VER) && defined(_DEBUG) // prints in the debug output panel of Visual Studio
			OutputDebugStringW((message + "\n").w_str().c_str());
#endif
		}
	}

}

#endif
