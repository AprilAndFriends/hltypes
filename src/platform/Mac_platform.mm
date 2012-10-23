/// @file
/// @author  Boris Mikic
/// @author  Kresimir Spes
/// @version 2.0
///
/// @section LICENSE
///
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#import <Foundation/Foundation.h>
#include "hstring.h"

void nsLog(chstr message)
{
	NSLog(@"%s", message.c_str());
}
