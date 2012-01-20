/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef USE_LIBXML2

#include <libxml/xmlmemory.h>

#include "LibXml2_Node.h"
#include "LibXml2_Property.h"

namespace hlxml
{
	LibXml2_Property::LibXml2_Property(LibXml2_Node* node, _xmlAttr* prop) : Property()
	{
		this->node = node;
		this->prop = prop;
	}

	LibXml2_Property::~LibXml2_Property()
	{
	}

	hstr LibXml2_Property::name()
	{
		return hstr((const char*)this->prop->name);
	}

	hstr LibXml2_Property::value()
	{
		return hstr((const char*)this->prop->children->content);
	}

	Property* LibXml2_Property::next()
	{
		return this->node->prop(this->prop->next);
	}

}

#endif