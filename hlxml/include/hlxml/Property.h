/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a generic XML property.

#ifndef HLXML_PROPERTY_H
#define HLXML_PROPERTY_H

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

#define foreach_xmlproperty(propertyName, nodeName) for (hlxml::Property* propertyName = nodeName->iterProperties(); propertyName != NULL; propertyName = propertyName->next())

namespace hlxml
{
	class hlxmlExport Property
	{
	public:
		Property();
		virtual ~Property();

		virtual hstr name() = 0;
		virtual hstr value() = 0;
		virtual Property* next() = 0;

	};

}

#endif
