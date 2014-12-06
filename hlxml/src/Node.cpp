/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <tinyxml.h>

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Document.h"
#include "Exception.h"
#include "Node.h"
#include "Property.h"

namespace hlxml
{
	Node::Node(Document* document, TiXmlNode* node)
	{
		this->document = document;
		this->node = node;
	}

	Node::~Node()
	{
		foreach_map (TiXmlAttribute*, Property*, it, this->props)
		{
			delete it->second;
		}
		this->props.clear();
	}

	hstr Node::getFilename()
	{
		return this->document->getFilename();
	}

	int Node::getLine()
	{
		return this->node->Row();
	}

	Node::Type Node::getType()
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

	hstr Node::getValue()
	{
		return hstr(this->node->Value());
	}

	bool Node::pbool(chstr propertyName)
	{
		return (bool)hstr(this->_findProperty(propertyName));
	}

	bool Node::pbool(chstr propertyName, bool defaultValue)
	{
		const char* nodeValue = this->_findProperty(propertyName, true);
		return (nodeValue != NULL ? (bool)hstr(nodeValue) : defaultValue);
	}

	int Node::pint(chstr propertyName)
	{
		return (int)hstr(this->_findProperty(propertyName));
	}

	int Node::pint(chstr propertyName, int defaultValue)
	{
		const char* nodeValue = this->_findProperty(propertyName, true);
		return (nodeValue != NULL ? (int)hstr(nodeValue) : defaultValue);
	}

	float Node::pfloat(chstr propertyName)
	{
		return (float)hstr(this->_findProperty(propertyName));
	}

	float Node::pfloat(chstr propertyName, float defaultValue)
	{
		const char* nodeValue = this->_findProperty(propertyName, true);
		return (nodeValue != NULL ? (float)hstr(nodeValue) : defaultValue);
	}

	hstr Node::pstr(chstr propertyName)
	{
		return this->_findProperty(propertyName);
	}

	hstr Node::pstr(chstr propertyName, chstr defaultValue)
	{
		const char* nodeValue = this->_findProperty(propertyName, true);
		return (nodeValue != NULL ? hstr(nodeValue) : defaultValue);
	}

	bool Node::pexists(chstr propertyName)
	{
		return (this->_findProperty(propertyName, true) != NULL);
	}

	Node* Node::next()
	{
		return this->document->_node(this->node->NextSiblingElement());
	}

	Node* Node::iterChildren()
	{
		return this->document->_node(this->node->FirstChildElement());
	}

	Property* Node::iterProperties()
	{
		return this->_prop(this->node->ToElement()->FirstAttribute());
	}

	const char* Node::_findProperty(chstr propertyName, bool ignoreError)
	{
		for (TiXmlAttribute *attr = this->node->ToElement()->FirstAttribute(); attr != NULL; attr = attr->Next())
		{
			if (propertyName == attr->Name())
			{
				return attr->Value();
			}
		}
		if (!ignoreError) // only villians use ignoreError
		{
			throw XMLPropertyNotExistsException(propertyName, this);
		}
		return NULL;
	}

	Property* Node::_prop(TiXmlAttribute* prop)
	{
		if (prop == NULL)
		{
			return NULL;
		}
		if (!this->props.has_key(prop))
		{
			this->props[prop] = new Property(this, prop);
		}
		return this->props[prop];
	}

	bool Node::operator==(const char* name)
	{
		return (hstr(this->node->Value()) == hstr(name));
	}

	bool Node::operator!=(const char* name)
	{
		return (hstr(this->node->Value()) != hstr(name));
	}

	bool Node::operator==(chstr name)
	{
		return (hstr(this->node->Value()) == name);
	}

	bool Node::operator!=(chstr name)
	{
		return (hstr(this->node->Value()) != name);
	}

}
