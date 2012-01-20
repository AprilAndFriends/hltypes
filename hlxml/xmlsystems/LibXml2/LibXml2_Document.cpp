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
#include <libxml/parser.h>

#include <hltypes/hstring.h>

#include "Exception.h"
#include "LibXml2_Document.h"
#include "LibXml2_Node.h"

namespace hlxml
{
	LibXml2_Document::LibXml2_Document(chstr filename) : Document(filename), rootNode(NULL)
	{
		hstr realFilename = normalize_path(filename);
		this->document = xmlParseFile(realFilename.c_str());
		if (this->document == NULL)
		{
			throw XMLException("Unable to parse xml file '" + realFilename + "', document is invalid", NULL);
		}
	}

	LibXml2_Document::~LibXml2_Document()
	{
		this->rootNode = NULL;
		foreach_map (_xmlNode*, LibXml2_Node*, it, this->nodes)
		{
			delete it->second;
		}
		this->nodes.clear();
		if (this->document != NULL)
		{
			xmlFreeDoc(this->document);
		}
	}

	Node* LibXml2_Document::root(chstr type)
	{
		if (this->rootNode == NULL)
		{
			_xmlNode* libXml2Root = xmlDocGetRootElement(this->document);
			if (libXml2Root == NULL)
			{
				throw XMLException("No root node found in XML file '" + this->filename + "'!", NULL);
			}
			this->rootNode = this->node(libXml2Root);
			if (type != "" && *this->rootNode != type)
			{
				throw XMLException("Root node type is not '" + type + "' in XML file '" + this->filename + "'!", NULL);
			}
		}
		return this->rootNode;
	}

	LibXml2_Node* LibXml2_Document::node(_xmlNode* node)
	{
		if (node == NULL)
		{
			return NULL;
		}
		if (!this->nodes.has_key(node))
		{
			this->nodes[node] = new LibXml2_Node(this, node);
		}
		return this->nodes[node];
	}

}

#endif