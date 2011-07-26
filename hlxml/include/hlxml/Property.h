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

#include <libxml/xmlmemory.h>

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

namespace hlxml
{
	struct hlxmlExport Property : public _xmlAttr
	{
	public:
		Property* next();
		hstr name();
		hstr value();

	};

}

#endif
