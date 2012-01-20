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
/// Represents a TinyXML node.

#ifdef USE_TINYXML
#ifndef HLXML_NODE_H
#define HLXML_NODE_H

#ifdef USE_TINYXML
#include <tinyxml.h>
#define _xmlAttr TiXmlAttribute
#else
#include <libxml/xmlmemory.h>
#endif

#include <hltypes/hstring.h>

#include "hlxmlExport.h"

#define foreach_xmlnode(nodeName, rootName) for (hlxml::Node* nodeName = rootName->iterChildren(); nodeName != NULL; nodeName = nodeName->next())

namespace hlxml
{
	class Property;

#ifdef USE_TINYXML
	struct hlxmlExport Node : public TiXmlNode
#else
	struct hlxmlExport Node : public _xmlNode
#endif
	{
	public:
		bool pexists(chstr propertyName);
		
		void setProperty(chstr name, chstr value);
		Node* next();
		Node* iterChildren();
		Property* iterProperties();
	
		bool operator==(const char* name);
		bool operator!=(const char* name);
		bool operator==(chstr name);
		bool operator!=(chstr name);
	
	protected:
		const char* _findProperty(chstr propertyName, bool ignoreError = false);
	
	};

}

#endif
#endif