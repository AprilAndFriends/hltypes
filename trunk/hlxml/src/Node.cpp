/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <libxml/xmlmemory.h>

#include "Exception.h"
#include "Node.h"
#include "Property.h"

namespace hlxml
{
	Node::Node()
	{
	}

	Node::~Node()
	{
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

	const char* Node::_findProperty(chstr propertyName, bool ignoreError)
	{
		if (!ignoreError) // only villians use ignoreError
		{
			throw XMLPropertyNotExistsException(propertyName, this);
		}
		return NULL;
	}

	bool Node::operator==(const char* name)
	{
		return this->_equals(name);
	}

	bool Node::operator!=(const char* name)
	{
		return (!this->_equals(name));
	}

	bool Node::operator==(chstr name)
	{
		return this->_equals(name.c_str());
	}

	bool Node::operator!=(chstr name)
	{
		return (!this->_equals(name.c_str()));
	}

}
