/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "Document.h"
#include "Exception.h"
#include "Node.h"

namespace hlxml
{
	Document::Document(chstr filename) : rootNode(NULL)
	{
		this->doc = xmlParseFile((filename).c_str());
		if (this->doc == NULL)
		{
			throw XMLException("Unable to parse xml file '" + filename + "', document does not exist or is invalid", NULL);
		}
	}

	Document::~Document()
	{
		xmlFreeDoc(this->doc);
	}

	Node* Document::root(chstr rootElementQuery)
	{
		if (this->rootNode != NULL)
		{
			return this->rootNode;
		}
		this->rootNode = (Node*)xmlDocGetRootElement(this->doc);
		if (this->rootNode == NULL)
		{
			hstr docname = (char*)this->doc->URL;
			xmlFreeDoc(this->doc);
			throw XMLException("No root node found in xml file '" + docname + "'", NULL);
		}
		if (rootElementQuery != "" && *this->rootNode != rootElementQuery.c_str())
		{
			hstr docname = (char*)this->doc->URL;
			xmlFreeDoc(this->doc);
			throw XMLException("xml root node type is not '" + rootElementQuery + "' in xml file '" + docname + "'", NULL);
		}
		return this->rootNode;
	}

}
