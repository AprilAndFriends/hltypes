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

namespace hlxml
{
	Node::Node(Document* document, TiXmlNode* node) : childCount(0)
	{
		this->document = document;
		this->node = node;
		this->value = this->node->Value();
		this->type = TYPE_ELEMENT;
		if (this->node->ToText() != NULL)
		{
			this->type = TYPE_TEXT;
		}
		else if (this->node->ToComment() != NULL)
		{
			this->type = TYPE_COMMENT;
		}
		for (TiXmlAttribute* attr = this->node->ToElement()->FirstAttribute(); attr != NULL; attr = attr->Next())
		{
			this->properties[attr->Name()] = attr->Value();
		}
	}

	hstr Node::getFilename()
	{
		return this->document->getFilename();
	}

	int Node::getLine()
	{
		return this->node->Row();
	}

	int Node::getChildCount()
	{
		if (this->childCount == 0)
		{
			for (TiXmlNode* node = this->node->FirstChildElement(); node != NULL; node = node->NextSiblingElement())
			{
				++this->childCount;
			}
		}
		return this->childCount;
	}

	Node* Node::next()
	{
		return this->document->_node(this->node->NextSiblingElement());
	}

	Node* Node::iterChildren()
	{
		return this->document->_node(this->node->FirstChildElement());
	}

}
