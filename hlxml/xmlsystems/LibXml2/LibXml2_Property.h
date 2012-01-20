/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a LibXML2 property.

#ifdef USE_LIBXML2
#ifndef HLXML_LIBXML2_PROPERTY_H
#define HLXML_LIBXML2_PROPERTY_H

#include <hltypes/hstring.h>

#include "hlxmlExport.h"
#include "Property.h"

struct _xmlAttr;

namespace hlxml
{
	class LibXml2_Node;

	class hlxmlExport LibXml2_Property : public Property
	{
	public:
		LibXml2_Property(LibXml2_Node* node, _xmlAttr* prop);
		~LibXml2_Property();

		hstr name();
		hstr value();
		Property* next();

	protected:
		LibXml2_Node* node;
		_xmlAttr* prop;

	};

}

#endif
#endif