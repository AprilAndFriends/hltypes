/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a generic XML node.

#ifndef HLXML_NODE_H
#define HLXML_NODE_H

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "hlxmlExport.h"

class TiXmlNode;
class TiXmlAttribute;

#define foreach_xmlnode(nodeName, rootName) for (hlxml::Node* nodeName = rootName->iterChildren(); nodeName != NULL; nodeName = nodeName->next())

namespace hlxml
{
	class Document;
	class Property;

	class hlxmlExport Node
	{
	public:
		friend class Document;
		friend class Property;

		enum Type
		{
			// TODO
			TYPE_ELEMENT,
			TYPE_COMMENT,
			TYPE_TEXT
		};

		virtual ~Node();

		hstr getFilename();
		int getLine();
		Type getType();

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

		bool operator==(const char* name);
		bool operator!=(const char* name);
		bool operator==(chstr name);
		bool operator!=(chstr name);
		
	protected:
		Document* document;
		TiXmlNode* node;
		hmap<TiXmlAttribute*, Property*> props;

		Node(Document* document, TiXmlNode* node);

		Property* _prop(TiXmlAttribute* prop);
		const char* _findProperty(chstr propertyName, bool ignoreError = false);

	};

}

#endif
