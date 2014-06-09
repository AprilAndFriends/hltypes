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
/// Represents a generic XML property.

#ifndef HLXML_PROPERTY_H
#define HLXML_PROPERTY_H

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

#define foreach_xmlproperty(propertyName, nodeName) for (hlxml::Property* propertyName = nodeName->iterProperties(); propertyName != NULL; propertyName = propertyName->next())

class TiXmlAttribute;

namespace hlxml
{
	class Node;

	class hlxmlExport Property
	{
	public:
		friend class Node;

		virtual ~Property();

		hstr name();
		hstr value();
		Property* next();

	protected:
		Node* node;
		TiXmlAttribute* prop;

		Property(Node* node, TiXmlAttribute* prop);

	};

}

#endif
