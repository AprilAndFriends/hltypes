/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides platform-specific definitions for special functionality.

#ifndef HLTYPES_PLATFORM_INTERNAL_H
#define HLTYPES_PLATFORM_INTERNAL_H

#include "hstring.h"

namespace hltypes
{
	void _platformPrint(const String& tag, const String& message, int level);

}

#endif