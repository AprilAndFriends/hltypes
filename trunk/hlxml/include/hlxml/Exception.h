/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines exceptions.

#ifndef HLXML_EXCEPTION_H
#define HLXML_EXCEPTION_H

#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "hlxmlExport.h"

namespace hlxml
{
	class Node;

	class hlxmlExport _XMLException : public hltypes::exception
	{
	public:
		_XMLException(chstr msg, Node* node, chstr type, const char* file, int line);

	};

	#define XMLException(msg, node) _XMLException(msg, node, "XMLException", __FILE__, __LINE__)
	#define XMLPropertyNotExistsException(element, node) _XMLException(hstr("XML property doesn't exist: ") + element, node, "XMLPropertyNotExistsException", __FILE__, __LINE__)
	#define XMLUnknownClassException(element, node) _XMLException(hstr("Unknown class detected in XML file: ") + element, node, "XMLUnknownClassException", __FILE__, __LINE__)

}

#endif
