/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "RapidXmlConfig.h"
#include <rapidxml.hpp>

#include "Document.h"
#include "Exception.h"
#include "Node.h"

#define RAPIDXML_DOCUMENT ((rapidxml::xml_document<char>*)this->document)
#define RAPIDXML_NODE(node) ((rapidxml::xml_node<char>*)node)

namespace hlxml
{
	Node::Node(Document* document, void* node) : childCount(0)
	{
		this->document = document;
		this->node = node;
		rapidxml::xml_node<char>* rapidXmlNode = RAPIDXML_NODE(this->node);
		this->name = rapidXmlNode->name();
		this->value = rapidXmlNode->value();
		this->type = TYPE_ELEMENT;
		rapidxml::node_type type = rapidXmlNode->type();
		if (type == rapidxml::node_element && this->value != "")
		{
			this->type = TYPE_TEXT;
		}
		else if (type == rapidxml::node_comment)
		{
			this->type = TYPE_COMMENT;
		}
		for (rapidxml::xml_attribute<char>* attr = rapidXmlNode->first_attribute(); attr != NULL; attr = attr->next_attribute())
		{
			this->properties[attr->name()] = attr->value();
		}
	}

	hstr Node::getFilename() const
	{
		return this->document->getFilename();
	}

	int Node::getLine() const
	{
		return 0;
	}

	int Node::getChildCount()
	{
		if (this->childCount == 0)
		{
			for (rapidxml::xml_node<char>* node = RAPIDXML_NODE(this->node)->first_node(); node != NULL; node = node->next_sibling())
			{
				++this->childCount;
			}
		}
		return this->childCount;
	}

	Node* Node::next() const
	{
		return this->document->_node(RAPIDXML_NODE(this->node)->next_sibling());
	}

	Node* Node::iterChildren() const
	{
		return this->document->_node(RAPIDXML_NODE(this->node)->first_node());
	}

}
