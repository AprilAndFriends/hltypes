/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "Document.h"
#include "Exception.h"
#include "Node.h"

namespace hlxml
{
	Document::Document(chstr filename) : rootNode(NULL)
	{
		this->xmlDocument = xmlParseFile((filename).c_str());
		if (this->xmlDocument == NULL)
		{
			throw XMLException("Unable to parse xml file '" + filename + "', xmlDocumentument does not exist or is invalid", NULL);
		}
	}

	Document::~Document()
	{
		xmlFreeDoc(this->xmlDocument);
	}

	Node* Document::root(chstr rootElementQuery)
	{
		if (this->rootNode != NULL)
		{
			return this->rootNode;
		}
		this->rootNode = (Node*)xmlDocGetRootElement(this->xmlDocument);
		if (this->rootNode == NULL)
		{
			hstr docname = (char*)this->xmlDocument->URL;
			xmlFreeDoc(this->xmlDocument);
			throw XMLException("No root node found in xml file '" + docname + "'", NULL);
		}
		if (rootElementQuery != "" && *this->rootNode != rootElementQuery)
		{
			hstr docname = (char*)this->xmlDocument->URL;
			xmlFreeDoc(this->xmlDocument);
			throw XMLException("xml root node type is not '" + rootElementQuery + "' in xml file '" + docname + "'", NULL);
		}
		return this->rootNode;
	}

}
