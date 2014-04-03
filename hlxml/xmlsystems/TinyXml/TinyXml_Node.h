/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a TinyXML node.

#ifndef HLXML_TINYXML_NODE_H
#define HLXML_TINYXML_NODE_H

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "hlxmlExport.h"
#include "Node.h"
#include "Property.h"

class TiXmlAttribute;
class TiXmlNode;

namespace hlxml
{
	class TinyXml_Document;
	class TinyXml_Property;
	class Property;

	class hlxmlExport TinyXml_Node : public Node
	{
	public:
		TinyXml_Node(TinyXml_Document* document, TiXmlNode* node);
		~TinyXml_Node();

		hstr getFilename();
		int getLine();
		Type getType();

		void setProperty(chstr name, chstr value);
		Node* next();
		Node* iterChildren();
		Property* iterProperties();

		TinyXml_Property* prop(TiXmlAttribute* prop);
	
	protected:
		TinyXml_Document* document;
		TiXmlNode* node;
		hmap<TiXmlAttribute*, TinyXml_Property*> props;

		const char* _findProperty(chstr propertyName, bool ignoreError = false);
		bool _equals(const char* name);
	
	};

}

#endif
