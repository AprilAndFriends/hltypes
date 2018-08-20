/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides special preprocessor macros for platform definitions and special platform specific functions.

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// split into 2 parts, because of platform header inclusion
#ifndef HLTYPES_PLATFORM_H
	#if defined(_WIN32) && defined(_MSC_VER)
		#ifdef max
			#undef max
		#endif
		#ifdef min
			#undef min
		#endif
		#ifndef NOMINMAX
			#define NOMINMAX
		#endif
	#endif
#endif

#ifndef HL_DEPRECATED
	#if defined(_MSC_VER) && !defined(__ANDROID__)
		#define HL_DEPRECATED(message) __declspec(deprecated(message))
	#else
		#define HL_DEPRECATED(message) __attribute__((deprecated(message)))
	#endif
#endif

#ifdef __HL_INCLUDE_PLATFORM_HEADERS
#ifndef HLTYPES_PLATFORM_H_PLATFORM_HEADERS_INCLUDED
#define HLTYPES_PLATFORM_H_PLATFORM_HEADERS_INCLUDED

#if defined(_WIN32) && defined(_MSC_VER)
	#include <windows.h>
#endif

#endif
#endif

#ifndef HLTYPES_PLATFORM_H
#define HLTYPES_PLATFORM_H
#define _HL_TRY_DELETE(name) \
	if (name != NULL) \
	{ \
		delete name; \
		name = NULL; \
	}
#define _HL_TRY_DELETE_ARRAY(name) \
	if (name != NULL) \
	{ \
		delete[] name; \
		name = NULL; \
	}
#define _HL_TRY_RELEASE(name) \
	if (name != NULL) \
	{ \
		name->Release(); \
		name = NULL; \
	}

#if defined(_WIN32) && defined(_MSC_VER)
	// define _UWP for external projects just in case
	#if !defined(_UWP) && defined(WINAPI_FAMILY) && defined(WINAPI_FAMILY_PARTITION)
		#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
			#define _UWP
		#endif
	#endif
	#ifdef _UWP
		#if !defined(_WINPHONE) && !defined(_OPENKODE)
			#using <Windows.winmd>
		#endif
		#include <wrl.h>
		#define _HL_HSTR_TO_PSTR(string) ref new Platform::String((string).wStr().c_str())
		#define _HL_HSTR_TO_PSTR_DEF(string) Platform::String^ p ## string = _HL_HSTR_TO_PSTR(string)
		#define _HL_PSTR_TO_HSTR(string) hltypes::String::fromUnicode((string)->Data())
	#endif
#endif

#endif
#endif
