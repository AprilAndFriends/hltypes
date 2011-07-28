/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <libxml/xmlmemory.h>

#include "Exception.h"
#include "Property.h"

namespace hlxml
{
	Property* Property::next()
	{
		return (Property*)_xmlAttr::next;
	}

	hstr Property::name()
	{
		return hstr((const char*)_xmlAttr::name);
	}

	hstr Property::value()
	{
		return hstr((const char*)this->children->content);
	}

}
