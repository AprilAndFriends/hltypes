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
/// Represents an XML node.

#ifndef HLXML_NODE_H
#define HLXML_NODE_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

namespace hlxml
{
	struct Property;

	struct hlxmlExport Node : public _xmlNode
	{
	public:
		bool pbool(chstr propertyName);
		bool pbool(chstr propertyName, bool defaultValue);
		int pint(chstr propertyName);
		int pint(chstr propertyName, int defaultValue);
		float pfloat(chstr propertyName);
		float pfloat(chstr propertyName, float defaultValue);
		hstr pstr(chstr propertyName);
		hstr pstr(chstr propertyName, chstr defaultValue);
	
		bool pexists(chstr propertyName);
	
		Node* next();
		Node* iterChildren();
		Property* iterProperties();
	
		bool operator==(chstr name);
		bool operator!=(chstr name);
	
	protected:
		const char* findProperty(chstr propertyName, bool ignoreError = false);
	
	};

}

#endif
