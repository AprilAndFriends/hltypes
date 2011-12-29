/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "hstream.h"

namespace hltypes
{
	Stream::Stream(unsigned char encryption_offset) : StreamBase(encryption_offset)
	{
	}
	
	Stream::~Stream()
	{
	}
	
}
