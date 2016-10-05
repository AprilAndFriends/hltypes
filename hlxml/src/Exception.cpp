/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hexception.h>
#include <hltypes/hstring.h>

#include "Node.h"
#include "Exception.h"

namespace hlxml
{
	_XMLException::_XMLException(chstr message, Node* node, const char* file, int line) :
		hexception("", file, line)
	{
		this->_setInternalMessage((node == NULL ? message : message + ", in file " + node->filename + ", line " + hstr(node->line)), file, line);
	}

}
