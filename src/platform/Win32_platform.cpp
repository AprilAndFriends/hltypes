/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#if defined(_WIN32) && !defined(_WINRT)
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
			if (consoleHandle == NULL)
			{
				return;
			}
		}
		int color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // white
		if (level == Log::LevelError)
		{
			color = FOREGROUND_RED; // red
		}
		else if (level == Log::LevelWarn)
		{
			color = FOREGROUND_RED | FOREGROUND_GREEN; // yellow
		}
		else if (level == Log::LevelDebug)
		{
			color = FOREGROUND_GREEN; // green
		}
		SetConsoleTextAttribute(consoleHandle, color | FOREGROUND_INTENSITY);
	}

	void _platformPrint(const String& tag, const String& message, int level)
	{
		_setLevelColor(level);
		if (tag != "")
		{
			printf("[%s] %s\n", tag.c_str(), message.c_str());
#ifdef _MSC_VER // prints in the output panel of Visual Studio
			OutputDebugStringW(("[" + tag + "] " + message + "\n").w_str().c_str());
#endif
		}
		else
		{
			printf("%s\n", message.c_str());
#ifdef _MSC_VER // prints in the output panel of Visual Studio
			OutputDebugStringW((message + "\n").w_str().c_str());
#endif
		}
	}

}

#endif
