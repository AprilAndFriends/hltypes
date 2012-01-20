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
#ifdef USE_TINYXML
#include <tinyxml.h>
#define _xmlAttr TiXmlAttribute
#else
#include <libxml/xmlmemory.h>
#endif

#include "Exception.h"
#include "Property.h"

namespace hlxml
{
	Property* Property::next()
	{
#ifdef USE_TINYXML
		return (Property*)this->Next();
#else
		return (Property*)_xmlAttr::next;
#endif
	}

	hstr Property::name()
	{
#ifdef USE_TINYXML
		return hstr((const char*)this->Name());
#else
		return hstr((const char*)_xmlAttr::name);
#endif
	}

	hstr Property::value()
	{
#ifdef USE_TINYXML
		return hstr((const char*)this->Value());
#else
		return hstr((const char*)this->children->content);
#endif
	}

}

#endif