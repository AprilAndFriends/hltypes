/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "Node.h"
#include "Exception.h"

namespace hlxml
{
	_XMLException::_XMLException(chstr msg, Node* node, chstr type, const char* file, int line) :
		hltypes::exception("", file, line)
	{
		this->msg += msg;
		if (node != NULL)
		{
			this->msg += ", in file " + node->getFilename() + ", line " + hstr(node->getLine());
		}
	}

}