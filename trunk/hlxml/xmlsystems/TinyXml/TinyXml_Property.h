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
/// Represents a TinyXML property.

#ifdef USE_TINYXML
#ifndef HLXML_TINYXML_PROPERTY_H
#define HLXML_TINYXML_PROPERTY_H

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

class TiXmlAttribute;

namespace hlxml
{
	class hlxmlExport TinyXml_Property : public Property
	{
	public:
		TinyXml_Property(TinyXml_Node* node, TiXmlAttribute* prop);
		~TinyXml_Property();

		hstr name();
		hstr value();
		Property* next();

	protected:
		TinyXml_Node* node;
		TiXmlAttribute* prop;

	};
}

#endif
#endif