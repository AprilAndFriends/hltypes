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
/// Represents a LibXML2 node.

#ifdef USE_LIBXML2
#ifndef HLXML_LIBXML2_NODE_H
#define HLXML_LIBXML2_NODE_H

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "hlxmlExport.h"
#include "Node.h"
#include "Property.h"

struct _xmlAttr;
struct _xmlNode;

namespace hlxml
{
	class LibXml2_Document;
	class LibXml2_Property;
	class Property;

	class hlxmlExport LibXml2_Node : public Node
	{
	public:
		LibXml2_Node(LibXml2_Document* document, _xmlNode* node);
		~LibXml2_Node();

		hstr getFilename();
		int getLine();
		Type getType();

		void setProperty(chstr name, chstr value);
		Node* next();
		Node* iterChildren();
		Property* iterProperties();

		LibXml2_Property* prop(_xmlAttr* prop);
	
	protected:
		LibXml2_Document* document;
		_xmlNode* node;
		hmap<_xmlAttr*, LibXml2_Property*> props;

		const char* _findProperty(chstr propertyName, bool ignoreError = false);
		bool _equals(const char* name);
	
	};

}

#endif
#endif