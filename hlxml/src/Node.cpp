/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "Exception.h"
#include "Node.h"

namespace hlxml
{
	const char* Node::findProperty(chstr propertyName, bool ignoreError)
	{
		for (_xmlAttr* a = this->properties; a != NULL; a = a->next)
		{
			if (xmlStrcmp(a->name, (const xmlChar*)propertyName.c_str()) == 0)
			{
				return (const char*)a->children->content;
			}
		}
		if (!ignoreError)
		{
			throw XMLPropertyNotExistsException(propertyName, this);
		}
		return NULL;
	}

	bool Node::pbool(chstr propertyName)
	{
		return (bool)hstr(this->findProperty(propertyName));
	}

	bool Node::pbool(chstr propertyName, bool defaultValue)
	{
		const char* nodeValue = this->findProperty(propertyName, true);
		return (nodeValue != NULL ? (bool)hstr(nodeValue) : defaultValue);
	}

	int Node::pint(chstr propertyName)
	{
		return (int)hstr(this->findProperty(propertyName));
	}

	int Node::pint(chstr propertyName, int defaultValue)
	{
		const char* nodeValue = this->findProperty(propertyName, true);
		return (nodeValue != NULL ? (int)hstr(nodeValue) : defaultValue);
	}

	float Node::pfloat(chstr propertyName)
	{
		return (float)hstr(this->findProperty(propertyName));
	}

	float Node::pfloat(chstr propertyName, float defaultValue)
	{
		const char* nodeValue = this->findProperty(propertyName, true);
		return (nodeValue != NULL ? (float)hstr(nodeValue) : defaultValue);
	}

	hstr Node::pstr(chstr propertyName)
	{
		return this->findProperty(propertyName);
	}

	hstr Node::pstr(chstr propertyName, chstr defaultValue)
	{
		const char* nodeValue = this->findProperty(propertyName, true);
		return (nodeValue != NULL ? nodeValue : defaultValue);
	}

	bool Node::pexists(chstr propertyName)
	{
		return (this->findProperty(propertyName, true) != NULL);
	}

	Node* Node::next()
	{
		return (Node*)_xmlNode::next;
	}

	Node* Node::iterChildren()
	{
		return (Node*)this->children;
	}

	Property* Node::iterProperties()
	{
		return (Property*)this->properties;
	}

	bool Node::operator==(chstr name)
	{
		return (xmlStrcmp(this->name, (const xmlChar*)name.c_str()) == 0);
	}

	bool Node::operator!=(chstr name)
	{
		return (xmlStrcmp(this->name, (const xmlChar*)name.c_str()) != 0);
	}

}
