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

#include <tinyxml/tinyxml.h>

#include <hltypes/hstring.h>

#include "Exception.h"
#include "TinyXml_Document.h"
#include "TinyXml_Node.h"

namespace hlxml
{
	TinyXml_Document::TinyXml_Document(chstr filename) : Document(filename), rootNode(NULL)
	{
		hstr realFilename = normalize_path(filename);
		this->document = new TiXmlDocument(realFilename.c_str());
		if (this->document == NULL)
		{
			throw XMLException("Unable to parse xml file '" + realFilename + "', document is invalid", NULL);
		}
		this->document->LoadFile();
	}

	TinyXml_Document::~TinyXml_Document()
	{
		this->rootNode = NULL;
		foreach_map (TiXmlNode*, TinyXml_Node*, it, this->nodes)
		{
			delete it->second;
		}
		this->nodes.clear();
		if (this->document != NULL)
		{
			delete this->document;
		}
	}

	Node* TinyXml_Document::root(chstr type)
	{
		if (this->rootNode == NULL)
		{
			TiXmlNode* tinyXmlNode = this->document->FirstChildElement();
			if (tinyXmlNode == NULL)
			{
				throw XMLException("No root node found in XML file '" + this->filename + "'!", NULL);
			}
			this->rootNode = this->node(tinyXmlNode);
			if (type != "" && *this->rootNode != type)
			{
				throw XMLException("Root node type is not '" + type + "' in XML file '" + this->filename + "'!", NULL);
			}
		}
		return this->rootNode;
	}

	TinyXml_Node* TinyXml_Document::node(TiXmlNode* node)
	{
		if (node == NULL)
		{
			return NULL;
		}
		if (!this->nodes.has_key(node))
		{
			this->nodes[node] = new TinyXml_Node(this, node);
		}
		return this->nodes[node];
	}

}

#endif