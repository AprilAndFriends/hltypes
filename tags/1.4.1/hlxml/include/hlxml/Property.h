/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents an XML property.

#ifndef HLXML_PROPERTY_H
#define HLXML_PROPERTY_H

#ifdef USE_TINYXML
#include <tinyxml.h>
#define _xmlAttr TiXmlAttribute
#else
#include <libxml/xmlmemory.h>
#endif

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

#define foreach_xmlproperty(propertyName, nodeName) for (hlxml::Property* propertyName = nodeName->iterProperties(); propertyName != NULL; propertyName = propertyName->next())

namespace hlxml
{
#ifdef USE_TINYXML
	class hlxmlExport Property : public TiXmlAttribute
#else
	class hlxmlExport Property : public _xmlAttr
#endif
	{
	public:
		Property* next();
		hstr name();
		hstr value();

	};

}

#endif
