/// @file
/// @version 4.2
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
	HL_ENUM_CLASS_DEFINE(Node::Type,
	(
		HL_ENUM_DEFINE(Node::Type, Element);
		HL_ENUM_DEFINE(Node::Type, Comment);
		HL_ENUM_DEFINE(Node::Type, Text);
	));

	Node::Node(Document* document, void* node) :
		line(0)
	{
		rapidxml::xml_node<char>* rapidXmlNode = RAPIDXML_NODE(node);
		this->name = hstr(rapidXmlNode->name(), (int)rapidXmlNode->name_size());
		this->value = hstr(rapidXmlNode->value(), (int)rapidXmlNode->value_size());
		this->type = Type::Element;
		this->filename = document->getFilename();
		//this->line = 0;
		rapidxml::node_type type = rapidXmlNode->type();
		if (type == rapidxml::node_element && this->value != "")
		{
			this->type = Type::Text;
		}
		else if (type == rapidxml::node_comment)
		{
			this->type = Type::Comment;
		}
		for (rapidxml::xml_attribute<char>* attr = rapidXmlNode->first_attribute(); attr != NULL; attr = attr->next_attribute())
		{
			this->properties[hstr(attr->name(), (int)attr->name_size())] = hstr(attr->value(), (int)attr->value_size());
		}
		for (rapidxml::xml_node<char>* child = rapidXmlNode->first_node(); child != NULL; child = child->next_sibling())
		{
			this->children += new Node(document, child);
		}
	}

	Node::~Node()
	{
		foreach (Node*, it, this->children)
		{
			delete (*it);
		}
	}

}
