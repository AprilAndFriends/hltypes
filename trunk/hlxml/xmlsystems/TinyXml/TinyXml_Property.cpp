/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef USE_TINYXML

#include <tinyxml/tinyxml.h>

#include "TinyXml_Node.h"
#include "TinyXml_Property.h"

namespace hlxml
{
	TinyXml_Property::TinyXml_Property(TinyXml_Node* node, TiXmlAttribute* prop) : Property()
	{
		this->node = node;
		this->prop = prop;
	}

	TinyXml_Property::~TinyXml_Property()
	{
	}

	hstr TinyXml_Property::name()
	{
		return hstr((const char*)this->prop->Name());
	}

	hstr TinyXml_Property::value()
	{
		return hstr((const char*)this->prop->Value());
	}

	Property* TinyXml_Property::next()
	{
		return this->node->prop(this->prop->Next());
	}

}

#endif