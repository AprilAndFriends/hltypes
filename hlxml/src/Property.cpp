/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <tinyxml.h>

#include "Node.h"
#include "Property.h"

namespace hlxml
{
	Property::Property(Node* node, TiXmlAttribute* prop)
	{
		this->node = node;
		this->prop = prop;
	}

	Property::~Property()
	{
	}

	hstr Property::name()
	{
		return hstr(this->prop->Name());
	}

	hstr Property::value()
	{
		return hstr(this->prop->Value());
	}

	Property* Property::next()
	{
		return this->node->_prop(this->prop->Next());
	}

}
