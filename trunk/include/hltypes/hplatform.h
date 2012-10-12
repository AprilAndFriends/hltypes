/// @file
/// @author  Boris Mikic
/// @version 1.9
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Provides special preprocessor macros for platform definitions.

#ifndef HLTYPES_PLATFORM_H
#define HLTYPES_PLATFORM_H

#include <windows.h>
#if defined(_WIN32) && defined(WINAPI_FAMILY) && defined(WINAPI_FAMILY_PARTITION)
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#define _HL_WINRT 1
#using <Windows.winmd>
#define _HL_HSTR_TO_PSTR(string) ref new Platform::String((string).w_str().c_str())
#define _HL_HSTR_TO_PSTR_DEF(string) Platform::String^ p ## string = _HL_HSTR_TO_PSTR(string)
#endif
#endif
#ifndef _HL_WINRT
#define _HL_WINRT 0
#endif

#endif
