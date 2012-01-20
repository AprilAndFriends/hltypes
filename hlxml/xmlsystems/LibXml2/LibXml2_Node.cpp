/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef USE_LIBXML2

#include <libxml/xmlmemory.h>

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Exception.h"
#include "LibXml2_Document.h"
#include "LibXml2_Node.h"
#include "LibXml2_Property.h"

namespace hlxml
{
	LibXml2_Node::LibXml2_Node(LibXml2_Document* document, _xmlNode* node) : Node()
	{
		this->document = document;
		this->node = node;
	}

	LibXml2_Node::~LibXml2_Node()
	{
		foreach_map (_xmlAttr*, LibXml2_Property*, it, this->props)
		{
			delete it->second;
		}
		this->props.clear();
	}

	hstr LibXml2_Node::getFilename()
	{
		return this->document->getFilename();
	}

	int LibXml2_Node::getLine()
	{
		return this->node->line;
	}

	Node::Type LibXml2_Node::getType()
	{
		if (this->node->type == XML_TEXT_NODE)
		{
			return TYPE_TEXT;
		}
		if (this->node->type == XML_COMMENT_NODE)
		{
			return TYPE_COMMENT;
		}
		return TYPE_ELEMENT;
	}

	void LibXml2_Node::setProperty(chstr name, chstr value)
	{
		xmlSetProp(this->node, (xmlChar*)name.c_str(), (xmlChar*)value.c_str());
	}

	Node* LibXml2_Node::next()
	{
		return this->document->node(this->node->next);
	}

	Node* LibXml2_Node::iterChildren()
	{
		return this->document->node(this->node->children);
	}

	Property* LibXml2_Node::iterProperties()
	{
		return this->prop(this->node->properties);
	}

	const char* LibXml2_Node::_findProperty(chstr propertyName, bool ignoreError)
	{
		for (_xmlAttr* attr = this->node->properties; attr != NULL; attr = attr->next)
		{
			if (xmlStrcmp(attr->name, (const xmlChar*)propertyName.c_str()) == 0)
			{
				return (const char*)attr->children->content;
			}
		}
		return Node::_findProperty(propertyName, ignoreError);
	}

	LibXml2_Property* LibXml2_Node::prop(_xmlAttr* prop)
	{
		if (prop == NULL)
		{
			return NULL;
		}
		if (!this->props.has_key(prop))
		{
			this->props[prop] = new LibXml2_Property(this, prop);
		}
		return this->props[prop];
	}

	bool LibXml2_Node::_equals(const char* name)
	{
		return (xmlStrcmp(this->node->name, (const xmlChar*)name) == 0);
	}

}

#endif