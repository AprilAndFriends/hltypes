/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "Node.h"
#include "Exception.h"

namespace hlxml
{
	_XMLException::_XMLException(chstr msg, Node* node, chstr type, const char* file, int line) :
		hltypes::exception((node == NULL ? msg : msg + ", in file " + node->getFilename() + ", line " + hstr(node->getLine())),
		file, line)
	{
	}

}
