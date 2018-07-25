/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#if defined(_WIN32) && !defined(_WINRT)
#define __HL_INCLUDE_PLATFORM_HEADERS
#include "hlog.h"
#include "hplatform.h"
#include "hstring.h"
#include "platform_internal.h"

namespace hltypes
{
	static HANDLE _consoleHandle = NULL;
	static int _consoleColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // white

	void _setLevelColor(int level)
	{
		if (_consoleHandle == NULL)
		{
			_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			if (_consoleHandle == NULL)
			{
				return;
			}
			// remember the initial console color
			CONSOLE_SCREEN_BUFFER_INFO info;
			if (GetConsoleScreenBufferInfo(_consoleHandle, &info) != 0)
			{
				_consoleColor = (info.wAttributes & 0xFF);
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
		SetConsoleTextAttribute(_consoleHandle, color | FOREGROUND_INTENSITY);
	}

	void _resetColor()
	{
		// reset the console color after finished printing
		if (_consoleHandle == NULL)
		{
			SetConsoleTextAttribute(_consoleHandle, _consoleColor | FOREGROUND_INTENSITY);
		}
	}

	void _platformPrint(const String& tag, const String& message, int level)
	{
		_setLevelColor(level);
		if (tag != "")
		{
			printf("[%s] %s\n", tag.cStr(), message.cStr());
#ifdef _MSC_VER // prints in the output panel of Visual Studio
			OutputDebugStringW(("[" + tag + "] " + message + "\n").wStr().c_str());
#endif
		}
		else
		{
			printf("%s\n", message.cStr());
#ifdef _MSC_VER // prints in the output panel of Visual Studio
			OutputDebugStringW((message + "\n").wStr().c_str());
#endif
		}
		_resetColor();
	}

	bool _platformClipboardClear()
	{
		if (!OpenClipboard(NULL))
		{
			Log::error(logTag, "Cannot open the Clipboard!");
			return false;
		}
		if (!EmptyClipboard())
		{
			Log::error(logTag, "Cannot empty the Clipboard!");
			CloseClipboard();
			return false;
		}
		CloseClipboard();
		return true;
	}

	bool _platformClipboardIsString()
	{
		if (!OpenClipboard(NULL))
		{
			Log::error(logTag, "Cannot open the Clipboard!");
			return false;
		}
		bool result = (GetClipboardData(CF_TEXT) != NULL);
		CloseClipboard();
		return result;
	}

	bool _platformClipboardGetString(hstr& string)
	{
		if (!OpenClipboard(NULL))
		{
			Log::error(logTag, "Cannot open the Clipboard!");
			return false;
		}
		HANDLE handle = GetClipboardData(CF_UNICODETEXT);
		if (handle == NULL)
		{
			CloseClipboard();
			return false;
		}
		string = hstr::fromUnicode((wchar_t*)handle);
		CloseClipboard();
		return true;
	}

	bool _platformClipboardSetString(chstr string)
	{
		if (!OpenClipboard(NULL))
		{
			Log::error(logTag, "Cannot open the Clipboard!");
			return false;
		}
		if (!EmptyClipboard())
		{
			Log::error(logTag, "Cannot empty the Clipboard!");
			CloseClipboard();
			return false;
		}
		std::wstring wString = string.wStr();
		int size = (wString.size() + 1) * sizeof(wchar_t);
		HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, size);
		memcpy((wchar_t*)hGlobal, wString.c_str(), size);
		if (::SetClipboardData(CF_UNICODETEXT, hGlobal) == NULL)
		{
			Log::errorf(logTag, "Cannot set Clipboard data! System Error: %08X", GetLastError());
			CloseClipboard();
			GlobalFree(hGlobal);
			return false;
		}
		CloseClipboard();
		return true;
	}

}
#endif
