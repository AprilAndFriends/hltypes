/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef USE_TINYXML
#ifdef USE_TINYXML
#include <tinyxml.h>
#define _xmlAttr TiXmlAttribute
#else
#include <libxml/xmlmemory.h>
#endif

#include "Exception.h"
#include "Node.h"
#include "Property.h"

namespace hlxml
{
	const char* Node::_findProperty(chstr propertyName, bool ignoreError)
	{
#ifdef USE_TINYXML
		
		for (TiXmlAttribute *attr = this->ToElement()->FirstAttribute(); attr != NULL; attr = attr->Next())
		{
			if(chstr(attr->Name()) == propertyName)
			{
				return attr->Value();
			}
		}
#else
		for (_xmlAttr* attr = this->properties; attr != NULL; attr = attr->next)
		{
			if (xmlStrcmp(attr->name, (const xmlChar*)propertyName.c_str()) == 0)
			{
				return (const char*)attr->children->content;
			}
		}
#endif
		if (!ignoreError) // only villians use ignoreError
		{
			throw XMLPropertyNotExistsException(propertyName, this);
		}
		return NULL;
	}

	void Node::setProperty(chstr name, chstr value)
	{
#ifdef USE_TINYXML
		this->ToElement()->SetAttribute(name.c_str(), value.c_str());
#else
		xmlSetProp(this, (xmlChar*)name.c_str(), (xmlChar*)value.c_str());
#endif
	}

	Node* Node::next()
	{
#ifdef USE_TINYXML
		return (Node *)this->NextSiblingElement();
#else
		return (Node*)_xmlNode::next;
#endif
	}

	Node* Node::iterChildren()
	{
#ifdef USE_TINYXML
		return (Node *)this->FirstChildElement();
#else
		return (Node*)this->children;
#endif
	}

	Property* Node::iterProperties()
	{
#ifdef USE_TINYXML
		return (Property*)this->ToElement()->FirstAttribute();
#else
		return (Property*)this->properties;
#endif
	}

	bool Node::operator==(const char* name)
	{
#ifdef USE_TINYXML
		printf("this->Value() %s :: name %s\n", this->Value(), name);
		return chstr(this->Value()) == chstr(name);
#else
		return (xmlStrcmp(this->name, (const xmlChar*)name) == 0);
#endif
	}

	bool Node::operator!=(const char* name)
	{
#ifdef USE_TINYXML
		printf("this->Value() %s :: name %s\n", this->Value(), name);
		return chstr(this->Value()) != chstr(name);
#else
		return (xmlStrcmp(this->name, (const xmlChar*)name) != 0);
#endif
	}

	bool Node::operator==(chstr name)
	{
#ifdef USE_TINYXML
		printf("this->Value() %s :: name %s\n", this->Value(), name.c_str());
		return chstr(this->Value()) == name;
#else
		return (xmlStrcmp(this->name, (const xmlChar*)name.c_str()) == 0);
#endif
	}

	bool Node::operator!=(chstr name)
	{
#ifdef USE_TINYXML
		printf("this->Value() %s :: name %s\n", this->Value(), name.c_str());
		return chstr(this->Value()) != name;
#else
		return (xmlStrcmp(this->name, (const xmlChar*)name.c_str()) != 0);
#endif
	}

}

#endif