/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <tinyxml.h>

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Exception.h"
#include "TinyXml_Document.h"
#include "TinyXml_Node.h"
#include "TinyXml_Property.h"

namespace hlxml
{
	TinyXml_Node::TinyXml_Node(TinyXml_Document* document, TiXmlNode* node) : Node()
	{
		this->document = document;
		this->node = node;
	}

	TinyXml_Node::~TinyXml_Node()
	{
		foreach_map (TiXmlAttribute*, TinyXml_Property*, it, this->props)
		{
			delete it->second;
		}
		this->props.clear();
	}

	hstr TinyXml_Node::getFilename()
	{
		return this->document->getFilename();
	}

	int TinyXml_Node::getLine()
	{
		return this->node->Row();
	}

	Node::Type TinyXml_Node::getType()
	{
		if (this->node->ToText() != NULL)
		{
			return TYPE_TEXT;
		}
		if (this->node->ToComment() != NULL)
		{
			return TYPE_COMMENT;
		}
		return TYPE_ELEMENT;
	}

	void TinyXml_Node::setProperty(chstr name, chstr value)
	{
		this->node->ToElement()->SetAttribute(name.c_str(), value.c_str());
	}

	Node* TinyXml_Node::next()
	{
		return this->document->node(this->node->NextSiblingElement());
	}

	Node* TinyXml_Node::iterChildren()
	{
		return this->document->node(this->node->FirstChildElement());
	}

	Property* TinyXml_Node::iterProperties()
	{
		return this->prop(this->node->ToElement()->FirstAttribute());
	}

	const char* TinyXml_Node::_findProperty(chstr propertyName, bool ignoreError)
	{
		for (TiXmlAttribute *attr = this->node->ToElement()->FirstAttribute(); attr != NULL; attr = attr->Next())
		{
			if (propertyName == attr->Name())
			{
				return attr->Value();
			}
		}
		return Node::_findProperty(propertyName, ignoreError);
	}

	TinyXml_Property* TinyXml_Node::prop(TiXmlAttribute* prop)
	{
		if (prop == NULL)
		{
			return NULL;
		}
		if (!this->props.has_key(prop))
		{
			this->props[prop] = new TinyXml_Property(this, prop);
		}
		return this->props[prop];
	}

	bool TinyXml_Node::_equals(const char* name)
	{
		return (hstr(this->node->Value()) == hstr(name));
	}

}
